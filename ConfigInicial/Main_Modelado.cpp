/*Práctica 4_Lopez Alexa
  Fecha: 5/09/2025
  Tema: Modelado geometrico
  Objetivo:Construir un leon estilo voxel
  Numero de cuenta: 319023024*/

#include<iostream>
#include <algorithm>
//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
//Manipular la vista directamente
float movX=0.0f;
float movY=0.0f;
float movZ=-5.0f;
float rot = 0.0f;
static void CrearVAOConVertices(const float* data, size_t numFloats, GLuint& outVAO, GLuint& outVBO) {
	glGenVertexArrays(1, &outVAO);
	glGenBuffers(1, &outVBO);

	glBindVertexArray(outVAO);
	glBindBuffer(GL_ARRAY_BUFFER, outVBO);
	glBufferData(GL_ARRAY_BUFFER, numFloats * sizeof(float), data, GL_STATIC_DRAW);

	// layout (location = 0) -> posición (x,y,z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// layout (location = 1) -> color (r,g,b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4_Modelado geometrico_Alexa Lopez", nullptr, nullptr);

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

	

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 0.647f, 0.0f,//Front
		0.5f, -0.5f, 0.5f,  1.0f, 0.647f, 0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.647f, 0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.647f, 0.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 0.647f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.647f, 0.0f,

		-0.5f, -0.5f,-0.5f, 1.0f, 0.647f, 0.0f,//Back
		 0.5f, -0.5f,-0.5f, 1.0f, 0.647f, 0.0f,
		 0.5f,  0.5f,-0.5f, 1.0f, 0.647f, 0.0f,
		 0.5f,  0.5f,-0.5f, 1.0f, 0.647f, 0.0f,
		-0.5f,  0.5f,-0.5f, 1.0f, 0.647f, 0.0f,
		-0.5f, -0.5f,-0.5f, 1.0f, 0.647f, 0.0f,

		 0.5f, -0.5f,  0.5f,  1.0f, 0.647f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.647f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.647f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.647f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.647f, 0.0f,
		 0.5f,  -0.5f, 0.5f,  1.0f, 0.647f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.647f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.647f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.647f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.647f, 0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 0.647f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.647f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 0.647f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.647f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.647f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.647f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.647f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.647f, 0.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 0.647f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.647f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.647f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.647f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.647f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.647f, 0.0f,
	};

	// Naranja fuerte (1.0, 0.478, 0.0)
	float verticesNaranjaFuerte[sizeof(vertices) / sizeof(vertices[0])];
	std::copy(std::begin(vertices), std::end(vertices), std::begin(verticesNaranjaFuerte));
	for (size_t i = 0; i < sizeof(verticesNaranjaFuerte) / sizeof(float); i += 6) {
		verticesNaranjaFuerte[i + 3] = 1.0f;   // R
		verticesNaranjaFuerte[i + 4] = 0.478f; // G 
		verticesNaranjaFuerte[i + 5] = 0.0f;   // B
	}

	// Negro (0,0,0)
	float verticesNegro[sizeof(vertices) / sizeof(vertices[0])];
	std::copy(std::begin(vertices), std::end(vertices), std::begin(verticesNegro));
	for (size_t i = 0; i < sizeof(verticesNegro) / sizeof(float); i += 6) {
		verticesNegro[i + 3] = 0.0f; // R
		verticesNegro[i + 4] = 0.0f; // G
		verticesNegro[i + 5] = 0.0f; // B
	}

	GLuint vaoBase = 0, vboBase = 0;
	GLuint vaoNaranjaFuerte = 0, vboNaranjaFuerte = 0;
	GLuint vaoNegro = 0, vboNegro = 0;
	CrearVAOConVertices(vertices, sizeof(vertices) / sizeof(float), vaoBase, vboBase);
	CrearVAOConVertices(verticesNaranjaFuerte, sizeof(verticesNaranjaFuerte) / sizeof(float), vaoNaranjaFuerte, vboNaranjaFuerte);
	CrearVAOConVertices(verticesNegro, sizeof(verticesNegro) / sizeof(float), vaoNegro, vboNegro);

	/*glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); */// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);


		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		//glBindVertexArray(VAO);
		/*
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/
		glBindVertexArray(vaoBase); //Naranja normal para el cuerpo del leon
		//Cara del león
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 2.0f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Trompa del león
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.5f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Tronco del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 5.0f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Primer pierna del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(1.5f, -0.2f, 4.5f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Primer pata del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.8f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(1.5f, -5.0f, 3.0f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Segunda pierna del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(-1.5f, -0.2f, 4.5f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Segunda pata del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.8f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(-1.5f, -5.0f, 3.0f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Tercera pierna del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(-1.5f, -0.2f, -4.5f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Tercera pata del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.8f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(-1.5f, -5.0f, -2.62f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Cuarta pierna del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(1.5f, -0.2f, -4.5f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuarta pata del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.8f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(1.5f, -5.0f, -2.62f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Cola del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.9f));
		model = glm::translate(model, glm::vec3(0.0f, 4.45f, -2.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.9f, 0.4f));
		model = glm::translate(model, glm::vec3(0.0f, 1.7f, -7.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.9f));
		model = glm::translate(model, glm::vec3(0.0f, 2.3f, -3.48f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 1.2f, 0.4f));
		model = glm::translate(model, glm::vec3(0.0f, 1.1f, -9.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.9f));
		model = glm::translate(model, glm::vec3(0.0f, 4.4f, -4.37f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		glBindVertexArray(vaoNaranjaFuerte);//Naranja fuerte para la melena del leon 

		
		//Melena del leon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.5f, 3.3f, 0.3f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, 8.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.9f, 2.5f, 0.3f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, 8.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 2.2f, 0.5f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.5f, 0.9f, 4.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); 

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 2.2f, 0.5f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(-0.5f, 0.9f, 4.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.8f, 1.0f, 0.5f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 4.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Nariz
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.4f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 4.5f, 9.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cola del leon final
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.9f, 0.3f));//Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -14.37f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.5f));
		model = glm::translate(model, glm::vec3(0.0f, 4.4f, -9.37f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(vaoNegro);
		//Ojos
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(-2.0f, 11.5f, 14.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(2.0f, 11.5f, 14.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		/*
		//Primer pata de la mesa
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(14.4f, -0.4f, 14.5f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Segunda pata de la mesa
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(-14.4f, -0.4f, 14.5f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Tercera pata de la mesa
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(-14.4f, -0.4f, -14.5f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuarta pata de la mesa
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));//Tamaño de la pata
		model = glm::translate(model, glm::vec3(14.4f, -0.4f, -14.5f));//Posición de la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/
		
		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}

	//glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &vaoBase);
	glDeleteBuffers(1, &vboBase);
	glDeleteVertexArrays(1, &vaoNaranjaFuerte);
	glDeleteBuffers(1, &vboNaranjaFuerte);
	glDeleteVertexArrays(1, &vaoNegro);
	glDeleteBuffers(1, &vboNegro);


	glfwTerminate();
	return EXIT_SUCCESS;
 }
 //Recordar que se mueve la vista
 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += 0.0008f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= 0.0008f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.04f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.04f;
 }


