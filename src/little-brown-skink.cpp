/**
 * File: little-brown-skink.cpp
 * Description: main file to launch the little brown skink renderer
*/



/**
 * Dependencies:
 * glfw version 3.3
 * glad 64-bit binaries
 * x86-mingw64 for g++ compiling
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


// if glfw gets an error, this function is run to print the error
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// keypress callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// close the window if the escape key is pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char* argv[]) 
{
	// attempt to initialize glfw
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}
	// set the error callback function as above
	glfwSetErrorCallback(error_callback);


	// set minimum OpenGL version and profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// create a combined OpenGL window and context
	GLFWwindow* window = glfwCreateWindow(800, 600, "Little Brown Skink Renderer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "Window or OpenGL context creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	// before the OpenGL API can be used, a current OpenGL context must be acquired
	glfwMakeContextCurrent(window);
	// set the key callback function for the main window
	glfwSetKeyCallback(window, key_callback);
	// set minimum buffer swap interval
	glfwSwapInterval(1);
	// set window icon
	int width, height, channels;
	unsigned char* pixels = stbi_load("./src/lbs-icon.png", &width, &height, &channels, 4);
	GLFWimage images[1];
	images[0].width = width;
	images[0].height = height;
	images[0].pixels = pixels;
	glfwSetWindowIcon(window, 1, images);


	// attempt to load OpenGL with GLAD
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}


	// setup a full screen quad to display our renders on
	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
	};
	// VertexBufferObject
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	

	double t0 = glfwGetTime();
	double t, tau;
	double dtau = 0.001; // time-step for physics in seconds
	// main loop while the window is open
	while (!glfwWindowShouldClose(window))
	{
		// get current time
		t = glfwGetTime() - t0;

		// // catch physics up to current time
		// while (tau < t)
		// {
		// 	prev_state = state;
		// 	integrate(state, tau, dtau);
		// }

		// // render the current state
		// render(state);

		// swap buffer being displayed
		glfwSwapBuffers(window);
		// look for events
		glfwPollEvents();
	}


	// OpenGL cleanup
	glfwDestroyWindow(window);

	// close glfw
	glfwTerminate();
	return 0;
}