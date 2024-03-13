#include "imGui/imgui.h"
#include "imGui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"

#include "iostream"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assimp/Importer.hpp>

int width = 500;
int height = 500;

GLFWwindow* window;
void WindowResizingHandler(GLFWwindow* window, int width, int height);

int main(void) {
	//Window Creation
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(width, height, "Hello", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, WindowResizingHandler);

	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
	glfwSwapInterval(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.38, 0.38, 0.38, 1.0);

	WindowResizingHandler(window, width, height);


	//Setup IMGUI

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");



	double lastFrameTime = glfwGetTime();
	double currentTime;
	int frameCount = 0;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentTime = glfwGetTime();
		frameCount++;

		float deltaTime = currentTime - lastFrameTime;
		if (currentTime - lastFrameTime >= 1.0)
		{
			glfwSetWindowTitle(window, std::to_string(frameCount).c_str());
			frameCount = 0;
			lastFrameTime = currentTime;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Hello");
		ImGui::Text("I am Happy It Finally WORKS!");
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