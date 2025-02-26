//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Modified by Assistant on [Date] to handle directional and point lighting toggling with L key.
//

#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include "GL/glew.h"
#endif

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"
#include "Rain.hpp"


#include <iostream>

const int glWindowWidth = 1280;
const int glWindowHeight = 720;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

bool cameraAnimationActive = false;   
float cameraAnimationTime = 0.0f;     
float cameraAnimationSpeed = 0.5f;   

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;

float sunAngle = 0.0f;
float sunIntensity = 1.0f;
glm::vec3 sunDirection = glm::vec3(0.0f, 1.0f, 1.0f);
glm::vec3 baseSunColor = glm::vec3(1.0f, 0.95f, 0.8f);

GLuint lightDirLoc;
GLuint lightColorLoc;



gps::Camera myCamera(
    glm::vec3(0.0f, 0.2f, 0.6f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 0.01f;

bool pressedKeys[1024];
float angleY = 0.0f;

gps::Model3D myModel;
gps::Model3D light;
gps::Shader myCustomShader;
gps::Model3D drinkGlass;
gps::Model3D drinkGlass2;
gps::Model3D drinkGlass3;
gps::Model3D torta;
gps::Model3D torta2;
gps::Model3D torta3;

gps::Model3D umbrella;
gps::Model3D bota;

//std::vector<glm::vec3> cameraPositions = {
//      glm::vec3(-0.2f, 0.1f, 0.4f),
//     glm::vec3(-0.2f, 0.1f, -0.2f),
//    glm::vec3(0.1f, 0.1f, -0.3f),
//    glm::vec3(0.2f, 0.16f, 0.0f),
//    glm::vec3(-0.17f, 0.1f, 0.3f),
//};
//
//std::vector<glm::vec3> cameraTargets = {
//    glm::vec3(0.3f, 0.2f, -0.1f),
//    glm::vec3(-0.1f, 0.12f, 0.1f),
//    glm::vec3(-0.17f, 0.1f, 0.35f),
//    glm::vec3(0.2f, 0.15f, 0.0f),
//    glm::vec3(-0.17f, 0.1f, 0.2f),
//
//};

std::vector<glm::vec3> cameraPositions = {
      glm::vec3(-0.2f, 0.1f, 0.4f),
     glm::vec3(-0.2f, 0.1f, -0.2f),
    glm::vec3(0.1f, 0.1f, -0.3f),
    glm::vec3(0.1f, 0.16f, 0.0f),
    glm::vec3(-0.17f, 0.1f, 1.0f),
};

std::vector<glm::vec3> cameraTargets = {
    glm::vec3(0.3f, 0.2f, -0.1f),
    glm::vec3(-0.1f, 0.12f, 0.1f),
    glm::vec3(-0.17f, 0.1f, 0.35f),
    glm::vec3(0.2f, 0.15f, 0.0f),
    glm::vec3(-0.17f, 0.1f, 0.2f),

};

int totalWaypoints = 5;
float waypointTime = 3.0f; 


bool pointLightEnabled = false; 
GLuint pointLightEnabledLoc;    

bool pointTorchEnabled = false;
GLuint pointTorchEnabledLoc;

bool pointTorchEnabled2 = false;
GLuint pointTorchEnabledLoc2;

bool pointTorchEnabled3 = false;
GLuint pointTorchEnabledLoc3;


GLuint pointLightPosWorldLoc;
GLuint pointLightColorLoc;

GLuint pointTorchPosWorldLoc;
GLuint pointTorchColorLoc;

GLuint pointTorchPosWorldLoc2;
GLuint pointTorchColorLoc2;

GLuint pointTorchPosWorldLoc3;
GLuint pointTorchColorLoc3;


GLuint constantAttenuationLoc;
GLuint linearAttenuationLoc;
GLuint quadraticAttenuationLoc;
GLuint glassColorLoc;

GLuint objectColorLoc;
GLuint isEmissiveLoc;

GLenum polygonMode = GL_FILL; 


bool fogEnabled = false; 
bool rainEnabled = false;
GLuint fogEnabledLoc;
GLuint fogColorLoc;
GLuint fogDensityLoc;

gps::SkyBox mySkyBox;
gps::Shader skyboxShader;
gps::Shader rainShader;

float lastFrameTime = 0.0f;


Rain rain(30000, gps::Shader());

glm::mat4 lightProjection, lightView, lightSpaceMatrix;

float umbrellaRotationSpeed = 30.0f; 
float umbrellaCurrentAngle = 0.0f;   

float yaw = -90.0f; 
float pitch = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f; 
bool firstMouse = true;


GLenum glCheckError_(const char* file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
    glViewport(0, 0, width, height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            pressedKeys[key] = true;
        else if (action == GLFW_RELEASE)
            pressedKeys[key] = false;
    }


    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        polygonMode = GL_POINT; // Point mode
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        polygonMode = GL_LINE; // Wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        polygonMode = GL_FILL; // Fill mode
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        fogEnabled = !fogEnabled; // Toggle fod
        glUniform1i(fogEnabledLoc, fogEnabled);
    }


    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        pointLightEnabled = !pointLightEnabled; // Toggle the point light
    }

    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        pointTorchEnabled = !pointTorchEnabled; // Toggle the torch light
    }

    if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        pointTorchEnabled2 = !pointTorchEnabled2; // Toggle the torch light
    }

    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        pointTorchEnabled3 = !pointTorchEnabled3; // Toggle the torch light
    }


    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        rainEnabled = !rainEnabled; // Toggle rain
    }


    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        cameraAnimationActive = !cameraAnimationActive;
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    
}


