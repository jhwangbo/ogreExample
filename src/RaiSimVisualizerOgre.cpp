//
// Created by jhwangbo on 17.01.19.
//

#include "raisim/RaiSimVisualizerOgre.hpp"

std::string RaiSimVisualizerOgre::resourceDir_;
SceneManager *RaiSimVisualizerOgre::scnMgr_;
CameraMan *RaiSimVisualizerOgre::cameraMan_;

inline char separator() {
#ifdef _WIN32
  return '\\';
#else
  return '/';
#endif
}

std::string getPathName(const std::string &s) {

  char sep = '/';

#ifdef _WIN32
  sep = '\\';
#endif

  size_t i = s.rfind(sep, s.length());
  if (i != std::string::npos) {
    return (s.substr(0, i));
  }

  return ("");
}

RaiSimVisualizerOgre::~RaiSimVisualizerOgre() {

}

bool RaiSimVisualizerOgre::mouseMoved(const MouseMotionEvent &evt) {
  mouseX_ = evt.x;
  mouseY_ = evt.y;
  if (ImguiManager::getSingleton().mouseMoved(evt)) return true;
  if (leftMouseButtonPressed_)
    if (cameraMan_->mouseMoved(evt))
      return true;

//  if(rightMouseButtonPressed_) {
//    auto position = cameraMan_->getCamera()->getPosition();
//  }

  return true;
}

