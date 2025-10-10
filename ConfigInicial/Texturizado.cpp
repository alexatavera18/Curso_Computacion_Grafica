/*Nombre : Alexa Fernanda López Tavera
* Practica 7: Texturizado
* Realizar un dado con la textura (En una sola textura todas las caras del dado)
* Fecha de entrega: 10 de Octubre,2025
* No. de cuenta:319023024*/

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 7_Texturizado_Alexa_Lopez", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Mouse look (como antes)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW\n";
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Shaders
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // ===========================
    // Geometría: CUBO con UVs
    // Formato: pos(3), color(3), uv(2) => stride = 8
    // Atlas 3x2 (u: 0,1/3,2/3,1 ; v: 0,0.5,1)
    // Orden imagen: Arriba [6|5|4], Abajo [3|2|1]
    // Caras: FRONT=1, BACK=6, LEFT=3, RIGHT=4, TOP=5, BOTTOM=2
    // ===========================
    const float U0 = 0.0f, U1 = 1.0f / 3.0f, U2 = 2.0f / 3.0f, U3 = 1.0f;
    const float V0 = 0.0f, V1 = 0.5f, V2 = 1.0f;

    GLfloat vertices[] =
    {
        // FRONT (1) -> (2,0)
        -0.5f,-0.5f, 0.5f,  1,1,1,  U2, V0,
         0.5f,-0.5f, 0.5f,  1,1,1,  U3, V0,
         0.5f, 0.5f, 0.5f,  1,1,1,  U3, V1,
        -0.5f, 0.5f, 0.5f,  1,1,1,  U2, V1,

        // BACK (6) -> (0,1)  (u invertida)
        -0.5f,-0.5f,-0.5f,  1,1,1,  U1, V1,
         0.5f,-0.5f,-0.5f,  1,1,1,  U0, V1,
         0.5f, 0.5f,-0.5f,  1,1,1,  U0, V2,
        -0.5f, 0.5f,-0.5f,  1,1,1,  U1, V2,

        // LEFT (3) -> (0,0)
        -0.5f,-0.5f,-0.5f,  1,1,1,  U0, V0,
        -0.5f,-0.5f, 0.5f,  1,1,1,  U1, V0,
        -0.5f, 0.5f, 0.5f,  1,1,1,  U1, V1,
        -0.5f, 0.5f,-0.5f,  1,1,1,  U0, V1,

        // RIGHT (4) -> (2,1)
         0.5f,-0.5f, 0.5f,  1,1,1,  U2, V1,
         0.5f,-0.5f,-0.5f,  1,1,1,  U3, V1,
         0.5f, 0.5f,-0.5f,  1,1,1,  U3, V2,
         0.5f, 0.5f, 0.5f,  1,1,1,  U2, V2,

         // TOP (5) -> (1,1)
         -0.5f, 0.5f, 0.5f,  1,1,1,  U1, V1,
          0.5f, 0.5f, 0.5f,  1,1,1,  U2, V1,
          0.5f, 0.5f,-0.5f,  1,1,1,  U2, V2,
         -0.5f, 0.5f,-0.5f,  1,1,1,  U1, V2,

         // BOTTOM (2) -> (1,0)
         -0.5f,-0.5f,-0.5f,  1,1,1,  U1, V0,
          0.5f,-0.5f,-0.5f,  1,1,1,  U2, V0,
          0.5f,-0.5f, 0.5f,  1,1,1,  U2, V1,
         -0.5f,-0.5f, 0.5f,  1,1,1,  U1, V1,
    };

    GLuint indices[] =
    {
        0,1,2,  0,2,3,      // FRONT
        4,5,6,  4,6,7,      // BACK
        8,9,10, 8,10,11,    // LEFT
        12,13,14, 12,14,15, // RIGHT
        16,17,18, 16,18,19, // TOP
        20,21,22, 20,22,23  // BOTTOM
    };

    // Buffers
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Atributos (stride = 8 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Textura
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load("images/dado_text.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    if (image) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(image);

    // Sampler
    lampShader.Use();
    glUniform1i(glGetUniformLocation(lampShader.Program, "ourTexture"), 0);

    // Loop
    while (!glfwWindowShouldClose(window))
    {
        // Time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lampShader.Use();

        // Vista desde la cámara 
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Modelo ESTÁTICO (identidad)
        glm::mat4 model(1.0f); // <--- sin rotación

        // Uniforms
        GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Dibujo
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

// Camera WASD
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Key callback
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)      keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

//Ver en cámara con mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos; // y invertida

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