void processMovement(float deltaTime)
{
    static float yaw = 0.0f;   
    static float pitch = 0.0f; 

    if (!cameraAnimationActive) {
        if (pressedKeys[GLFW_KEY_Q]) {
            yaw -= 1.0f; 
            myCamera.rotate(pitch, yaw);
            view = myCamera.getViewMatrix();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }

        if (pressedKeys[GLFW_KEY_E]) {
            yaw += 1.0f; 
            myCamera.rotate(pitch, yaw);
            view = myCamera.getViewMatrix();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }

        if (pressedKeys[GLFW_KEY_UP]) {
            myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
            view = myCamera.getViewMatrix();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }

        if (pressedKeys[GLFW_KEY_DOWN]) {
            myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
            view = myCamera.getViewMatrix();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }

        if (pressedKeys[GLFW_KEY_LEFT]) {
            myCamera.move(gps::MOVE_LEFT, cameraSpeed);
            view = myCamera.getViewMatrix();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }

        if (pressedKeys[GLFW_KEY_RIGHT]) {
            myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
            view = myCamera.getViewMatrix();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }

        if (pressedKeys[GLFW_KEY_T]) { 
            myCamera.moveUp(cameraSpeed);
            view = myCamera.getViewMatrix();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }

        if (pressedKeys[GLFW_KEY_Y]) { 
            myCamera.moveDown(cameraSpeed);
            view = myCamera.getViewMatrix();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }
    }
    else
    {

        cameraAnimationTime += deltaTime * cameraAnimationSpeed;

        float totalAnimTime = totalWaypoints * waypointTime;
        float currentAnimTime = fmod(cameraAnimationTime, totalAnimTime);
        int currentSegment = (int)(currentAnimTime / waypointTime);
        int nextSegment = (currentSegment + 1) % totalWaypoints;
        float tSegment = fmod(currentAnimTime, waypointTime) / waypointTime;
        glm::vec3 startPos = cameraPositions[currentSegment];
        glm::vec3 endPos = cameraPositions[nextSegment];
        glm::vec3 newPos = glm::mix(startPos, endPos, tSegment);
        glm::vec3 startTarget = cameraTargets[currentSegment];
        glm::vec3 endTarget = cameraTargets[nextSegment];
        glm::vec3 newTarget = glm::mix(startTarget, endTarget, tSegment);

        newTarget.y = newPos.y;
        myCamera = gps::Camera(newPos, newTarget, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    if (pressedKeys[GLFW_KEY_Z]) {
        sunAngle += 1.0f;
    }
    if (pressedKeys[GLFW_KEY_X]) {
        sunAngle -= 1.0f;
    }
    if (pressedKeys[GLFW_KEY_S]) {
        sunIntensity += 0.01f;
    }
    if (pressedKeys[GLFW_KEY_A]) {
        sunIntensity -= 0.01f;
    }
    if (sunIntensity < 0.0f) sunIntensity = 0.0f;
    if (sunIntensity > 1.0f) sunIntensity = 1.0f;

    float radians = glm::radians(sunAngle);
    float x = sin(radians);
    float y = 1.0f;
    float z = cos(radians);
    sunDirection = glm::normalize(glm::vec3(x, y, z));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glm::vec3 sunDirEye = glm::normalize(glm::vec3(view * glm::vec4(sunDirection, 0.0f)));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(sunDirEye));

    glm::vec3 finalSunColor = baseSunColor * sunIntensity;
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(finalSunColor));

    glUniform1i(pointLightEnabledLoc, pointLightEnabled);

    glUniform1i(pointTorchEnabledLoc, pointTorchEnabled);

    glUniform1i(pointTorchEnabledLoc2, pointTorchEnabled2);

    glUniform1i(pointTorchEnabledLoc3, pointTorchEnabled3);




}

