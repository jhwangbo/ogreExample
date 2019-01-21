/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/


Copyright (c) 2000-2013 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

#include "raisim/RaiSimVisualizerOgre.hpp"

void imguiRenderCallBack() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {}
      if (ImGui::MenuItem("Copy", "CTRL+C")) {}
      if (ImGui::MenuItem("Paste", "CTRL+V")) {}
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  ImGui::SetNextWindowPos({0, 100});
  if (!ImGui::Begin("ImGui Demo")) {
    // Early out if the window is collapsed, as an optimization.
    ImGui::End();
    return;
  }
  ImGui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);
  ImGui::End();
}

void imguiSetupCallback() {

  ImGuiStyle& style = ImGui::GetStyle();

  style.ChildRounding = 3.f;
  style.GrabRounding = 0.f;
  style.WindowRounding = 0.f;
  style.ScrollbarRounding = 3.f;
  style.FrameRounding = 3.f;
  style.WindowTitleAlign = ImVec2(0.5f,0.5f);

  style.Colors[ImGuiCol_Text]                  = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
  style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
  style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
  style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
  style.Colors[ImGuiCol_Border]                = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
  style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
  style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
  style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
  style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
  style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
  style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
  style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
  style.Colors[ImGuiCol_Button]                = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
  style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
  style.Colors[ImGuiCol_Header]                = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_Column]                = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
  style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
  style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
  style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);

//  ImGuiIO &io = ImGui::GetIO();
//  auto *font = io.Fonts->AddFontFromFileTTF("/home/jhwangbo/workspace/ogreTest/third-party/ogre-imgui/imgui/misc/fonts/Cousine-Regular.ttf",
//                                            1.0f);
//  IM_ASSERT(font != NULL);
//  IM_ASSERT(font->IsLoaded());
//  io.FontDefault = font;
}

void setupCallback() {
  std::string file = RaiSimVisualizerOgre::getResourceDir() + "/model/monkey/monkey.obj";
  RaiSimVisualizerOgre::loadMeshFile(file, "monkeyMesh");
  RaiSimVisualizerOgre::loadMaterialFile("monkey_obj2.material");

// and finally, now that we have a named Mesh resource, we can use it
// in our createEntity() call...
  auto *monkey = RaiSimVisualizerOgre::getSceneManager()->createEntity("LocalMesh_Ent", "monkeyMesh");
  monkey->getSubEntity(0)->setMaterialName("monkey");
  monkey->getMesh()->buildTangentVectors(); // enforce that we have tangent vectors

  MaterialPtr mat = MaterialManager::getSingleton().getByName("monkey");
  auto mParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
  mParams->setNamedConstant("u_ScaleIBLAmbient", Vector4(Real(0.5)));

  SceneNode *monkeyNode = RaiSimVisualizerOgre::getSceneManager()->getRootSceneNode()->createChildSceneNode();
  monkeyNode->attachObject(monkey);
  monkeyNode->setPosition(0, 0, 0);
  RaiSimVisualizerOgre::getCameraMan()->setTarget(monkeyNode);
}

int main(int argc, char **argv) {
  RaiSimVisualizerOgre::getSingleton()->setImguiSetupCallback(imguiSetupCallback);
  RaiSimVisualizerOgre::getSingleton()->setImguiRenderCallback(imguiRenderCallBack);
  RaiSimVisualizerOgre::getSingleton()->setSetUpCallback(setupCallback);
  RaiSimVisualizerOgre::getSingleton()->initApp();
  RaiSimVisualizerOgre::getSingleton()->renderOneFrame();
  RaiSimVisualizerOgre::getSingleton()->closeApp();

  return 0;
}

//! [fullsource]
