//
// Created by jhwangbo on 22.12.18.
//

#ifndef RAISIMVISUALIZERFILAMENT_RSVISUALIZERFILAMENT_HPP
#define RAISIMVISUALIZERFILAMENT_RSVISUALIZERFILAMENT_HPP

#include "helperFunctions.hpp"
#include "raiSim/World.hpp"
#include "filament/Engine.h"

namespace raisim {

class GraphicObject {
 public:
  GraphicObject() {
    rotationOffset.setIdentity();
  }
  Renderable graphics = Renderable(0);
  Vec<3> scale = {1.0, 1.0, 1.0}, offset = {.0, .0, .0};
  Mat<3, 3> rotationOffset;
  unsigned long int group = 1ul;
  size_t localId = 0;
};

class ObjectSet {
 public:
  ObjectSet(raisim::Object *ob, std::vector<GraphicObject> graphics)
      : raiSimObject(ob), graphicObjects(std::move(graphics)) {
  }

  raisim::Object *raiSimObject;
  std::vector<GraphicObject> graphicObjects;
};

class RsVisualizerFilament {

 public:

  RsVisualizerFilament();
  ~RsVisualizerFilament();

  void registerSet(raisim::Object *ob,
                   const std::vector<GraphicObject> &graphics,
                   bool removeExistingEntry = false);

  void updateObject(filament::Engine *engine, const ObjectSet &os);

  void update(filament::Engine *engine);

  void remove(raisim::World &world, raisim::Object *ob);

  void init(const Config &config,
            RaiSimFilamentApp::SetupCallback setupCallback,
            size_t width = 1024,
            size_t height = 640,
            bool useGui = false);
  void run(const Config &config,
           RaiSimFilamentApp::ImGuiCallback imgui = RaiSimFilamentApp::ImGuiCallback(),
           RaiSimFilamentApp::PreRenderCallback preRender = RaiSimFilamentApp::PreRenderCallback(),
           RaiSimFilamentApp::PostRenderCallback postRender = RaiSimFilamentApp::PostRenderCallback());
  void end(RaiSimFilamentApp::CleanupCallback cleanup);

 private:
  RaiSimFilamentApp *app;
  std::vector<ObjectSet> objectSet_;
  ulong visualMask_ = ulong(1);
  RaiSimFilamentApp::AnimCallback animateCallback;

};

}

#endif //RAISIMVISUALIZERFILAMENT_RSVISUALIZERFILAMENT_HPP
