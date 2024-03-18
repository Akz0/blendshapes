#include "imGui/imgui.h"
#include "imGui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"

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





double lastFrameTime = glfwGetTime();
double currentTime;
float deltaTime = currentTime - lastFrameTime;
int frameCount = 0;

GLFWwindow* window;
void WindowResizingHandler(GLFWwindow* window, int width, int height);
void FramebufferHandler(GLFWwindow* window, int width, int height);
void MouseButtonHandler(GLFWwindow* window, int button, int action, int mods);
void MouseHandler(GLFWwindow* window, double xposIn, double yposIn);
void processInput(GLFWwindow* window);

Camera camera;
Mouse mouse;

const char* label[] = {
	"NEUTRAL", "JAW_OPEN", "KISS", "L_BROW_LOWER", "L_BROW_NARROW",
	"L_BROW_RAISE", "LEFT_CLOSED", "L_LOWER_O", "L_UPPER_O",
	"L_NOSE_WRINKLE", "L_PUFF", "L_SAD", "L_SMILE", "L_SUCK",
	"R_BROW_LOWER", "R_BROW_NARROW", "R_BROW_RAISE", "REFT_CLOSED",
	"R_LOWER_O", "R_UPPER_O", "R_NOSE_WRINKLE", "R_PUFF", "R_SAD",
	"R_SMILE", "R_SUCK"
};

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
}
void LoadTestModels() {
	Mesh Key1("./src/test-model/Basis.obj");
	Mesh Key2("./src/test-model/Smooth Vertex.obj");
	Mesh Key3("./src/test-model/Circle.obj");
}
int main(void) {
	//Window Creation
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

#pragma region Inspector Controls
	glm::vec3 finalPosition(0.0,0.0,-60.0f);
	glm::vec3 finalScale(1.0f);
	glm::vec3 finalRotation(0.0f);
	static ImVec4 finalMeshColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); 

	//Light Information
	static ImVec4 LightColor = ImVec4(1.0, 1.0, 1.0, 1.0);
	glm::vec3 LightPosition(0.0, 3.0, -3.0);
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
		//Update Transforms
		glm::mat4 model = glm::translate(glm::mat4(1.0f), finalPosition);
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



void AnimateFace(GLFWwindow* window) {

	


}