bool initOpenGLWindow()
{
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
    if (!glWindow) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
    glfwSetKeyCallback(glWindow, keyboardCallback);
    glfwSetCursorPosCallback(glWindow, mouseCallback);

    glfwMakeContextCurrent(glWindow);

    glfwSwapInterval(1); 

#if not defined (__APPLE__)
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "ERROR: Failed to initialize GLEW\n");
        return false;
    }
#endif

    // Get version info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);   
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // For RETINA display
    glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

    return true;
}

void initOpenGLState()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0);
    glViewport(0, 0, retina_width, retina_height);

    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS);    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);   
    glFrontFace(GL_CCW);     

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


void initObjects() {
    myModel.LoadModel("objects/scene/scene3.obj", "objects/scene/");
    light.LoadModel("objects/light/SA_LD_Medieval_Horn_Lantern.obj", "objects/light/");
    drinkGlass.LoadModel("objects/glass/Glass.obj");
    drinkGlass2.LoadModel("objects/glass/Glass.obj");
    drinkGlass3.LoadModel("objects/glass/Glass.obj");
    torta.LoadModel("objects/torta/tikiTorch.obj");
    umbrella.LoadModel("objects/umbrella2/untitled.obj");
    bota.LoadModel("objects/bota/bota.obj");
    torta2.LoadModel("objects/torta/tikiTorch.obj");
    torta3.LoadModel("objects/torta/tikiTorch.obj");




}

void initShaders() {
    myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
    myCustomShader.useShaderProgram();
    skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
    skyboxShader.useShaderProgram();
    rainShader.loadShader("shaders/rain.vert", "shaders/rain.frag");
    rainShader.useShaderProgram();

    rain= Rain(30000, rainShader);
    rain.setSpawnAreaLimits(-3.0f, 3.0f, -3.0f, 3.0f); 
    rain.initializeRainSystem();

}

