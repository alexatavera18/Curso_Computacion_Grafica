//Nombre: Alexa Fernanda López Tavera
//Practica 9: Fuentes de Luz minimo 3 diferentes fuentes de luz
//Fecha de Entrega: 24 de Octubre,2025
//No. de cuenta: 319023024

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

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

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
bool keys[1024] = { false };
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights 
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica9_Fuentes de luz_Alexa Lopez", nullptr, nullptr);
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // ======= MODELOS =======
    Model Piso((char*)"Models/piso.obj");
    Model RedDog((char*)"Models/RedDog.obj");
    Model Granja((char*)"granja/house_flat.obj");
    Model Arbol((char*)"Arbol/tree_trunk_axis_textures.obj");
    Model Pelota((char*)"BasketBall/BasketBall.obj");
    Model Flor((char*)"Flower/Flower_.obj");
    Model Tetera((char*)"Tetera/OBJ/Teapot.obj");
	Model lampara1((char*)"lamp/uploads_files_2605484_lamp.obj");
	Model lampara2((char*)"lamp1/uploads_files_5284295_Desk+Lamp.obj");
	Model lampara3((char*)"lamp2/Lanternv001.obj");
    // ========================

    // Posiciones de la bombilla dentro de cada lámpara
    glm::vec3 lampLightPos[3] = {
        glm::vec3(-2.2f,  2.6f, 0.0f), // lámpara 1
        glm::vec3(1.4f, -0.3f, 0.6f), // lámpara 2
        glm::vec3(2.8f,  0.40f, 0.0f)  // lámpara 3
    };

    // Colores (RGB) por lámpara
    glm::vec3 lampColor[3] = {
        glm::vec3(0.60f, 0.80f, 1.00f), // azul cielo (lamp 1)
        glm::vec3(1.0f, 0.7f, 0.0f), // amarillo cálido (lamp 2)
        glm::vec3(1.00f, 0.25f, 0.20f)  // rojo (lamp 3)
    };


    // VAO/VBO para el cubo 
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Material uniforms
    lightingShader.Use();
    glUniform1f(glGetUniformLocation(lightingShader.Program, "alphaMul"), 0.4f);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);


    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // ====== LUCES  ======
        lightingShader.Use();

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.001f, 0.001f, 0.001f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.01f, 0.01f, 0.01f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);

        // Point lights
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);

        // ========= POINT LIGHTS: 3 lámparas activas, 4ª apagada =========

        const float kC = 0.1f;
        const float kL = 0.5f;
        const float kQ = 0.5f;

        // --- Lámpara 1: azul cielo ---
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),lampLightPos[0].x, lampLightPos[0].y, lampLightPos[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),lampColor[0].x * 0.05f, lampColor[0].y * 0.05f, lampColor[0].z * 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),lampColor[0].x, lampColor[0].y, lampColor[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),lampColor[0].x, lampColor[0].y, lampColor[0].z);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), kC);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), kL);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), kQ);

        // --- Lámpara 2: amarillo cálido ---
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"),lampLightPos[1].x, lampLightPos[1].y, lampLightPos[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"),lampColor[1].x * 0.05f, lampColor[1].y * 0.05f, lampColor[1].z * 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"),lampColor[1].x, lampColor[1].y, lampColor[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"),lampColor[1].x, lampColor[1].y, lampColor[1].z);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), kC);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), kL);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), kQ);

        // --- Lámpara 3: rojo ---
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"),lampLightPos[2].x, lampLightPos[2].y, lampLightPos[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"),lampColor[2].x * 0.05f, lampColor[2].y * 0.05f, lampColor[2].z * 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"),lampColor[2].x, lampColor[2].y, lampColor[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"),lampColor[2].x, lampColor[2].y, lampColor[2].z);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), kC);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), kL);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), kQ);

        // --- Luz 4 desactivada ---
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);


        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.74f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        // Matrices
        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1.0f);

        // ====== DIBUJO DE MODELOS ======

        // Piso
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3( 0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // RedDog
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 0.35f, 0.5f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        RedDog.Draw(lightingShader);

        // Granja
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, -0.75f, 0.0f ));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f ));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Granja.Draw(lightingShader);

        // lampara1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.4f, 1.50f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.4f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        lampara1.Draw(lightingShader);

        // lampara2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.2f, -1.75f, 0.6f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        lampara2.Draw(lightingShader);

        // lampara3
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        lampara3.Draw(lightingShader);

        // Árbol
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, -0.35f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Arbol.Draw(lightingShader);

        // Pelota
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.75f, 2.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Pelota.Draw(lightingShader);

        // Flor
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, -0.7f, 2.0f ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Flor.Draw(lightingShader);

        // Tetera
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.7f, -0.8f ));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Tetera.Draw(lightingShader);

      
       /* lampShader.Use();
        GLint mLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint vLoc = glGetUniformLocation(lampShader.Program, "view");
        GLint pLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        for (GLuint i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);*/

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    // Mover luz puntual 0 
    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    if (keys[GLFW_KEY_SPACE])
    {
        active = !active;
        if (active)  Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
        else         Light1 = glm::vec3(0);
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
