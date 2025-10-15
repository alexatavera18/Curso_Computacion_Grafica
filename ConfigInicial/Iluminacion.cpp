//Nombre : Alexa Fernanda López Tavera
//Practica 8 : Iluminación
// Modificar trayectoria para que sea en medio circulo, una luz de dia y otra de noche (Solo en la parte superior)
// Desactivar el sol con una tecla y activar la luz de noche 
// Entregar PDF, video del codigo funcionando y el codigo
//Fecha de entrega: 17 de Octubre,2025
//No. de cuenta:319023024*/

// Std. Includes
#include <string>
//Para hacer una esfera
#include <iostream>
#include <vector>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024] = { false };
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes

// --- Día/Noche y arco ---
bool  isDay = true;               // true=Sol (amarillo), false=Luna (azulado)
// Extremos del arco
const float X_LEFT = -5.0f;
const float X_RIGHT = 7.0f;
const float RADIUS = (X_RIGHT - X_LEFT) * 0.5f;   
const float CX = (X_RIGHT + X_LEFT) * 0.5f;       
const float APEX_Y = 6.0f;                        
const float CY = APEX_Y - RADIUS;                 
const float CZ = 2.0f; 
const float TOGGLE_EPS = 0.02f;  


// Parametrización: x = cx + R cos(theta), y = cy + R sin(theta)
float theta = glm::pi<float>();

// Posición de luz
glm::vec3 lightPos(0.0f);

// Timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// --- VAO/VBO de esfera (Sol/Luna) ---
GLuint SPHERE_VAO = 0, SPHERE_VBO = 0;
GLsizei SPHERE_VERT_COUNT = 0;