void initUniforms() {

    myCustomShader.useShaderProgram();

    modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
    viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
    projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
    normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
    lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
    lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");

    pointLightEnabledLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLightEnabled");
    pointLightPosWorldLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLightPosWorld");
    pointLightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor");

    pointTorchEnabledLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchEnabled");
    pointTorchPosWorldLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchPosWorld");
    pointTorchColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchColor");

    pointTorchEnabledLoc2 = glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchEnabled2");
    pointTorchPosWorldLoc2 = glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchPosWorld2");
    pointTorchColorLoc2 = glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchColor2");

    pointTorchEnabledLoc3 = glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchEnabled3");
    pointTorchPosWorldLoc3 = glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchPosWorld3");
    pointTorchColorLoc3 = glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchColor3");



    constantAttenuationLoc = glGetUniformLocation(myCustomShader.shaderProgram, "constantAttenuation");
    linearAttenuationLoc = glGetUniformLocation(myCustomShader.shaderProgram, "linearAttenuation");
    quadraticAttenuationLoc = glGetUniformLocation(myCustomShader.shaderProgram, "quadraticAttenuation");
    objectColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "objectColor");
    isEmissiveLoc = glGetUniformLocation(myCustomShader.shaderProgram, "isEmissive");



    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glm::vec3 sunDirEye = glm::normalize(glm::vec3(view * glm::vec4(sunDirection, 0.0f)));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(sunDirEye));

    glm::vec3 finalSunColor = baseSunColor * sunIntensity;
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(finalSunColor));

    glm::vec3 torchLightPosWorld = glm::vec3(0.3f, 0.1f, 0.0f); 
    glUniform3fv(pointLightPosWorldLoc, 1, glm::value_ptr(torchLightPosWorld));

    glm::vec3 torchLightColor = glm::vec3(1.0f, 0.8f, 0.5f); 
    glUniform3fv(pointLightColorLoc, 1, glm::value_ptr(torchLightColor));

    glm::vec3 tortaLightPosWorld = glm::vec3(-0.1f, 0.08f, 0.3f);
    glUniform3fv(pointTorchPosWorldLoc, 1, glm::value_ptr(tortaLightPosWorld));

    glm::vec3 tortaLightColor = glm::vec3(1.0f, 0.4f, 0.5f); 
    glUniform3fv(pointTorchColorLoc, 1, glm::value_ptr(tortaLightColor));

    glm::vec3 tortaLightPosWorld2 = glm::vec3(-0.1f, 0.05f, 0.08f); 
    glUniform3fv(pointTorchPosWorldLoc2, 1, glm::value_ptr(tortaLightPosWorld2));

    glm::vec3 tortaLightColor2 = glm::vec3(0.3f, 0.5f, 0.8f); 
    glUniform3fv(pointTorchColorLoc2, 1, glm::value_ptr(tortaLightColor2));

    glm::vec3 tortaLightPosWorld3 = glm::vec3(-0.1f, 0.05f, 0.17f); 
    glUniform3fv(pointTorchPosWorldLoc3, 1, glm::value_ptr(tortaLightPosWorld3));

    glm::vec3 tortaLightColor3 = glm::vec3(0.2f, 0.6f, 1.0f);
    glUniform3fv(pointTorchColorLoc3, 1, glm::value_ptr(tortaLightColor3));

    glUniform1f(constantAttenuationLoc, 1.0f);
    glUniform1f(linearAttenuationLoc, 0.09f);
    glUniform1f(quadraticAttenuationLoc, 0.032f);

    glUniform1i(pointLightEnabledLoc, pointLightEnabled);

    glUniform1i(pointTorchEnabledLoc, pointTorchEnabled);

    glUniform1i(pointTorchEnabledLoc2, pointTorchEnabled2);

    glUniform1i(pointTorchEnabledLoc3, pointTorchEnabled3);


    fogEnabledLoc = glGetUniformLocation(myCustomShader.shaderProgram, "fogEnabled");
    fogColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "fogColor");
    fogDensityLoc = glGetUniformLocation(myCustomShader.shaderProgram, "fogDensity");

    glUniform1i(fogEnabledLoc, fogEnabled); 
    glUniform4f(fogColorLoc, 0.5f, 0.5f, 0.5f, 1.0f);
    glUniform1f(fogDensityLoc, 0.80f); 

    GLuint alphaLoc = glGetUniformLocation(myCustomShader.shaderProgram, "alpha");
    glUniform1f(alphaLoc, 0.5f);

    glassColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "glassColor");



}

