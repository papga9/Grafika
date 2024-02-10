#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "GameMaster.h"

//TODO:
//felcommentezni
//kukacok egymáson fedik egymást
//more than 2 player game
//menürendszer
//	start gomb
//	irányíthatóság kiiratni
//	player nevek beállítása
//	beállítások
//		kukac "ratio"
//		bomb "ratio"
//		bomba robbanás idő
//		kukac sebessége
//		bomba sebzés
//		maximum életpont
//		FailureChance
//játék közben
//	jobb meg bal felső sarokban élet csík kirajzolása
//	nyertes képernyő megjelenítése console helyett

// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 1080;
int keyEventCode = GLFW_KEY_F;
double xpos, ypos, rxpos, rypos;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (action == GLFW_REPEAT || action == GLFW_PRESS) {
		keyEventCode = key;
	}
	/* if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		 keyEventCode = GLFW_KEY_UP;
	 }
	 else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		 keyEventCode = GLFW_KEY_RIGHT;
	 }
	 else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		 keyEventCode = GLFW_KEY_DOWN;
	 }
	 else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		 keyEventCode = GLFW_KEY_LEFT;
	 }*/
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//getting cursor position, left button
		glfwGetCursorPos(window, &xpos, &ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		//getting cursor position, right button
		glfwGetCursorPos(window, &rxpos, &rypos);
		std::cout << "OI!" << std::endl;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

int main() {

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	GameMaster gamemaster(SCR_WIDTH, SCR_HEIGHT);

	// render loop
	// -----------
	double prev_x = 0, prev_y = 0, prev_rx = 0, prev_ry = 0;
	while (!glfwWindowShouldClose(window)) {
		// render
		// ------


		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		gamemaster.handleKeyEvent(keyEventCode);
		if (xpos != prev_x || ypos != prev_y) {
			gamemaster.handleMouseEvent(xpos / (SCR_WIDTH / 2) - 1, -ypos / (SCR_HEIGHT / 2) + 1, true);
		}

		else if (rxpos != prev_rx || rypos != prev_ry) {
			gamemaster.handleMouseEvent(rxpos / (SCR_WIDTH / 2) - 1, -rypos / (SCR_HEIGHT / 2) + 1, false);
			std::cout << "io I pressed here: " << rxpos / (SCR_WIDTH / 2) - 1 << " ; " << rypos / (SCR_WIDTH / 2) - 1 << std::endl;
		}
		gamemaster.draw();

		prev_x = xpos;
		prev_y = ypos;
		prev_rx = rxpos;
		prev_ry = rypos;
		keyEventCode = GLFW_KEY_F;


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