// Genera una esfera (pos + normal) en un solo VBO (layout: vec3 pos, vec3 normal)
void CreateSphere(int stacks, int slices, float radius, GLuint& vao, GLuint& vbo, GLsizei& vertCount)
{
    std::vector<float> data;
    data.reserve(stacks * slices * 6 * 3);

    for (int i = 0; i < stacks; ++i)
    {
        float phi1 = glm::pi<float>() * float(i) / float(stacks);
        float phi2 = glm::pi<float>() * float(i + 1) / float(stacks);

        for (int j = 0; j < slices; ++j)
        {
            float theta1 = 2.0f * glm::pi<float>() * float(j) / float(slices);
            float theta2 = 2.0f * glm::pi<float>() * float(j + 1) / float(slices);

            // Cuatro puntos del quad sobre la esfera
            glm::vec3 p1 = glm::vec3(
                std::sin(phi1) * std::cos(theta1),
                std::cos(phi1),
                std::sin(phi1) * std::sin(theta1)
            );
            glm::vec3 p2 = glm::vec3(
                std::sin(phi2) * std::cos(theta1),
                std::cos(phi2),
                std::sin(phi2) * std::sin(theta1)
            );
            glm::vec3 p3 = glm::vec3(
                std::sin(phi2) * std::cos(theta2),
                std::cos(phi2),
                std::sin(phi2) * std::sin(theta2)
            );
            glm::vec3 p4 = glm::vec3(
                std::sin(phi1) * std::cos(theta2),
                std::cos(phi1),
                std::sin(phi1) * std::sin(theta2)
            );

            // Dos triángulos: (p1,p2,p3) y (p1,p3,p4)
            glm::vec3 tri[6] = { p1,p2,p3,  p1,p3,p4 };
            for (int k = 0; k < 6; ++k)
            {
                glm::vec3 n = glm::normalize(tri[k]);
                glm::vec3 v = tri[k] * radius;
                data.push_back(v.x); data.push_back(v.y); data.push_back(v.z);
                data.push_back(n.x); data.push_back(n.y); data.push_back(n.z);
            }
        }
    }

    vertCount = (GLsizei)(data.size() / 6);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
/*glm::vec3 lightPos(0.5f, 0.5f, 2.5f);
float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;*/

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica8_Materiales e Iluminacion_Alexa Lopez", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model red_dog((char*)"Models/RedDog.obj");
    Model granja((char*)"granja/house_flat.obj");
    Model arbol((char*)"Arbol/tree_trunk_axis_textures.obj");
    Model pelota((char*)"BasketBall/BasketBall.obj");
    Model flor((char*)"Flower/Flower_.obj");
    Model tetera((char*)"Tetera/OBJ/Teapot.obj");




    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    /*
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

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    image = stbi_load("granja/color.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);
    CreateSphere(/*stacks*/24, /*slices*/36, /*radius*/0.6f, SPHERE_VAO, SPHERE_VBO, SPHERE_VERT_COUNT);

    // Estado inicial de la luz: extremo izquierdo (árbol)
    lightPos = glm::vec3(CX + RADIUS * std::cos(theta),
        CY + RADIUS * std::sin(theta),
        CZ);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();
        lightPos = glm::vec3(CX + RADIUS * std::cos(theta),
            CY + RADIUS * std::sin(theta),
            CZ);

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"),
            lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        if (isDay) {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.70f, 0.60f, 0.45f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 2.80f, 2.60f, 1.90f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 3.20f, 3.20f, 2.50f);
        }
        else {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.28f, 0.32f, 0.50f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.80f, 2.00f, 2.60f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 2.40f, 2.60f, 3.00f);
        }
        /**lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x + movelightPos, lightPos.y + movelightPos, lightPos.z + movelightPos);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);
        */


        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.2f,0.8f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.8f);



        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glBindVertexArray(VAO);
       
       // glDrawArrays(GL_TRIANGLES, 0, 36);
		// Perro
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.5f));                 
        red_dog.Draw(lightingShader);

        // Granja
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, -1.2f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));                  
        granja.Draw(lightingShader);

        // Arbol
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        // Pelota
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 2.0f));
        model = glm::scale(model, glm::vec3(3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pelota.Draw(lightingShader);

        // Flor
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f,4.5f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        flor.Draw(lightingShader);

        // Tetera
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f,-1.5f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tetera.Draw(lightingShader);

        glBindVertexArray(0);

        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        //Luz
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(1.0f)); // tamaño  del sol/luna
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Color de la esfera 
        GLint lampColorLoc = glGetUniformLocation(lampshader.Program, "lampColor");
        if (lampColorLoc >= 0)
        {
            if (isDay) glUniform3f(lampColorLoc, 1.0f, 0.90f, 0.55f);  // amarillo 
            else       glUniform3f(lampColorLoc, 0.75f, 0.75f, 0.78f); // azul 
        }

        glBindVertexArray(SPHERE_VAO);
        glDrawArrays(GL_TRIANGLES, 0, SPHERE_VERT_COUNT);
        glBindVertexArray(0);

        /*
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos + movelightPos);
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Swap the buffers*/
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &SPHERE_VAO);
    glDeleteBuffers(1, &SPHERE_VBO);
    glDeleteTextures(1, &texture);

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

   /* if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }
    */

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
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        // O: mover hacia la derecha (de árbol -> granja) => theta disminuye hacia 0
        if (key == GLFW_KEY_O)
        {
            theta -= 0.05f;
            if (theta < 0.0f) theta = 0.0f;
        }
        // L: mover hacia la izquierda (de granja -> árbol) => theta aumenta hacia π
        else if (key == GLFW_KEY_L)
        {
            theta += 0.05f;
            if (theta > glm::pi<float>()) theta = glm::pi<float>();
        }
        // ESPACIO
        else if (key == GLFW_KEY_SPACE) // cambia el día/noche 
        {
            bool atLeft = fabs(theta - glm::pi<float>()) < TOGGLE_EPS;
            bool atRight = fabs(theta - 0.0f) < TOGGLE_EPS;  

            if (atLeft || atRight) {
                isDay = !isDay;
                if (atLeft)  theta = glm::pi<float>();  
                if (atRight) theta = 0.0f;
            }
        }

    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
