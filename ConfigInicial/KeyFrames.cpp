//Nombre: Alexa Fernanda López Tavera
//Previo 12: Animación por keyframes
//Fecha de Entrega: 9 de Noviembre, 2025
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
void Animation();

// Carga automática: sentado con dos traseras flex + saludo con delantera derecha + cola en wag
void LoadSitWaveTail();

const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Luz
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Punto de luz
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,2.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
    -0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,
     0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f, -0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f, -0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,
    -0.5f,-0.5f, 0.5f,  0.0f,0.0f, 1.0f,  0.5f,-0.5f, 0.5f,  0.0f,0.0f, 1.0f,  0.5f, 0.5f, 0.5f,  0.0f,0.0f, 1.0f,
     0.5f, 0.5f, 0.5f,  0.0f,0.0f, 1.0f, -0.5f, 0.5f, 0.5f,  0.0f,0.0f, 1.0f, -0.5f,-0.5f, 0.5f,  0.0f,0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,  -0.5f, 0.5f,-0.5f, -1.0f,0.0f,0.0f,  -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,
    -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,  -0.5f,-0.5f, 0.5f, -1.0f,0.0f,0.0f,  -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,
     0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,   0.5f, 0.5f,-0.5f,  1.0f,0.0f,0.0f,   0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,
     0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,   0.5f,-0.5f, 0.5f,  1.0f,0.0f,0.0f,   0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,
    -0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,   0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f,  0.5f,-0.5f, 0.5f, 0.0f,-1.0f,0.0f,
     0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  -0.5f,-0.5f, 0.5f, 0.0f,-1.0f,0.0f, -0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f,
    -0.5f, 0.5f,-0.5f,  0.0f, 1.0f,0.0f,   0.5f, 0.5f,-0.5f, 0.0f, 1.0f,0.0f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,0.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 1.0f,0.0f,  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,0.0f, -0.5f, 0.5f,-0.5f, 0.0f, 1.0f,0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Anim vars
float rotBall = 0.0f;
float rotDog = 0.0f;

// Delanteras
float FLegs = 0.0f;        // Front Left (queda en 0)
float fRightLeg = 0.0f;    // Front Right (saludo)

// Traseras (ambas positivas = flexionadas/sentado)
float RLegs = 0.0f;        // Back Right
float bLeftLeg = 0.0f;     // Back Left

// Cabeza/cola
float head = 0.0f;
float tail = 0.0f;

// Posición del perro
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;

