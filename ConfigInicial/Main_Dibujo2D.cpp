//Nombre: Alexa Fernanda López Tavera
//Practica 2: Dibujo de primitivas 2D
//Fecha de entrega: 22 de Agosto,2025
//No. de cuenta:319023024

#include<iostream>
//#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 900, HEIGHT = 900;


int main() {
    glfwInit();
    //Verificaci n de compatibilidad 
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2, Alexa Lopez", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, resize);

    //Verificaci n de errores de creacion  ventana
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    //Verificaci n de errores de inicializaci n de glew

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Imprimimos informacin de OpenGL del sistema
    std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Define las dimensiones del viewport
    //glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Set up vertex data (and buffer(s)) and attribute pointers
    float vertices[] = {
        //Pico
        0.0f,  0.0f, 0.0f,      0.9922f,0.6588f,0.2941f,  // 0
        -0.05f, 0.00f, 0.0f,    0.9922f,0.6588f,0.2941f,  // 1
        -0.05f, 0.05f, 0.0f,    0.9922f,0.6588f,0.2941f,  // 2
        0.0f,  0.05f, 0.0f,     0.9922f,0.6588f,0.2941f, // 3 
        //Ojo derecho
        0.0f, 0.05f, 0.0f,      0.0f,0.0f,0.0f, //4
        0.0f,0.1f,0.0f,			0.0f,0.0f,0.0f, //5
        0.05f,0.1f,0.0f,		0.0f,0.0f,0.0f, //6
        0.05f,0.05f,0.0f,		0.0f,0.0f,0.0f, //7
        //Ojo izquierdo
        -0.05f, 0.05f, 0.0f,    0.0f,0.0f,0.0f,  // 8
        -0.1f, 0.05f, 0.0f,     0.0f,0.0f,0.0f,  // 9
        -0.1f, 0.1f, 0.0f,      0.0f,0.0f,0.0f,  // 10
        -0.05f, 0.1f, 0.0f,     0.0f,0.0f,0.0f,  // 11
        //Cabeza 
        0.1f, 0.15f, 0.0f,      0.0f,0.0f,0.0f, //12
        0.1f,0.2f,0.0f,			0.0f,0.0f,0.0f, //13
        -0.15f,0.15f,0.0f,		0.0f,0.0f,0.0f, //14
        -0.15f,0.2f,0.0f,		    0.0f,0.0f,0.0f, //15
        //Brazo izq
        -0.2f,0.15f,.0f,         0.0f,0.0f,0.0f,//16
        -0.2f,-0.15f,.0f,        0.0f,0.0f,0.0f,//17
        -0.15f,-0.15f,.0f,       0.0f,0.0f,0.0f,//18
        //Brazo derecho
        0.15f,0.15f,.0f,        0.0f,0.0f,0.0f,//19
        0.15f,-0.15f,.0f,       0.0f,0.0f,0.0f,//20
        0.1f,-0.15f,.0f,        0.0f,0.0f,0.0f,//21
        //Mano izq
        -0.25f,0.0f,.0f,         0.0f,0.0f,0.0f,//22
        -0.2f,0.0f,.0f,          0.0f,0.0f,0.0f,//23
        -0.25f,-0.05f,.0f,       0.0f,0.0f,0.0f,//24
        -0.2f,-0.05f,.0f,        0.0f,0.0f,0.0f,//25
        //Mano derecha
        0.15f,0.0f,.0f,         0.0f,0.0f,0.0f,//26
        0.2f,0.0f,.0f,          0.0f,0.0f,0.0f,//27
        0.2f,-0.05f,.0f,        0.0f,0.0f,0.0f,//28
        0.15f,-0.05f,.0f,       0.0f,0.0f,0.0f,//29
        //Cuadro abajo negro 1
        -0.1f,-0.15f, .0f,      0.0f, 0.0f, 0.0f,//30
        -0.1f,-0.2f,.0f,        0.0f,0.0f,0.0f,//31
        -0.15f,-0.2f,.0f,        0.0f,0.0f,0.0f,//32
        //Cuadro abajo negro 2
        -0.05f,-0.15f, .0f,      0.0f, 0.0f, 0.0f,//33
         0.0f,-0.15f,.0f,        0.0f,0.0f,0.0f,//34
         0.0f,-0.2f,.0f,        0.0f,0.0f,0.0f,//35
        -0.05f,-0.2f,.0f,        0.0f,0.0f,0.0f,//36
        //Cuadro abajo negro 2
         0.05f,-0.15f, .0f,      0.0f, 0.0f, 0.0f,//37
         0.1f,-0.2f,.0f,        0.0f,0.0f,0.0f,//38
         0.05f,-0.2f,.0f,        0.0f,0.0f,0.0f,//39
         //Patita izq
          -0.1f,-0.15f, .0f,      0.9922f,0.6588f,0.2941f,//30 naranja (40)
          -0.1f,-0.2f,.0f,        0.9922f,0.6588f,0.2941f,//31 naranja (41)
          -0.05f,-0.15f, .0f,     0.9922f,0.6588f,0.2941f,//33 naranja (42)
          -0.05f,-0.2f,.0f,       0.9922f,0.6588f,0.2941f,//36 naranja (43)

          //Patita derecha
          0.0f,-0.15f,.0f,        0.9922f,0.6588f,0.2941f,//34 naranja (44)
          0.0f,-0.2f,.0f,        0.9922f,0.6588f,0.2941f,//35 naranja (45)
          0.05f,-0.15f, .0f,      0.9922f,0.6588f,0.2941f,//37 naranja (46)
          0.05f,-0.2f,.0f,        0.9922f,0.6588f,0.2941f,//39 naranja (47)

          0.15f,-0.05f,.0f,      0.0f,0.0f,0.0f,//48

    };
    unsigned int indices[] = {  // note that we start from 0!
        0,1,2,// second Triangle
        0,2,3,
        4,5,6,
        7,4,6,
        8,9,11,
        9,10,11,
        12,13,14,
        13,14,15,
        14,16,18,
        16,17,18,
        12,19,20,
        12,20,21,
        23,24,25,
        22,23,24,
        26,27,28,
        26,48,28,
        18,32,31,
        18,30,31,
        33,36,35,
        33,34,35,
        21,37,39,
        21,38,39,
        40,41,42,
        43,42,41,
        44,45,46,
        45,46,47,
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Enlazar  Vertex Array Object
    glBindVertexArray(VAO);

    //2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. Despues colocamos las caracteristicas de los vertices

    //Posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Rojo, con opacidad total
        glClear(GL_COLOR_BUFFER_BIT);


        // Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);


        //glPointSize(10);
        //glDrawArrays(GL_POINTS,0,4);

        //glDrawArrays(GL_LINES,1,3);
        //glDrawArrays(GL_LINE_LOOP,0,4);
         //glDrawArrays(GL_TRIANGLES,1,3);
        glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_INT, 0);


        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
    //glViewport(0, 0, screenWidth, screenHeight);
}

