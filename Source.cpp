

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/cimport.h> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "glm/ext.hpp"

#include <iostream>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <vector> 
#include <list>
#include <algorithm>

#include "src/Shader.h"
#include "src/Model.h"
#include "src/Utilities.h"
#include "src/Manipulator.h"

#include <limits>

std::vector<float> MultiplyScalar(std::vector<float> vec, float num);

GLFWwindow* window;
void WindowResizingHandler(GLFWwindow* window, int width, int height);
void FramebufferHandler(GLFWwindow* window, int width, int height);
void MouseButtonHandler(GLFWwindow* window, int button, int action, int mods);
void MouseHandler(GLFWwindow* window, double xposIn, double yposIn);
void processInput(GLFWwindow* window);


double lastFrameTime = glfwGetTime();
double currentTime;
float deltaTime = currentTime - lastFrameTime;
int frameCount = 0;






Camera camera;
Mouse mouse;
glm::mat4 model;



const char* label[] = {
	"NEUTRAL", "JAW_OPEN", "KISS", "L_BROW_LOWER", "L_BROW_NARROW",
	"L_BROW_RAISE", "LEFT_CLOSED", "L_LOWER_O", "L_UPPER_O",
	"L_NOSE_WRINKLE", "L_PUFF", "L_SAD", "L_SMILE", "L_SUCK",
	"R_BROW_LOWER", "R_BROW_NARROW", "R_BROW_RAISE", "REFT_CLOSED",
	"R_LOWER_O", "R_UPPER_O", "R_NOSE_WRINKLE", "R_PUFF", "R_SAD",
	"R_SMILE", "R_SUCK"
};

//const char* label[] = {
//	"NEUTRAL", "JAW_OPEN", "KISS",
//	"LEFT_CLOSED",  "L_SMILE", "L_PUFF", "L_SAD",
//	"RIGHT_CLOSED", "R_PUFF", "R_SAD", "R_SMILE",
//};

const char* TestLabel[] = {
	"Basic","Smooth Vertex","Sphere"
};