void RaiSimVisualizerOgre::addResourceDirectory(const std::string &dir, bool recursivelyAdd) {
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dir,
                                                                 "FileSystem",
                                                                 ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                 recursivelyAdd,
                                                                 true);
  // Initialise the resource groups:
  ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void RaiSimVisualizerOgre::loadMaterialFile(const std::string &filename) {
  DataStreamPtr ds = Ogre::ResourceGroupManager::getSingleton().openResource(filename,
                                                                             ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  Ogre::MaterialManager::getSingleton().parseScript(ds, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

bool fileExists(const std::string &filename) {
  return fopen(filename.c_str(), "rb");
}

void RaiSimVisualizerOgre::loadMeshFile(const std::string &filename, const std::string &meshName) {
  if (!fileExists(filename))
    OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
                "File " + filename + " not found.",
                "OgreMeshLoaded");

  std::string meshFilename;
  meshFilename = filename;
  std::string extension = filename.substr(filename.find_last_of('.') + 1);
  std::string baseFilename = filename.substr(filename.find_last_of(separator()) + 1,
                                             filename.size() - filename.find_last_of(separator()) - extension.size()
                                                 - 2);
  std::string path = getPathName(filename);

  if (extension != "mesh") {
    meshFilename = path + separator() + baseFilename + "_" + extension + ".mesh";

    AssimpLoader::AssOptions opts;
    opts.quietMode = false;
    opts.logFile = "ass.log";
    opts.customAnimationName = "";
    opts.dest = "";
    opts.animationSpeedModifier = 1.0;
    opts.lodValue = 250000;
    opts.lodFixed = 0;
    opts.lodPercent = 20;
    opts.numLods = 0;
    opts.usePercent = true;
    opts.params = AssimpLoader::LP_GENERATE_SINGLE_MESH | AssimpLoader::LP_GENERATE_MATERIALS_AS_CODE;
    opts.source = filename;

    AssimpLoader loader;
    if (!loader.convert(opts))
      OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "file conversion failed");
  }

  FILE *pFile = fopen(meshFilename.c_str(), "rb");
  if (!pFile) OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "File " + meshFilename + " not found.", "OgreMeshLoaded");

  struct stat tagStat;
  stat(meshFilename.c_str(), &tagStat);
//  MemoryDataStream* memstream = new MemoryDataStream(meshFilename, tagStat.st_size, true);
  MemoryDataStream* memoryDataStream = new MemoryDataStream(meshFilename, tagStat.st_size, true);

  fread((void *) memoryDataStream->getPtr(), tagStat.st_size, 1, pFile);
  fclose(pFile);

// give the resource a name -- it can be the full pathname if you like, since it's
// just going to be the key in an STL associative tree container
  MeshPtr pMesh = MeshManager::getSingleton().createManual(meshName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

// this part does the actual load into the live Mesh object created above
  MeshSerializer meshSerializer;
  DataStreamPtr stream(memoryDataStream);
  meshSerializer.importMesh(stream, pMesh.get());
}

void RaiSimVisualizerOgre::generateMeshFile(const std::string &filename) {

}

bool RaiSimVisualizerOgre::mouseWheelRolled(const MouseWheelEvent &evt) {
  if (cameraMan_->mouseWheelRolled(evt)) return true;
//  if(cameraMan_->getStyle() == CS_FREELOOK)
//    cameraMan_->

  return true;
}

bool RaiSimVisualizerOgre::mousePressed(const MouseButtonEvent &evt) {
  if (ImguiManager::getSingleton().mousePressed(evt)) return true;

  switch (evt.button) {
    case BUTTON_LEFT:leftMouseButtonPressed_ = true;
      if (hovered_) {
        selected_ = hovered_;
        cameraMan_->setStyle(CS_ORBIT);
        cameraMan_->setTarget(selected_);
      } else {
        if (selected_) selected_->showBoundingBox(false);
        selected_ = nullptr;
      }
      break;
    case BUTTON_RIGHT:cameraMan_->setStyle(CS_FREELOOK);
      if (selected_) selected_->showBoundingBox(false);
      selected_ = nullptr;
      rightMouseButtonPressed_ = true;
      break;
    default:break;
  }

  if (cameraMan_->mousePressed(evt)) return true;
  return true;
}

bool RaiSimVisualizerOgre::mouseReleased(const MouseButtonEvent &evt) {
  if (ImguiManager::getSingleton().mouseReleased(evt)) return true;

  switch (evt.button) {
    case BUTTON_LEFT:leftMouseButtonPressed_ = false;
      break;
    case BUTTON_RIGHT:rightMouseButtonPressed_ = false;
      break;
    default:break;
  }

  if (cameraMan_->mouseReleased(evt)) return true;
  return true;
}

bool RaiSimVisualizerOgre::frameRenderingQueued(const Ogre::FrameEvent &evt) {
  cameraMan_->frameRendered(evt);   // if dialog isn't up, then update the camera

  Ogre::Vector3 camPos = mainCamera_->getRealPosition();
  Ogre::Ray cameraRay = mainCamera_->getCameraToViewportRay(mouseX_ / float(getRenderWindow()->getWidth()),
                                                            mouseY_ / float(getRenderWindow()->getHeight()));
  raySceneQuery_->setRay(cameraRay);

  raySceneQuery_->setSortByDistance(true);
  Ogre::RaySceneQueryResult &result = raySceneQuery_->execute();
  auto iter = result.begin();
  if (iter != result.end())
    hovered_ = iter->movable->getParentSceneNode();
  else
    hovered_ = nullptr;

  if (selected_) selected_->showBoundingBox(true);

  return true;
}

void RaiSimVisualizerOgre::frameRendered(const Ogre::FrameEvent &evt) {
  ImguiManager::getSingleton().frameRendered(evt);
  cameraMan_->frameRendered(evt);
}

bool RaiSimVisualizerOgre::frameStarted(const Ogre::FrameEvent &evt) {
  ApplicationContext::frameStarted(evt);
  ImguiManager::getSingleton().newFrame(
      evt.timeSinceLastFrame,
      Ogre::Rect(0, 0, getRenderWindow()->getWidth(), getRenderWindow()->getHeight()));
  if (imGuiRenderCallback_) imGuiRenderCallback_();
  cameraMan_->setTopSpeed(cameraSpeed_);
  return true;
}

bool RaiSimVisualizerOgre::keyPressed(const KeyboardEvent &evt) {
  auto &key = evt.keysym.sym;
  // termination gets the highest priority
  if (key == SDLK_ESCAPE) getRoot()->queueEndRendering();

  if (keyboardCallback_)
    if (keyboardCallback_(evt))
      return true;

  if (cameraMan_->keyPressed(evt)) return true;

  switch (key) {
    default:break;
  }
  return true;
}

bool RaiSimVisualizerOgre::keyReleased(const KeyboardEvent &evt) {
  if (cameraMan_->keyReleased(evt)) return true;
  return true;
}

void RaiSimVisualizerOgre::shutdown() {
  delete cameraMan_;
}

RaiSimVisualizerOgre::RaiSimVisualizerOgre()
    : ApplicationContext("OgreTutorialApp") {
  std::ifstream inFile;
  inFile.open("resourceDir.rg");
  if (!inFile) {
    std::cerr << "Unable to open file datafile.txt";
    exit(1);   // call system to stop
  }
  std::getline(inFile, resourceDir_);
}

void RaiSimVisualizerOgre::setup() {
  // do not forget to call the base first
  mRoot->initialise(false);
  std::map<std::string, std::string> param;
  param["FSAA"] = "4";
  createWindow(mAppName, 3000, 2000, param);

  locateResources();
  initialiseRTShaderSystem();
  loadResources();

  // adds context as listener to process context-level (above the sample level) events
  mRoot->addFrameListener(this);

  addInputListener(this);

  // imgui
  Ogre::ImguiManager::createSingleton();
  addInputListener(Ogre::ImguiManager::getSingletonPtr());
  if(imGuiSetupCallback_) imGuiSetupCallback_();

  // get a pointer to the already created root
  Root *root = getRoot();
  scnMgr_ = root->createSceneManager();
  Ogre::ImguiManager::getSingleton().init(scnMgr_);

  // register our scene with the RTSS
  RTShader::ShaderGenerator *shadergen = RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(scnMgr_);

  // -- tutorial section start --
  //! [turnlights]
  scnMgr_->setAmbientLight(ColourValue(1, 1, 1));
  raySceneQuery_ = scnMgr_->createRayQuery(Ogre::Ray());
  //! [turnlights]

  //! [newlight]
  light_ = scnMgr_->createLight("mainLight");
  lightNode_ = scnMgr_->getRootSceneNode()->createChildSceneNode();
  light_->setDiffuseColour(1, 1, 1);
//  light->set
  lightNode_->attachObject(light_);
  lightNode_->setPosition(50, 10, 50);
  //! [lightpos]

  //! [camera]
  camNode_ = scnMgr_->getRootSceneNode()->createChildSceneNode();

  // create the camera
  mainCamera_ = scnMgr_->createCamera("myCam");
  mainCamera_->setNearClipDistance(0.01); // specific to this sample
  mainCamera_->setAutoAspectRatio(true);
  camNode_->attachObject(mainCamera_);
  camNode_->setPosition(0, 10, 5);
  cameraMan_ = new CameraMan(camNode_);   // create a default camera controller
  cameraMan_->setStyle(CS_ORBIT);

  // and tell it to render into the main window
  viewport_ = getRenderWindow()->addViewport(mainCamera_);
//  viewPort->setBackgroundColour(ColourValue(0.05, 0.05, 0.05));
  //! [camera]

  /// add default resource directory
  addResourceDirectory(resourceDir_+"/pbr", true);
  loadMaterialFile("rsPBR.material");

  Ogre::TexturePtr rttTexture =
      Ogre::TextureManager::getSingleton().createManual(
          "RttTex",
          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
          Ogre::TEX_TYPE_2D,
          getRenderWindow()->getWidth(), getRenderWindow()->getHeight(),
          0,
          Ogre::PF_R8G8B8,
          Ogre::TU_RENDERTARGET);

// process data
  Ogre::RenderTexture* renderTexture = rttTexture->getBuffer()->getRenderTarget();
  renderTexture->update();


  if(setUpCallback_) setUpCallback_();

  /// raisim pbr material

}

