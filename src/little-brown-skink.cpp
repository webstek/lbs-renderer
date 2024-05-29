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

// window resize callback function
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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


	// vertex shader
	const char *vertexShaderSource = "#version 410 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int shaderSuccess;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile vertex shader" << std::endl << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}


	// fragment shader
	const char *fragShaderSource = "#version 410 core \n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.2f, 0.2f, 0.9f, 1.0f);\n"
		"}\0";
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader" << std::endl << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}


	// shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader" << std::endl << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	
	// setup a full screen quad to display our renders on with a texture
	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};
	// VertexBufferObject and VertexArrayObject
	GLuint VAO, VBO, EBO;
	// gen vao and buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind vao
	glBindVertexArray(VAO);
	
	// bind vbo and set data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// bind ebo and set data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// enable vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind buffer and vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	

	// timing variables
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

		// display the rendering using OpenGL
        glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap buffer being displayed
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// OpenGL cleanup
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);

	// close glfw
	glfwTerminate();
	return 0;
}