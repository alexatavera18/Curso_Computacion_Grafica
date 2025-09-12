//Nombre: Alexa Fernanda López Tavera
//Practica 5: Modelado Jerarquico
//Fecha de entrega: 12 de Septiembre,2025
//No. de cuenta:319023024
//Terminar la mano robotica y agregar restricciones

#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);


const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

//For model
float hombro = 0.0f, 
codo = 0.0f, 
muneca = 0.0f, 
dedo1 = 0.0f, 
dedo2 = 0.0f,
dedo3 = 0.0;

float dedo2A = 0.0f, 
dedo2B = 0.0f, 
dedo2C = 0.0f, 
dedo3A = 0.0f, 
dedo3B = 0.0f, 
dedo3C = 0.0f, 
dedo4A = 0.0f, 
dedo4B = 0.0f, 
dedo4C = 0.0f,
dedo5A = 0.0f,
dedo5B = 0.0f;
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica5_Modelado jerarquico_Alexa Lopez", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection


	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, -0.5f,-0.5f,
		 0.5f, -0.5f,-0.5f,
		 0.5f,  0.5f,-0.5f,
		 0.5f,  0.5f,-0.5f,
		-0.5f,  0.5f,-0.5f,
		-0.5f, -0.5f,-0.5f,

		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);



	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	glm::mat4 projection = glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp3 = glm::mat4(1.0f); //Temp


		//View set up 
		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		glBindVertexArray(VAO);

		//Model Bicep
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A

		//Model Antebrazo
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//B

		//Model Palma
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0, 0.0f));
		modelTemp2 = modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//C

		//Model Dedo1A 
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.35f, 0.42f));
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // D

		//Model Dedo1B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo2), glm::vec3(0.0f, 0.0, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // E

		//Model Dedo1C
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo3), glm::vec3(0.0f, 0.0, 1.0f));
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // E

		//Model Dedo2A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.15f));
		model = glm::rotate(model, glm::radians(dedo2A), glm::vec3(0.0f, 0.0, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo2B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo2B), glm::vec3(0.0f, 0.0, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo2C
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo2C), glm::vec3(0.0f, 0.0, 1.0f));
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo3A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.13f));
		model = glm::rotate(model, glm::radians(dedo3A), glm::vec3(0.0f, 0.0, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo3B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo3B), glm::vec3(0.0f, 0.0, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo3C
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo3C), glm::vec3(0.0f, 0.0, 1.0f));
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo4A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.450f));
		model = glm::rotate(model, glm::radians(dedo4A), glm::vec3(0.0f, 0.0, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo4B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo4B), glm::vec3(0.0f, 0.0, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo4C
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo4C), glm::vec3(0.0f, 0.0, 1.0f));
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo5A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, -0.36f, 0.30f));
		model = glm::rotate(model, glm::radians(dedo5A), glm::vec3(0.0f, 1.0, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo5B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo5B), glm::vec3(0.0f, 1.0, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.330f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.27f, 0.15f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		movY += 0.005f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		movY -= 0.005f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		hombro += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		hombro -= 0.18f;

	float angSpeed = 0.2f;  // Velocidad de rotaci�n

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		codo = glm::clamp(codo + angSpeed, -90.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		codo = glm::clamp(codo - angSpeed, -90.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		muneca = glm::clamp(muneca + angSpeed, -45.0f, 45.0f);
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		muneca = glm::clamp(muneca - angSpeed, -45.0f, 45.0f);

	// Dedo 1 (Dedo1 A)
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dedo1 = glm::clamp(dedo1 + angSpeed, -10.0f, 80.0f);
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		dedo1 = glm::clamp(dedo1 - angSpeed, -10.0f, 80.0f);

	// Dedo 2 (Dedo1 B)
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dedo2 = glm::clamp(dedo2 + angSpeed, -90.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dedo2 = glm::clamp(dedo2 - angSpeed, -90.0f, 0.0f);

	// Dedo 3 (Dedo1 C)
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		dedo3 = glm::clamp(dedo3 + angSpeed, -75.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		dedo3 = glm::clamp(dedo3 - angSpeed, -75.0f, 0.0f);

	// Dedo 2
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dedo2A = glm::clamp(dedo2A + angSpeed, -10.0f, 80.0f);
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		dedo2A = glm::clamp(dedo2A - angSpeed, -10.0f, 80.0f);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dedo2B = glm::clamp(dedo2B + angSpeed, -90.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dedo2B = glm::clamp(dedo2B - angSpeed, -90.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		dedo2C = glm::clamp(dedo2C + angSpeed, -75.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		dedo2C = glm::clamp(dedo2C - angSpeed, -75.0f, 0.0f);

	// Dedo 3
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dedo3A = glm::clamp(dedo3A + angSpeed, -10.0f, 80.0f);
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		dedo3A = glm::clamp(dedo3A - angSpeed, -10.0f, 80.0f);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dedo3B = glm::clamp(dedo3B + angSpeed, -90.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dedo3B = glm::clamp(dedo3B - angSpeed, -90.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		dedo3C = glm::clamp(dedo3C + angSpeed, -75.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		dedo3C = glm::clamp(dedo3C - angSpeed, -75.0f, 0.0f);

	// Dedo 4
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dedo4A = glm::clamp(dedo4A + angSpeed, -10.0f, 80.0f);
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		dedo4A = glm::clamp(dedo4A - angSpeed, -10.0f, 80.0f);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dedo4B = glm::clamp(dedo4B + angSpeed, -90.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dedo4B = glm::clamp(dedo4B - angSpeed, -90.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		dedo4C = glm::clamp(dedo4C + angSpeed, -75.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		dedo4C = glm::clamp(dedo4C - angSpeed, -75.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		dedo5A = glm::clamp(dedo5A + angSpeed, -15.0f, 60.0f);
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dedo5A = glm::clamp(dedo5A - angSpeed, -70.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		dedo5B = glm::clamp(dedo5B + angSpeed, -15.0f, 90.0f);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dedo5B = glm::clamp(dedo5B - angSpeed, -45.0f, 0.0f);

}