glm::mat4 computeLightSpaceMatrix() {

    glm::vec3 sunDirEye = glm::normalize(glm::vec3(view * glm::vec4(sunDirection, 0.0f)));

    glm::mat4 lightView = glm::lookAt(sunDirEye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const GLfloat near_plane = 0.1f, far_plane = 10.0f;
    glm::mat4 lightProjection = glm::ortho ( - 1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);
    lightSpaceMatrix = lightProjection * lightView;
    return lightSpaceMatrix;
}




void renderScene(float deltaTime) {


        glViewport(0, 0, retina_width, retina_height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniform1i(fogEnabledLoc, fogEnabled);
        glUniform1f(fogDensityLoc, 0.80f);     

       

        if (!rainEnabled && !fogEnabled) {
            glDepthFunc(GL_LEQUAL);
            skyboxShader.useShaderProgram();
            glm::mat4 skyboxView = glm::mat4(glm::mat3(myCamera.getViewMatrix()));
            mySkyBox.Draw(skyboxShader, skyboxView, projection);
            glDepthFunc(GL_LESS);
        }
        else {
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f); 
        }

        if (rainEnabled) {
            rain.updateRainDrops(deltaTime);
            rain.uploadRainDataToGPU();
            rain.renderRain(projection, view); 

        }


        myCustomShader.useShaderProgram();



        glm::mat4 torchModel = glm::mat4(1.0f);
        torchModel = glm::translate(torchModel, glm::vec3(0.3f, 0.1f, 0.0f)); 
        torchModel = glm::scale(torchModel, glm::vec3(0.05f, 0.05f, 0.05f)); 
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(torchModel));
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorch"), true);
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isGlass"), false); 
        glDisable(GL_BLEND);             

        light.Draw(myCustomShader);

        glm::vec3 torchWorldPos = glm::vec3(0.3f, 0.1f, 0.0f);
        glm::vec3 torchPosEye = glm::vec3(view * glm::vec4(torchWorldPos, 1.0f));



        glm::mat4 torch2Model = glm::mat4(1.0f);
        torch2Model = glm::translate(torch2Model, glm::vec3(-0.1f, 0.08f, 0.3f)); 
        torch2Model = glm::scale(torch2Model, glm::vec3(0.0001f, 0.0001f, 0.0001f));  
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(torch2Model));
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorta"), true); 
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorch"), false); 
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isGlass"), false);
        glDisable(GL_BLEND);            

        torta.Draw(myCustomShader);

        glm::vec3 tortaWorldPos = glm::vec3(-0.1f, 0.08f, 0.3f);
        glm::vec3 tortaPosEye = glm::vec3(view * glm::vec4(tortaWorldPos, 1.0f));


        glm::mat4 torch3Model = glm::mat4(1.0f);
        torch3Model = glm::translate(torch3Model, glm::vec3(-0.1f, 0.05f, 0.07f));
        torch3Model = glm::scale(torch3Model, glm::vec3(0.0001f, 0.0001f, 0.0001f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(torch3Model));
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorta2"), true);
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorch"), false);
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isGlass"), false); 
        glDisable(GL_BLEND);           

        torta2.Draw(myCustomShader);

        glm::vec3 tortaWorldPos2 = glm::vec3(-0.1f, 0.05f, 0.08f);
        glm::vec3 tortaPosEye2 = glm::vec3(view * glm::vec4(tortaWorldPos2, 1.0f));


        glm::mat4 torch4Model = glm::mat4(1.0f);
        torch4Model = glm::translate(torch4Model, glm::vec3(-0.1f, 0.05f, 0.1f)); 
        torch4Model = glm::scale(torch4Model, glm::vec3(0.0001f, 0.0001f, 0.0001f)); 
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(torch4Model));
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorta3"), true); 
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorch"), false); 
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isGlass"), false); 
        glDisable(GL_BLEND);           

        torta3.Draw(myCustomShader);

        glm::vec3 tortaWorldPos3 = glm::vec3(-0.1f, 0.05f, 0.17f);
        glm::vec3 tortaPosEye3 = glm::vec3(view * glm::vec4(tortaWorldPos3, 1.0f));

        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorch"), false); 
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isGlass"), false); 
        glDisable(GL_BLEND);             

        glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightPosEye"), 1, glm::value_ptr(torchPosEye));
        glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.8f, 0.5f)));

        glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchPosEye"), 1, glm::value_ptr(tortaPosEye));
        glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.8f, 0.5f))); 


        glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchPosEye2"), 1, glm::value_ptr(tortaPosEye2));
        glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchColor2"), 1, glm::value_ptr(glm::vec3(0.3f, 0.5f, 0.8f)));

        glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchPosEye3"), 1, glm::value_ptr(tortaPosEye3));
        glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "pointTorchColor3"), 1, glm::value_ptr(glm::vec3(0.2f, 0.6f, 1.0f))); 

        glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "constantAttenuation"), 1.0f);
        glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "linearAttenuation"), 0.09f);
        glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "quadraticAttenuation"), 0.032f);

        myModel.Draw(myCustomShader);

        float rotationAngleDegrees = -90.0f; 
        float inclinationAngleDegrees = 20.0f; 

        float rotationAngleRadians = glm::radians(rotationAngleDegrees); 
        glm::vec3 rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f); 
        glm::vec3 umbrellaDynamicRotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);

        glm::mat4 botasta = glm::mat4(1.0f);
        botasta = glm::translate(botasta, glm::vec3(-0.17f, 0.01f, 0.35f));
        botasta = glm::scale(botasta, glm::vec3(0.0008f, 0.0008f, 0.0008f));
        botasta = glm::rotate(botasta, rotationAngleRadians, rotationAxis); 
        botasta = glm::rotate(botasta, glm::radians(inclinationAngleDegrees), glm::vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(botasta));


        bota.Draw(myCustomShader);

        umbrellaCurrentAngle += umbrellaRotationSpeed * deltaTime; 

        if (umbrellaCurrentAngle > 360.0f)
            umbrellaCurrentAngle -= 360.0f; 
        float umbrellaRotationRadians = glm::radians(umbrellaCurrentAngle);



        glm::mat4 umbrelaroteste = glm::mat4(1.0f);
        umbrelaroteste = glm::translate(umbrelaroteste, glm::vec3(-0.17f, 0.01f, 0.35f)); 
        umbrelaroteste = glm::scale(umbrelaroteste, glm::vec3(0.0008f, 0.0008f, 0.0008f));

        umbrelaroteste = glm::rotate(umbrelaroteste, rotationAngleRadians, rotationAxis);
        umbrelaroteste = glm::rotate(umbrelaroteste, glm::radians(inclinationAngleDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
        umbrelaroteste = glm::rotate(umbrelaroteste, umbrellaRotationRadians, umbrellaDynamicRotationAxis); 


        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(umbrelaroteste));
        umbrella.Draw(myCustomShader);




        glm::mat4 glassModel = glm::mat4(1.0f);
        glassModel = glm::translate(glassModel, glm::vec3(-0.13f, 0.01f, 0.35f));
        glassModel = glm::scale(glassModel, glm::vec3(0.007f, 0.007f, 0.007f));
        glUniform3f(glassColorLoc, 0.1f, 0.0f, 0.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glassModel));
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorch"), false); 
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isGlass"), true);  
        glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "alpha"), 0.5f);    

        glEnable(GL_BLEND); 
        drinkGlass.Draw(myCustomShader);
        glDisable(GL_BLEND); 

        glm::mat4 glassModel2 = glm::mat4(1.0f);
        glassModel2 = glm::translate(glassModel2, glm::vec3(-0.16f, 0.01f, 0.36f)); 
        glassModel2 = glm::scale(glassModel2, glm::vec3(0.007f, 0.007f, 0.007f));
        glUniform3f(glassColorLoc, 0.0f, 1.0f, 0.0f); 
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glassModel2));

        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorch"), false); 
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isGlass"), true);
        glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "alpha"), 0.5f);  

        glEnable(GL_BLEND); 
        drinkGlass2.Draw(myCustomShader);
        glDisable(GL_BLEND); 

        glm::mat4 glassModel3 = glm::mat4(1.0f);
        glassModel3 = glm::translate(glassModel3, glm::vec3(-0.18f, 0.01f, 0.4f)); 
        glassModel3 = glm::scale(glassModel3, glm::vec3(0.007f, 0.007f, 0.007f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glassModel3));
        glUniform3f(glassColorLoc, 0.0f, 0.0f, 1.0f); 

        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isTorch"), false); 
        glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "isGlass"), true);  
        glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "alpha"), 0.5f);   

        glEnable(GL_BLEND);
        drinkGlass3.Draw(myCustomShader);
        glDisable(GL_BLEND); 




    

}

void initSkybox() {
    std::vector<const GLchar*> faces = {
        "skybox/bluecloud_rt.jpg", "skybox/bluecloud_lf.jpg",
        "skybox/bluecloud_up.jpg", "skybox/bluecloud_dn.jpg",
        "skybox/bluecloud_bk.jpg", "skybox/bluecloud_ft.jpg"
    };
    mySkyBox.Load(faces);
}







// Clean up resources
void cleanup() {

    glfwDestroyWindow(glWindow);
    // Close GL context and any other GLFW resources
    glfwTerminate();
}

int main(int argc, const char* argv[]) {

    if (!initOpenGLWindow()) {
        glfwTerminate();
        return 1;
    }

    initOpenGLState();

    initObjects();
    initShaders();
    initUniforms();
    initSkybox();


    while (!glfwWindowShouldClose(glWindow)) {
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        processMovement(deltaTime);


        renderScene(deltaTime);


        glfwPollEvents();
        glfwSwapBuffers(glWindow);
    }

    cleanup();

    return 0;
}