typedef struct _frame {
    float rotDog, rotDogInc;

    float dogPosX, dogPosY, dogPosZ;
    float incX, incY, incZ;

    float head, headInc;

    float tail, tailInc;           // COLA

    float RLegs, RLegsInc;         // Back Right  (flex +)
    float bLeftLeg, bLeftLegInc;   // Back Left   (flex +)

    float fRightLeg, fRightLegInc; // Front Right (saludo +)
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;  // introducir datos
bool play = false;
int playIndex = 0;

// ---- util ----
void saveFrame(void) {
    printf("frameindex %d\n", FrameIndex);

    KeyFrame[FrameIndex].dogPosX = dogPosX;
    KeyFrame[FrameIndex].dogPosY = dogPosY;
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;

    KeyFrame[FrameIndex].rotDog = rotDog;
    KeyFrame[FrameIndex].head = head;
    KeyFrame[FrameIndex].tail = tail;

    KeyFrame[FrameIndex].RLegs = RLegs;
    KeyFrame[FrameIndex].bLeftLeg = bLeftLeg;
    KeyFrame[FrameIndex].fRightLeg = fRightLeg;

    FrameIndex++;
}

void resetElements(void) {
    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;

    rotDog = KeyFrame[0].rotDog;
    head = KeyFrame[0].head;
    tail = KeyFrame[0].tail;

    RLegs = KeyFrame[0].RLegs;
    bLeftLeg = KeyFrame[0].bLeftLeg;
    fRightLeg = KeyFrame[0].fRightLeg;

    // La delantera izquierda queda en 0
    FLegs = 0.0f;
}

void interpolation(void) {
    KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
    KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
    KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;

    KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
    KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
    KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;

    KeyFrame[playIndex].RLegsInc = (KeyFrame[playIndex + 1].RLegs - KeyFrame[playIndex].RLegs) / i_max_steps;
    KeyFrame[playIndex].bLeftLegInc = (KeyFrame[playIndex + 1].bLeftLeg - KeyFrame[playIndex].bLeftLeg) / i_max_steps;
    KeyFrame[playIndex].fRightLegInc = (KeyFrame[playIndex + 1].fRightLeg - KeyFrame[playIndex].fRightLeg) / i_max_steps;
}

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo 12_Animacion Keyframes_Alexa Lopez", nullptr, nullptr);
    if (!window) { std::cout << "Failed to create GLFW window\n"; glfwTerminate(); return EXIT_FAILURE; }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) { std::cout << "Failed to initialize GLEW\n"; return EXIT_FAILURE; }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Models
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // Init keyframes
    for (int i = 0; i < MAX_FRAMES; i++) {
        KeyFrame[i].dogPosX = KeyFrame[i].dogPosY = KeyFrame[i].dogPosZ = 0;
        KeyFrame[i].incX = KeyFrame[i].incY = KeyFrame[i].incZ = 0;
        KeyFrame[i].rotDog = KeyFrame[i].rotDogInc = 0;
        KeyFrame[i].head = KeyFrame[i].headInc = 0;
        KeyFrame[i].tail = KeyFrame[i].tailInc = 0;
        KeyFrame[i].RLegs = KeyFrame[i].RLegsInc = 0;
        KeyFrame[i].bLeftLeg = KeyFrame[i].bLeftLegInc = 0;
        KeyFrame[i].fRightLeg = KeyFrame[i].fRightLegInc = 0;
    }

    // VAO simple para el cubito de luz
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

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // ---- loop ----
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame; lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        Animation();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1.0f), modelTemp(1.0f);

        // Piso
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // ---- Perro ----
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        // Body
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
        model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
        modelTemp = model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // Head
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Tail (wag)
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Front Left (quieta)
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Front Right (saludo hacia arriba/adelante: +X)
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(-fRightLeg), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Back Left (flexión positiva)
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(-bLeftLeg), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Back Right (flexión positiva)
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // Pelota (opcional)
        model = glm::mat4(1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        // Lamparita
        lampShader.Use();
        GLint m2 = glGetUniformLocation(lampShader.Program, "model");
        GLint v2 = glGetUniformLocation(lampShader.Program, "view");
        GLint p2 = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(v2, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(p2, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(m2, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// ---------- input / anim ----------
void DoMovement() {
    if (keys[GLFW_KEY_4]) head += 1.0f;
    if (keys[GLFW_KEY_2]) rotDog += 1.0f;
    if (keys[GLFW_KEY_3]) rotDog -= 1.0f;

    if (keys[GLFW_KEY_H]) dogPosZ += 0.01f;
    if (keys[GLFW_KEY_Y]) dogPosZ -= 0.01f;
    if (keys[GLFW_KEY_G]) dogPosX -= 0.01f;
    if (keys[GLFW_KEY_J]) dogPosX += 0.01f;

    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // Reproducir/Pausar
    if (keys[GLFW_KEY_L]) {
        if (!play && (FrameIndex > 1)) {
            resetElements();
            interpolation();
            play = true; playIndex = 0; i_curr_steps = 0;
        }
        else {
            play = false;
        }
    }

    // Guardar KF manual
    if (keys[GLFW_KEY_K]) {
        if (FrameIndex < MAX_FRAMES) saveFrame();
    }

    // Cargar secuencia completa
    if (keys[GLFW_KEY_P]) LoadSitWaveTail();

    if (keys[GLFW_KEY_SPACE]) {
        active = !active;
        Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
    }
}

void Animation() {
    if (!play) return;

    if (i_curr_steps >= i_max_steps) {
        playIndex++;
        if (playIndex > FrameIndex - 2) {
            printf("termina anim\n");
            playIndex = 0; play = false;
        }
        else {
            i_curr_steps = 0;
            interpolation();
        }
    }
    else {
        dogPosX += KeyFrame[playIndex].incX;
        dogPosY += KeyFrame[playIndex].incY;
        dogPosZ += KeyFrame[playIndex].incZ;

        rotDog += KeyFrame[playIndex].rotDogInc;
        head += KeyFrame[playIndex].headInc;
        tail += KeyFrame[playIndex].tailInc;

        RLegs += KeyFrame[playIndex].RLegsInc;
        bLeftLeg += KeyFrame[playIndex].bLeftLegInc;
        fRightLeg += KeyFrame[playIndex].fRightLegInc;

        i_curr_steps++;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos; lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}

// === Keyframes: dos traseras flexionadas + delantera derecha arriba + cola wag ===
void LoadSitWaveTail()
{
    FrameIndex = 0;

    auto pushKF = [&](float px, float py, float pz,
        float yaw, float h, float t,
        float backFlex,  // valor para ambas traseras (flex +)
        float fRight)    // delantera derecha (saludo +)
        {
            dogPosX = px; dogPosY = py; dogPosZ = pz;
            rotDog = yaw; head = h; tail = t;

            // Traseras: ambas igual y positivas (sentado)
            RLegs = backFlex;   // Back Right
            bLeftLeg = backFlex;   // Back Left

            // Delanteras: izquierda 0, derecha levanta
            FLegs = 0.0f;
            fRightLeg = fRight;

            if (FrameIndex < MAX_FRAMES) saveFrame();
        };

    // KF0: De pie
    pushKF(0.0f, 0.00f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    // KF1: Transición a sentado (traseras flex +35, cola sube)
    pushKF(0.0f, -0.03f, 0.0f, 0.0f, -4.0f, +10.0f, 35.0f, 0.0f);

    // KF2: Sentado estable (traseras +55, cola +15)
    pushKF(0.0f, -0.05f, 0.0f, 0.0f, -6.0f, +15.0f, 55.0f, 0.0f);

    // KF3: Saludo (delantera derecha +45, wag cola -15)
    pushKF(0.0f, -0.05f, 0.0f, 0.0f, -4.0f, -15.0f, 55.0f, 45.0f);

    // KF4: Mantiene saludo, wag a +10
    pushKF(0.0f, -0.05f, 0.0f, 0.0f, -5.0f, +10.0f, 55.0f, 45.0f);

    // KF5: Baja la delantera, sigue sentado, cola a 0
    pushKF(0.0f, -0.05f, 0.0f, 0.0f, -5.0f, 0.0f, 55.0f, 0.0f);

    std::cout << "KF: ambas traseras flex + delantera derecha arriba + cola wag. Total = " << FrameIndex << std::endl;
}
