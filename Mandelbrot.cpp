#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// as usual for graphics canvas
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Canvas.h"
// contants
constexpr int width{ 800 }, height{ 800 };
constexpr float movement_speed{ 1.0f };
// global variables
glm::vec2 center{ -1.0f, 0.0f };
float zoom{ 0.25f };
float sec_per_frame{ 1.0f };
int max_iteration = 40;
// vertices for our canvas
constexpr GLfloat vertices[] = {
	 1.0f,  1.0f, 0.0f,  // top right
	-1.0f,  1.0f, 0.0f,  // top left
	-1.0f, -1.0f, 0.0f,  // bottom left
	 1.0f, -1.0f, 0.0f,  // bottom right
};
// indices
constexpr GLuint indices[] = {
	0, 1, 3,
	1, 2, 3
};

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
// declaration
static void theExplorer(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4); // anti-aliasing
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(width, height, "Mandelbrot Fractal", NULL, NULL);

	if (!window) 
	{	// no way!
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetKeyCallback(window, theExplorer);
	glfwSwapInterval(1);
	// load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// build and compile our shader program
	Shader shader("./Shaders/vertex.gls", "./Shaders/fragment.gls");
	shader.setVec2("viewportDimensions", width, height);
	// set up vertex data (and buffer(s)) and configure vertex attributes
	GLuint VBO, VAO, EBO; // vertex buffer object, vertex array object, element buffer objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer( // set the vertex attributes pointers
		0,                 // attribute. Must match the layout in the shader (location = 0).
		3,                 // size
		GL_FLOAT,          // type
		GL_FALSE,          // normalized?
		0,                 // stride
		(void*)0           // array buffer offset
	);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// classic viewport
	glViewport(0, 0, width, height);

	double prev_time_value{ glfwGetTime() };
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		double time_value{ glfwGetTime() };
		sec_per_frame = time_value - prev_time_value;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//////////////////////////////////////
		shader.drawCanvas();
		// https://github.com/donqustix/MandelbrotGL
		shader.setVec2("viewportDimensions", width, height);
		shader.setVec2("center", center);
		shader.setFloat("zoom", zoom);
		shader.setInt("max_iteration", max_iteration);
		//////////////////////////////////////
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		prev_time_value = time_value;
	}
	// clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	shader.deleteProgram();
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
// control keys
static void theExplorer(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_UP:
			center += glm::vec2(0, movement_speed / zoom * sec_per_frame);
			break;
		case GLFW_KEY_DOWN:
			center -= glm::vec2(0, movement_speed / zoom * sec_per_frame);
			break;
		case GLFW_KEY_RIGHT:
			center += glm::vec2(movement_speed / zoom * sec_per_frame, 0);
			break;
		case GLFW_KEY_LEFT:
			center -= glm::vec2(movement_speed / zoom * sec_per_frame, 0);
			break;
		case GLFW_KEY_LEFT_ALT:
			zoom += movement_speed * zoom * sec_per_frame;
			if (zoom > 10000) zoom = 10000; // set a zoom value max
			break;
		case GLFW_KEY_LEFT_CONTROL:
			zoom -= movement_speed * zoom * sec_per_frame;
			if (zoom < 0.25) zoom = 0.25; // set a zoom value min
			break;
		case GLFW_KEY_PAGE_UP:
			++max_iteration; // set a max_iteration value max
			if (max_iteration > 1000) max_iteration = 1000;
			break;
		case GLFW_KEY_PAGE_DOWN:
			--max_iteration; // set a max_iteration value min
			if (max_iteration < 5) max_iteration = 5;
			break;
		default:
			break;
		}
	}
}