//
// Created by jhwangbo on 17.01.19.
//

#ifndef OGRETEST_RAISIMVISUALIZER_HPP
#define OGRETEST_RAISIMVISUALIZER_HPP

//! [fullsource]

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreApplicationContext.h"
#include <iostream>
#include <Bites/OgreCameraMan.h>
#include <Bites/OgreTrays.h>
#include "ImguiManager.h"
#include "AssimpLoader.h"


using namespace Ogre;
using namespace OgreBites;

class RaiSimVisualizerOgre
    : public ApplicationContext, public InputListener, public TrayListener {

 public:

  static RaiSimVisualizerOgre* getSingleton() {
    static RaiSimVisualizerOgre singleton;
    return &singleton;
  }

  using ImGuiRenderCallback = std::function<void()>;
  using ImGuiSetupCallback = std::function<void()>;
  using KeyboardCallback = std::function<bool(const KeyboardEvent &)>;
  using SetUpCallback = std::function<void()>;

  void setImguiRenderCallback(ImGuiRenderCallback callback) { imGuiRenderCallback_ = callback; }
  void setImguiSetupCallback(ImGuiSetupCallback callback) { imGuiSetupCallback_ = callback; }
  void setKeyboardCallback(KeyboardCallback callback) { keyboardCallback_ = callback; }
  void setSetUpCallback(SetUpCallback callback) { setUpCallback_ = callback; }
  void addResourceDirectory(const std::string& dir, bool recursivelyAdd = false);
  static void loadMaterialFile(const std::string& filename);
  static void loadMeshFile(const std::string& filename, const std::string& meshName);
  static const std::string& getResourceDir() { return resourceDir_; }
  static SceneManager* getSceneManager() { return scnMgr_; }
  static CameraMan* getCameraMan() { return cameraMan_; }
  Light* getLight() { return light_; }
  SceneNode* getLightNode() { return lightNode_; }
  Viewport* getViewPort() {return viewport_; }
  void startRendering() { getRoot()->startRendering(); }
  void renderOneFrame() { getRoot()->renderOneFrame(); }

 private:

  RaiSimVisualizerOgre();
  ~RaiSimVisualizerOgre() final;
  void generateMeshFile(const std::string& filename);
  void setup() final;
  void shutdown() final;
  bool keyPressed(const KeyboardEvent &evt) final;
  bool keyReleased(const KeyboardEvent &evt) final;
  bool mousePressed(const MouseButtonEvent &evt) final;
  bool mouseReleased(const MouseButtonEvent &evt) final;
  bool mouseMoved(const MouseMotionEvent &evt) final;
  bool mouseWheelRolled(const MouseWheelEvent& evt) final;
  bool frameRenderingQueued(const Ogre::FrameEvent &evt) final;
  void frameRendered(const Ogre::FrameEvent &evt) final;
  bool frameStarted(const Ogre::FrameEvent &evt) final;

  ImGuiRenderCallback imGuiRenderCallback_ = nullptr;
  ImGuiSetupCallback imGuiSetupCallback_ = nullptr;
  KeyboardCallback keyboardCallback_ = nullptr;
  SetUpCallback setUpCallback_ = nullptr;
  SceneNode *camNode_ = nullptr;
  Camera *mainCamera_ = nullptr;
  static CameraMan *cameraMan_;
  RaySceneQuery *raySceneQuery_ = nullptr;
  float cameraSpeed_ = 100.f;
  bool leftMouseButtonPressed_ = false;
  bool rightMouseButtonPressed_ = false;
  int mouseX_=0, mouseY_=0;
  static std::string resourceDir_;
  SceneNode *selected_ = nullptr, *hovered_ = nullptr;
  static SceneManager *scnMgr_;
  Light *light_;
  SceneNode *lightNode_;
  Viewport* viewport_;
  GpuProgramParametersSharedPtr mParams;
};


#endif //OGRETEST_RAISIMVISUALIZER_HPP
