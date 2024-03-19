#include "Manipulator.h"
float Manipulator::PickerSize = 10.f;
std::vector<Manipulator> Manipulator::Pickers;
Mesh Manipulator::PickerMesh;
GLuint Manipulator::PickerVAO;
int Manipulator::CurrentPicker;

void Manipulator::ManipulatorInitialize() {
	Mesh mesh("./src/test-model/picker.obj");
	PickerMesh = mesh;
	LoadManipulator();
}

void Manipulator::LoadManipulator() {
	GLuint aPos = 0;
	GLuint aNormal = 1;
	GLuint aTexCords = 2;
	GLuint aTangent = 3;
	GLuint aBiTangent = 4;

	unsigned int aPosVBO = 0;
	unsigned int aTexCordsVBO = 0;
	unsigned int aNormalVBO = 0;
	unsigned int aTangentVBO = 0;
	unsigned int aBiTangentVBO = 0;

	glGenBuffers(1, &aPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, aPosVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * Manipulator::PickerMesh.totalPoints * sizeof(float), Manipulator::PickerMesh.VertexData.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &aNormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, aNormalVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * Manipulator::PickerMesh.totalPoints * sizeof(float), &Manipulator::PickerMesh.normals[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &Manipulator::PickerVAO);
	glBindVertexArray(Manipulator::PickerVAO);

	glEnableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, aPosVBO);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(aNormal);
	glBindBuffer(GL_ARRAY_BUFFER, aNormalVBO);
	glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}


void Manipulator::DrawPickers(Shader shader) {

	for (int i = 0; i < Manipulator::Pickers.size(); i++) {
		glm::mat4 picker_model = glm::mat4(1.0f);
		picker_model = glm::translate(picker_model, Manipulator::Pickers[i].position);
		picker_model = scale(picker_model, glm::vec3(0.3f));

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(picker_model));
		glUniform3f(glGetUniformLocation(shader.ID, "Color"), Pickers[i].color.x, Pickers[i].color.y, Pickers[i].color.z);
		glBindVertexArray(Manipulator::PickerVAO);
		glDrawArrays(GL_TRIANGLES, 0, Manipulator::PickerMesh.totalPoints);
	}
}
