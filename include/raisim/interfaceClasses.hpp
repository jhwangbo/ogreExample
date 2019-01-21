//
// Created by jhwangbo on 21.01.19.
//

#ifndef RAISIMOGREVISUALIZER_INTERFACECLASSES_HPP
#define RAISIMOGREVISUALIZER_INTERFACECLASSES_HPP

#include "Ogre.h"
#include "raisim/World.hpp"

namespace raisim {

class GraphicObject {
 public:
  GraphicObject() {
    rotationOffset.setIdentity();
  }
  Ogre::SceneNode* graphics;
  raisim::Vec<3> scale = {1.0, 1.0, 1.0}, offset = {.0, .0, .0};
  raisim::Mat<3, 3> rotationOffset;
  unsigned long int group = 1ul;
  size_t localId = 0;
};

class ObjectSet {
 public:
  ObjectSet(raisim::Object *ob, std::vector <GraphicObject> graphics)
      : raiSimObject(ob), graphicObjects(std::move(graphics)) {
  }

  raisim::Object *raiSimObject;
  std::vector <GraphicObject> graphicObjects;
};

}

#endif //RAISIMOGREVISUALIZER_INTERFACECLASSES_HPP
