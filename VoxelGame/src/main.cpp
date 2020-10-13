#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <iostream>


#include "Renderer.h"
#include "VertexBufferLayout.h"

#include "Camera.h"

/*
extern "C"
{//Use High Performance GPU
	__declspec(dllexport) bool AmdPowerXpressRequestHighPerformance = true;
	__declspec(dllexport) bool NvOptimusEnablement = true;
}
*/

const int width = 1280;
const int height = 720;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
static void mouse_callback(GLFWwindow* window, double xpos, double ypos){}

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
	window = glfwCreateWindow(width, height, "Voxel Test", NULL, NULL);
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
	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
{
	Renderer renderer;
	Shader shader("res/shaders/basic.vertex", "res/shaders/basic.fragment");

	float positions[] = {
			 0.0f, 0.0f, 0.0f,   //0
			 1.0f,  0.0f, 0.0f,   //1
			 1.0f,  1.0f, 0.0f,  //2
			 0.0f, 1.0f, 0.0f,	//3
			0.0f, 0.0f, -1.0f,   //4
			 1.0f,  0.0f, -1.0f,   //5
			 1.0f,  1.0f, -1.0f,  //6
			 0.0f, 1.0f, -1.0f,	//7

	};

	unsigned int indicies[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
	VertexArray va;
	IndexBuffer ib(indicies, 36);
	VertexBuffer vb(positions, 8 * 3 * sizeof(float));

	VertexBufferLayout vbl;

	vbl.Push<float>(3);
	va.AddBuffer(vb, vbl);
	

	Camera camera;
	
	glm::mat4 mvp;
	glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)width/(float)height, 0.1f, 1000.0f);
	glm::mat4 mod = glm::mat4(1.0f);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	float g = 0;
	bool up = true;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Clear frame before starting next
		glClearColor(0.5f, 0.75f, 0.95f, 1.0f);
		renderer.Clear();

		renderer.Draw(va, ib, shader);

		shader.SetUniform4f("u_Color", 0.4f, g, 0.8f, 1.0f);
		if (up)
			g += .005f;
		else
			g -= .005f;
		if (g >= .9f)
			up = false;
		if (g <= .1f)
			up = true;
		shader.SetUniformMat4f("u_MVP", mvp);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
		
		
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		camera.newMousepos((float)xpos, (float)ypos);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.moveForward();
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.moveBackward();
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.moveLeft();
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.moveRight();
		}

		mvp = proj * camera.GetViewMatrix() * mod;
	}


}
	glfwTerminate();

	return 0;
}