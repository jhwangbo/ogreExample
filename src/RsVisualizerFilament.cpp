//
// Created by jhwangbo on 22.12.18.
//

#include "raiSim/RsVisualizerFilament.hpp"

namespace raisim {

RsVisualizerFilament::RsVisualizerFilament() {
  app = &RaiSimFilamentApp::get();
  RaiSimFilamentApp::AnimCallback animCallback;
  animCallback = [this](filament::Engine *engine, filament::View *view, double now){ this->update(engine); };
  app->animate(animCallback);
}

RsVisualizerFilament::~RsVisualizerFilament() {

}

void RsVisualizerFilament::registerSet(raisim::Object *ob,
                                       const std::vector<GraphicObject> &graphics,
                                       bool removeExistingEntry) {
  const auto &match = std::find_if(
      objectSet_.begin(), objectSet_.end(),
      [&ob](const ObjectSet x) { return x.raiSimObject == ob; });

  if (match == objectSet_.end())
    objectSet_.emplace_back(ob, graphics);
  else {
    if (removeExistingEntry) match->graphicObjects.clear();
    match->graphicObjects.insert(match->graphicObjects.end(), graphics.begin(), graphics.end());
  }
}

void RsVisualizerFilament::updateObject(filament::Engine *engine, const ObjectSet &os) {
  Vec<3> pos, offsetInWorld;
  Mat<3, 3> bodyRotation, rot;

  auto &rcm = engine->getRenderableManager();
  auto &tcm = engine->getTransformManager();

  for (auto &grp : os.graphicObjects) {
    if (!rcm.hasComponent(g_meshSet->getRenderables()[grp.graphics]) || visualMask_ & grp.group) continue;

    os.raiSimObject->getPosition_W(grp.localId, pos);
    os.raiSimObject->getRotation_W(grp.localId, bodyRotation);

    auto ti = tcm.getInstance(g_meshSet->getRenderables()[grp.graphics]);
    matvecmul(bodyRotation, grp.offset, offsetInWorld);
    matmul(bodyRotation, grp.rotationOffset, rot);
    math::mat3f rotationFilament(rot[0], rot[3], rot[6],
                                 rot[1], rot[4], rot[7],
                                 rot[2], rot[5], rot[8]);
    math::float3 positionFilament(pos[0] + offsetInWorld[0], pos[1] + offsetInWorld[1], pos[2] + offsetInWorld[2]);
    tcm.setTransform(ti, math::mat4f(rotationFilament, positionFilament));
  }
}

void RsVisualizerFilament::update(filament::Engine *engine) {
  for (auto &os : objectSet_)
    updateObject(engine, os);
}

void RsVisualizerFilament::remove(raisim::World &world, raisim::Object *ob) {
  const auto &match = std::find_if(
      objectSet_.begin(), objectSet_.end(),
      [&ob](const ObjectSet x) { return x.raiSimObject == ob; });
  world.removeObject(ob);
  objectSet_.erase(match);
}

void RsVisualizerFilament::init(const Config &config,
                                RaiSimFilamentApp::SetupCallback setupCallback,
                                size_t width,
                                size_t height,
                                bool useGui) {
  app->init(config, setupCallback, width, height, useGui);
}

void RsVisualizerFilament::run(const Config &config,
                               RaiSimFilamentApp::ImGuiCallback imgui,
                               RaiSimFilamentApp::PreRenderCallback preRender,
                               RaiSimFilamentApp::PostRenderCallback postRender) {
  app->run(config, imgui, preRender, postRender);
}

void RsVisualizerFilament::end(RaiSimFilamentApp::CleanupCallback cleanup) {
  app->end(cleanup);
}

}
