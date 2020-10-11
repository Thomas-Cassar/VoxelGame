#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <iostream>


#include "Renderer.h"
#include "VertexBufferLayout.h"

extern "C"
{//Use High Performance GPU
	__declspec(dllexport) bool AmdPowerXpressRequestHighPerformance = true;
	__declspec(dllexport) bool NvOptimusEnablement = true;
}

int main()
{
	//Create pointer to a window
	GLFWwindow* window;

	//Start GLFW Library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window and ensure creation
	window = glfwCreateWindow(1280, 720, "Voxel Test", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	//Make window current
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);


	//Test GLEW is working and output GL Version
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;
	std::cout << glGetString(GL_VERSION)<<std::endl;


	Renderer renderer;
	Shader shader("res/shaders/basic.vertex", "res/shaders/basic.fragment");

	float positions[] = {
			-1.0f, -1.0f,   //0
			 1.0f,  -1.0f,  //1
			 0.0f, 1.0f,  //2
			 
	};

	unsigned int indicies[] = {
		0,1,2,
	};
	VertexArray va;
	IndexBuffer ib(indicies, 3);
	VertexBuffer vb(positions,2*3*sizeof(float));

	VertexBufferLayout vbl;

	vbl.Push<float>(2);
	va.AddBuffer(vb, vbl);	


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Clear frame before starting next
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderer.Clear();

		renderer.Draw(va,ib,shader);
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}