void LoadLowResFace() {
	Mesh Neutral(NEUTRAL_LOW_RES);
	Mesh JawOpen(JAW_OPEN_LOW_RES);
	Mesh Kiss(KISS_LOW_RES);
	Mesh LBrowLower(L_BROW_LOWER_LOW_RES);
	Mesh LBrowNarrow(L_BROW_NARROW_LOW_RES);
	Mesh LBrowRaise(L_BROW_RAISE_LOW_RES);
	Mesh LeftClosed(LEFT_CLOSED_LOW_RES);
	Mesh LLowerOpen(L_LOWER_O_LOW_RES);
	Mesh LUpperOpen(L_UPPER_O_LOW_RES);
	Mesh LNoseWrinkle(L_NOSE_WRINKLE_LOW_RES);
	Mesh LPuff(L_PUFF_LOW_RES);
	Mesh LSad(L_SAD_LOW_RES);
	Mesh LSmile(L_SMILE_LOW_RES);
	Mesh LSuck(L_SUCK_LOW_RES);
	Mesh RBrowLower(R_BROW_LOWER_LOW_RES);
	Mesh RBrowNarrow(R_BROW_NARROW_LOW_RES);
	Mesh RBrowRaise(R_BROW_RAISE_LOW_RES);
	Mesh RightClosed(R_CLOSED_LOW_RES);
	Mesh RLowerOpen(R_LOWER_O_LOW_RES);
	Mesh RUpperOpen(R_UPPER_O_LOW_RES);
	Mesh RNoseWrinkle(R_NOSE_WRINKLE_LOW_RES);
	Mesh RPuff(R_PUFF_LOW_RES);
	Mesh RSad(R_SAD_LOW_RES);
	Mesh RSmile(R_SMILE_LOW_RES);
	Mesh RSuck(R_SUCK_LOW_RES);
}
void LoadHighResFace() {
	Mesh Neutral(NEUTRAL);
	Mesh JawOpen(JAW_OPEN);
	Mesh Kiss(KISS);
	Mesh LBrowLower(L_BROW_LOWER);
	Mesh LBrowNarrow(L_BROW_NARROW);
	Mesh LBrowRaise(L_BROW_RAISE);
	Mesh LeftClosed(LEFT_CLOSED);
	Mesh LLowerOpen(L_LOWER_O);
	Mesh LUpperOpen(L_UPPER_O);
	Mesh LNoseWrinkle(L_NOSE_WRINKLE);
	Mesh LPuff(L_PUFF);
	Mesh LSad(L_SAD);
	Mesh LSmile(L_SMILE);
	Mesh LSuck(L_SUCK);
	Mesh RBrowLower(R_BROW_LOWER);
	Mesh RBrowNarrow(R_BROW_NARROW);
	Mesh RBrowRaise(R_BROW_RAISE);
	Mesh RightClosed(RIGHT_CLOSED);
	Mesh RLowerOpen(R_LOWER_O);
	Mesh RUpperOpen(R_UPPER_O);
	Mesh RNoseWrinkle(R_NOSE_WRINKLE);
	Mesh RPuff(R_PUFF);
	Mesh RSad(R_SAD);
	Mesh RSmile(R_SMILE);
	Mesh RSuck(R_SUCK);
}
void LoadTestModels() {
	Mesh Key1("./src/test-model/Basis.obj");
	Mesh Key2("./src/test-model/Smooth Vertex.obj");
	Mesh Key3("./src/test-model/Circle.obj");
}
int main(void) {
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, WindowResizingHandler);
	glfwSetCursorPosCallback(window, MouseHandler);
	glfwSetMouseButtonCallback(window, MouseButtonHandler);

	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
	glfwSwapInterval(0);

	Shader phongShader("./src/shaders/phong.vert", "./src/shaders/phong.frag");

	WindowResizingHandler(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Setup IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	
	//LoadTestModels();
	//LoadHighResFace();
	LoadLowResFace();
	Model::Initialize();
	Manipulator::ManipulatorInitialize();

#pragma region Inspector Controls
	glm::vec3 finalPosition(0.0,0.0,0.0f);
	glm::vec3 finalScale(1.0f);
	glm::vec3 finalRotation(0.0f);
	static ImVec4 finalMeshColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); 

	//Light Information
	static ImVec4 LightColor = ImVec4(1.0, 1.0, 1.0, 1.0);
	glm::vec3 LightPosition(0.0, 200.0, 75.0);
	static float LightPower = 1.0f;
	static float AmbientPower = 0.2f;
	static float DiffusePower = 0.2f;

	//
	bool isAnimate = false;
	int lineNumber = 0;

