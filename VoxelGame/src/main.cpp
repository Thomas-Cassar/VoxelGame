#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <iostream>


#include "Renderer.h"
#include "VertexBufferLayout.h"

#include "Player.h"

#include "ChunkManager.h"



extern "C"
{//Use High Performance GPU
	__declspec(dllexport) bool AmdPowerXpressRequestHighPerformance = true;
	__declspec(dllexport) bool NvOptimusEnablement = true;
}


const int width = 1920;
const int height = 1080;


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
	glfwSwapInterval(0);


	//Test GLEW is working and output GL Version
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
{
	Renderer renderer;
	Shader shader("res/shaders/basic.vertex", "res/shaders/basic.fragment");
	//Lighting
	shader.Bind();
	shader.SetUniform3f("lightDirection", 0.2f, -1.0f, 0.3f);
	shader.SetUniform3f("lightColor", 1.0f, 1.0f, 0.98f);
	shader.SetUniform2f("lightBias", 0.5f, 0.6f);
	shader.Unbind();

	ChunkManager managerofChunks;

	Player Player1(width,height,window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Cull back faces
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));

	//Enable depth testing
	GLCall(glEnable(GL_DEPTH_TEST));

	float currentTime = 0;
	float lastTime = 0;
	float deltaTime = 0;

	managerofChunks.updateChunks(glm::i32vec3(0, 0, 0));
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Timing and FPS counter
		lastTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		if (floor(currentTime) - floor(lastTime) == 1)
		{
			std::cout << 1 / deltaTime << "fps" << std::endl;
			std::cout << Player1.getworldLocation().x<<","<< Player1.getworldLocation().y << ","<< Player1.getworldLocation().z << std::endl;
		}


		//Clear frame before starting next
		renderer.Clear();

		managerofChunks.updateChunks(Player1.getChunkLocation());
		managerofChunks.drawChunks(renderer,shader,*Player1.getPlayerCamera());

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
		//Get input from user
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		Player1.getPlayerCamera()->newMousepos((float)xpos, (float)ypos);
		Player1.updatePlayerInput(deltaTime);

		
		
	}

}
	glfwTerminate();

	return 0;
}