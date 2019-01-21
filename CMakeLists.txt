cmake_minimum_required(VERSION 3.13)
project(raisimOgreVisualizer)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_MODULE_PATH "/usr/share/OGRE/cmake/modules" CACHE STRING "Modules for CMake" FORCE)

set(OGRE_IMGUI_DIR ${CMAKE_SOURCE_DIR}/third-party/ogre-imgui)
set(OGRE_ASSIMP_DIR ${CMAKE_SOURCE_DIR}/third-party/ogre-assimp)
set(OGRE_AVI_SAVER ${CMAKE_SOURCE_DIR}/third-party/SaveAvi)

find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})
find_package(ASSIMP REQUIRED)

include_directories(${OGRE_INCLUDE_DIRS})
include_directories(${OGRE_IMGUI_DIR}/imgui)
include_directories(${OGRE_IMGUI_DIR})
include_directories(${OGRE_ASSIMP_DIR}/src)
#include_directories(${OGRE_AVI_SAVER}/include)

### ogre imgui
set(IMGUI_SRC
        ${OGRE_IMGUI_DIR}/imgui/imgui.cpp
        ${OGRE_IMGUI_DIR}/imgui/imgui_draw.cpp
        ${OGRE_IMGUI_DIR}/imgui/imgui_widgets.cpp
        ${OGRE_IMGUI_DIR}/imgui/imgui_demo.cpp)

set(OGRE_IMGUI_SRC
        ${OGRE_IMGUI_DIR}/ImguiManager.cpp
        ${OGRE_IMGUI_DIR}/ImguiRenderable.cpp)

set(OGRE_ASSIMP_SRC
        ${OGRE_ASSIMP_DIR}/src/AssimpLoader.cpp
        ${OGRE_ASSIMP_DIR}/src/AssimpLoader.h)

#set(OGRE_AVI_SAVER_SRC
#        ${OGRE_AVI_SAVER}/src/AVIGenerator.cpp
#        ${OGRE_AVI_SAVER}/src/OgreAviSaver.cpp
#        ${OGRE_AVI_SAVER}/src/OgreAviSaverDll.cpp)

set(RAISIM_VISUALIZER_INC_DIR include)

set(RAISIM_VISUALIZER_HDR
        include/raisim/OgreVis.hpp
        include/raisim/interfaceClasses.hpp)

set(RAISIM_VISUALIZER_SRC
        src/OgreVis.cpp)

include_directories(${RAISIM_VISUALIZER_HDR_DIR})

add_library(raisim_visualizer ${RAISIM_VISUALIZER_SRC} ${RAISIM_VISUALIZER_HDR} ${IMGUI_SRC} ${OGRE_IMGUI_SRC} ${OGRE_ASSIMP_SRC} ${OGRE_AVI_SAVER_SRC})
target_link_libraries(raisim_visualizer ${ASSIMP_LIBRARIES} ${OGRE_LIBRARIES})
target_include_directories(raisim_visualizer PUBLIC ${RAISIM_VISUALIZER_INC_DIR})

add_executable(ogreTest main.cpp)
target_link_libraries(ogreTest raisim_visualizer)
target_include_directories(ogreTest PUBLIC)

### configure resource directory file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resourceDir.rg.in
        ${CMAKE_CURRENT_BINARY_DIR}/resourceDir.rg @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/resourceDir.rg
        DESTINATION ${CMAKE_BINARY_DIR})