#pragma endregion

	while (!glfwWindowShouldClose(window))
	{	

#pragma region Basic OpenGL Setup
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentTime = glfwGetTime();
		frameCount++;

		deltaTime = currentTime - lastFrameTime;
		if (currentTime - lastFrameTime >= 1.0)
		{
			glfwSetWindowTitle(window, std::to_string(frameCount).c_str());
			frameCount = 0;
			lastFrameTime = currentTime;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#pragma endregion

		processInput(window);
		phongShader.Activate();

#pragma region Set Uniforms
		//Update Transforms
		model = glm::translate(glm::mat4(1.0f), finalPosition);
		model = glm::rotate(model, finalRotation.x * glm::radians(1.f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, finalRotation.y * glm::radians(1.f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, finalRotation.z * glm::radians(1.f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, finalScale);
		
		//UpdateShader With Camera and Light Information
		glUniform3f(glGetUniformLocation(phongShader.ID, "LightPosition"), LightPosition.x, LightPosition.y, LightPosition.z);
		glUniform3f(glGetUniformLocation(phongShader.ID, "LightColor"), LightColor.x, LightColor.y, LightColor.z);
		glUniform1f(glGetUniformLocation(phongShader.ID, "LightPower"), LightPower);
		glUniform1f(glGetUniformLocation(phongShader.ID, "AmbientPower"), AmbientPower);
		glUniform1f(glGetUniformLocation(phongShader.ID, "DiffusePower"), DiffusePower);

		glUniform3f(glGetUniformLocation(phongShader.ID, "CameraPosition"), camera.position.x, camera.position.y, camera.position.z);
		glUniform3f(glGetUniformLocation(phongShader.ID, "Color"), finalMeshColor.x, finalMeshColor.y, finalMeshColor.z);
#pragma endregion

#pragma region Animate Shapes

		if (isAnimate && lineNumber < 6000) {
				Model::weights[0] = Model::animation_data[lineNumber];
				Model::weights[1] = Model::animation_data[lineNumber + 1];
				Model::weights[2] = Model::animation_data[lineNumber + 2];
				Model::weights[3] = Model::animation_data[lineNumber + 3];
				Model::weights[4] = Model::animation_data[lineNumber + 4];
				Model::weights[5] = Model::animation_data[lineNumber + 5];
				Model::weights[6] = Model::animation_data[lineNumber + 6];
				Model::weights[7] = Model::animation_data[lineNumber + 7];
				Model::weights[8] = Model::animation_data[lineNumber + 8];
				Model::weights[9] = Model::animation_data[lineNumber + 9];
				Model::weights[10] = Model::animation_data[lineNumber + 10];
				Model::weights[11] = Model::animation_data[lineNumber + 11];
				Model::weights[12] = Model::animation_data[lineNumber + 12];
				Model::weights[13] = Model::animation_data[lineNumber + 13];
				Model::weights[14] = Model::animation_data[lineNumber + 14];
				Model::weights[15] = Model::animation_data[lineNumber + 15];
				Model::weights[16] = Model::animation_data[lineNumber + 16];

				Model::weights[17] = Model::animation_data[lineNumber + 17];
				Model::weights[18] = Model::animation_data[lineNumber + 18];
				Model::weights[19] = Model::animation_data[lineNumber + 19];
				Model::weights[20] = Model::animation_data[lineNumber + 20];
				Model::weights[21] = Model::animation_data[lineNumber + 21];
				Model::weights[22] = Model::animation_data[lineNumber + 22];
				Model::weights[23] = Model::animation_data[lineNumber + 23];

				std::cout << "Line Number : " << lineNumber << std::endl;
				lineNumber += 24; 

		}

		if (lineNumber > 6000) {
			isAnimate = false;
			lineNumber = 0;
		}

#pragma endregion

		Model::Blend();
		Model::Load(Model::Result);
		Model::Display(camera, phongShader,model);

		if (isAnimate) { Sleep(50); }
		
		Manipulator::DrawPickers(phongShader);

#pragma region ImGui Window
		//ImGui
		ImGui::Begin("Inspector");

		if (ImGui::CollapsingHeader("BlendFace")) {
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::SeparatorText("Transform");
			ImGui::DragFloat3("Position", glm::value_ptr(finalPosition), 1, -1000.0f, 1000.0f);
			ImGui::DragFloat3("Scale", glm::value_ptr(finalScale), 0.1, -1000.f, 1000.f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(finalRotation), 1, -1000.f, 1000.f);

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::SeparatorText("Material");
			ImGui::ColorEdit3("Base Color", (float*)&finalMeshColor);

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::SeparatorText("BlendShape");

			for (int i = 0; i < Model::weights.size(); i++) {
				ImGui::SliderFloat(label[i+1], &Model::weights[i], 0.0, 1.0);
			}
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("Animation");
			ImGui::SameLine();
			if (isAnimate) {
				ImGui::ColorButton("AnimationID", ImVec4(0.0, 1.0, 0.0, 1.0));
			}
			else {
				ImGui::ColorButton("AnimationID", ImVec4(1.0, 0.0, 0.0, 1.0));
			}

			if (ImGui::Button("Animate")) {
				isAnimate = !isAnimate;
			}
			
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
		}


		if (ImGui::CollapsingHeader("Light")) {
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SeparatorText("Basic");
			ImGui::DragFloat3("Light Position", glm::value_ptr(LightPosition), 1, -1000.0f, 1000.0f);
			ImGui::ColorEdit3("Color", (float*)&LightColor);
			ImGui::DragFloat("Power", &LightPower, 0.001, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::DragFloat("Ambient Light", &AmbientPower, 0.001, 0.0f, 1.0f);
			ImGui::DragFloat("Diffuse Light", &DiffusePower, 0.001, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::Spacing();
		}

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void MouseButtonHandler(GLFWwindow* window, int button, int action, int mods) {
		
	if (Manipulator::Pickers.size() > 0) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			Manipulator::MoveStart(mouse.x, mouse.y);
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			Manipulator::MoveComplete(mouse.x, mouse.y);
			Model::DirectManipulation();
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		Manipulator::ManipulatorCreator(mouse.x, mouse.y);
	}
}


void WindowResizingHandler(GLFWwindow* window, int width, int height)
{
	width = width;
	height = height;
	glViewport(0, 0, width, height);
}

void FramebufferHandler(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MouseHandler(GLFWwindow* window, double xPos, double yPos) {
	int x = static_cast<int>(xPos);
	int y = static_cast<int>(yPos);

	mouse.x = x;
	mouse.y = y;
}

void processInput(GLFWwindow* window)
{
	float cameraSpeed = 0.01f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.position += cameraSpeed * camera.front;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.position-= cameraSpeed  * camera.front;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.position -= glm::normalize(cross(camera.front, camera.up)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.position += glm::normalize(cross(camera.front, camera.up)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.position += cameraSpeed * camera.up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.position -= cameraSpeed * camera.up;
	}

	camera.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
}


glm::vec3 ScreenToWorld(int mouseX, int mouseY) {
	float x = (2.0f * mouseX) / WINDOW_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / WINDOW_HEIGHT;
	glm::vec4 rayClip = glm::vec4(x, y, -1.0, 1.0);

	glm::vec4 rayEye = glm::inverse(camera.projection) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

	glm::vec3 rayWorld = glm::vec3(glm::inverse(camera.view) * rayEye);
	rayWorld = glm::normalize(rayWorld);
	return rayWorld;
}
float DistancePointToRay(glm::vec3 point, glm::vec3 rayOrigin, glm::vec3 rayDirection) {
	glm::vec3 p1 = point;
	glm::vec3 p2 = rayOrigin;
	glm::vec3 p3 = rayOrigin + rayDirection;

	float dist = glm::length(glm::cross(p1 - p2, p1 - p3)) / glm::length(p3 - p2);
	return dist;
}

void Manipulator::MoveStart(int mouseX,int mouseY) {
	
	glm::vec3 ray = ScreenToWorld(mouseX, mouseY);
	float closestDistance = (std::numeric_limits<float>::max)();
	GLuint ClosestPickerIndex = -1;

	for (int i = 0; i < Manipulator::Pickers.size(); i++) {
		float distance = DistancePointToRay(Manipulator::Pickers[i].position, camera.position,ray);
		if (distance < closestDistance) {
			closestDistance = distance;
			ClosestPickerIndex = i;
		}
	}

	glm::vec3 picker = Manipulator::Pickers[ClosestPickerIndex].position;
	Manipulator::CurrentPicker = ClosestPickerIndex;
	
	int m0_resize = (Model::constraints_m0.size() * 3);
	int m0_x = 3 * Manipulator::CurrentPicker;
	int m0_y = 3 * Manipulator::CurrentPicker + 1;
	int m0_z = 3 * Manipulator::CurrentPicker + 2;

	Model::m0.conservativeResize(m0_resize, 1);

	Model::m0(m0_x, 0) = picker.x;
	Model::m0(m0_y, 0) = picker.y;
	Model::m0(m0_z, 0) = picker.z;
}

void Manipulator::MoveComplete(int mouseX, int mouseY) {
	glm::vec3 ray = ScreenToWorld(mouseX, mouseY);

	int m_resize = (Model::constraints_m.size() * 3);
	int m_x = 3 * Manipulator::CurrentPicker;
	int m_y = 3 * Manipulator::CurrentPicker + 1;
	int m_z = 3 * Manipulator::CurrentPicker + 2;

	Model::m.conservativeResize(m_resize, 1);
	Model::B_Bar.conservativeResize(Model::m.rows(), Model::B_Delta[0].size());

	float closestDistance = (std::numeric_limits<float>::max)();
	GLuint ClosestMeshIndex = -1;
	GLuint ClosestVertexIndex = -1;

	for (int i = 1; i < Model::BlendShapes.size(); i++) {
		for (int j = 0; j < Model::Result.vertices.size(); j++) {
			float distance = DistancePointToRay(Model::BlendShapes[i].vertices[j], camera.position, ray);
			if (distance < closestDistance) {
				closestDistance = distance;
				ClosestMeshIndex = i;
				ClosestVertexIndex = j;
			}
		}
	}

	glm::vec3 ClosestVertex = Model::BlendShapes[ClosestMeshIndex].vertices[ClosestVertexIndex];
	Manipulator::Pickers[Manipulator::CurrentPicker].position = ClosestVertex;
	Model::m.conservativeResize(m_resize, 1);

	Model::m(m_x, 0) = ClosestVertex.x;
	Model::m(m_y, 0) = ClosestVertex.y;
	Model::m(m_z, 0) = ClosestVertex.z;
	Model::B_Bar.conservativeResize(Model::m.rows(), Model::B_Delta[0].size());

	for (int i = 0; i < Model::B_Delta[0].size(); i++) {
		Model::B_Bar(m_x, i) = Model::B_Delta[ClosestVertexIndex * 3][i];
		Model::B_Bar(m_y, i) = Model::B_Delta[ClosestVertexIndex * 3 + 1][i];
		Model::B_Bar(m_z, i) = Model::B_Delta[ClosestVertexIndex * 3 + 2][i];
	}
}

void Manipulator::ManipulatorCreator(int mouseX, int mouseY) {
	glm::vec3 ray = ScreenToWorld(mouseX, mouseY);
	float closestDistance = (std::numeric_limits<float>::max)();

	GLuint ClosestMeshIndex = -1;
	GLuint ClosestVertexIndex = -1;

	for (int i = 1; i < Model::BlendShapes.size(); i++) {
		for (int j = 0; j < Model::Result.vertices.size(); j++) {
			float distance = DistancePointToRay(Model::BlendShapes[i].vertices[j], camera.position, ray);
			if (distance < closestDistance) {
				closestDistance = distance;
				ClosestMeshIndex = i;
				ClosestVertexIndex = j;
			}
		}
	}

	glm::vec3 ClosestVertex = Model::BlendShapes[ClosestMeshIndex].vertices[ClosestVertexIndex];
	Manipulator::Pickers.push_back(Manipulator(ClosestVertex, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ClosestVertexIndex));
	Model::constraints_m0.push_back(ClosestVertexIndex);
	Model::constraints_m.push_back(ClosestVertexIndex);
}

void Model::DirectManipulation() {

	float alpha = 0.1f;
	float u = 0.001f;
	
	std::vector<float> PreviousWeights = MultiplyScalar(weights,alpha);

	const int size = Model::BlendShapes.size() - 1;

	Eigen::MatrixXf Identity(Model::BlendShapes.size()-1, Model::BlendShapes.size() - 1);

	for (int i = 0; i < Model::BlendShapes.size() - 1; i++) {
		for (int j = 0; j < Model::BlendShapes.size() - 1; j++) {
			if (i == j) {
				Identity(i, j) = 1.0f;
			}
			else {
				Identity(i, j) = 0.0f;
			}
		}
	}

	Eigen::MatrixXf B_Transpose = B_Bar.transpose();
	Eigen::MatrixXf BtB = B_Transpose * B_Bar;
	Eigen::MatrixXf Left_Side = (BtB + (Identity * (alpha + u)));

	Eigen::VectorXf M_M0 = m - m0;
	Eigen::MatrixXf Right_Side = (B_Transpose * M_M0);

	for (int i = 0; i < Right_Side.rows(); i++) {
		Right_Side(i, 0) = Right_Side(i, 0) + PreviousWeights[i];
	}

	Eigen::LDLT<Eigen::MatrixXf> solver(Left_Side);
	Eigen::VectorXf NewWeights = solver.solve(Right_Side);


	for (int i = 0; i < 24; i++) {

		if (NewWeights(i, 0) > 1) {
			NewWeights(i, 0) = 1;
		}
		if (NewWeights(i, 0) < 0) {
			NewWeights(i, 0) = 0;

		}

	}

	std::vector<float> newWeight;
	for (int i = 0; i < NewWeights.rows(); i++) {
		newWeight.push_back(NewWeights(i, 0));
	}

	Model::weights = newWeight;
}

std::vector<float> MultiplyScalar(std::vector<float> vec, float num) {
	std::vector<float> result;

	for (int i = 0; i < vec.size(); i++) {
		result.push_back(vec[i] * num);
	}

	return result;
}