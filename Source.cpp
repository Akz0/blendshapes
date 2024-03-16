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
	Shader basicShader("./src/shaders/basic.vert", "./src/shaders/basic.frag");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.38, 0.38, 0.38, 1.0);

	WindowResizingHandler(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Setup IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	Mesh mesh_Test("./src/test-model/Key 1.obj");
	Model::Load(mesh_Test, basicShader);

	glm::vec3 finalPosition(1.0f);
	glm::vec3 finalScale(1.0f);
	glm::vec3 finalRotation(0.0f);


	while (!glfwWindowShouldClose(window))
	{	
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

		processInput(window);

		mesh_Test.model = glm::translate(glm::mat4(1.0f), finalPosition);
		mesh_Test.model = glm::rotate(mesh_Test.model, glm::radians(10.f), finalRotation);
		Model::Display(camera, basicShader, mesh_Test.model);

		ImGui::Begin("Hello");

		ImGui::BeginGroup();
		ImGui::Text("BlendFace");
		ImGui::DragFloat3("Position", glm::value_ptr(finalPosition),1,-1000.0f,1000.0f);
		ImGui::DragFloat3("Scale", glm::value_ptr(finalScale),0.1,-1000.f,1000.f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(finalRotation),1,-1000.f,1000.f);
		ImGui::EndGroup();
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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


