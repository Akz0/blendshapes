#ifndef MANIPULATOR_CLASS_H
#define MANIPULATOR_CLASS_H

#include "imGui/imgui.h"
#include "imGui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include <vector>
#include "Model.h"
#include "Shader.h"

class Manipulator {

public:
	glm::vec3 position;
	ImVec4 color;
	int vertexIndex;

	Manipulator(glm::vec3 pos, ImVec4 col, int index) :position(pos), color(col), vertexIndex(index) {}

	static float PickerSize;
	static std::vector<Manipulator> Pickers;
	static int CurrentPicker;

	static Mesh PickerMesh;
	static GLuint PickerVAO;

	static void ManipulatorInitialize();
	static void LoadManipulator();

	static void DrawPickers(Shader shader);
	static void ManipulatorCreator(int x, int y);

	static void MoveStart(int mouseX, int mouseY);
	static void MoveComplete(int mouseX, int mouseY);
};


#endif // !MANIPULATOR_CLASS_H
