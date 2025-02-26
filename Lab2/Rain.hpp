#ifndef RAIN_HPP
#define RAIN_HPP

#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "Shader.hpp"

struct RainDrop {
    glm::vec3 position;
    glm::vec3 velocity;
    float delayBeforeFall;
    bool isActive;
};

class Rain {
public:
    Rain(int totalRainDrops, gps::Shader rainShaderProgram);

    void initializeRainSystem();
    void updateRainDrops(float deltaTime);
    void uploadRainDataToGPU();

    void renderRain(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

    void cleanUpRainSystem();
    void setSpawnAreaLimits(float lowerX, float upperX, float lowerZ, float upperZ);

private:
    glm::vec3 generateRandomStartPosition();

    gps::Shader shaderProgram;
    std::vector<RainDrop> rainDrops;

    GLuint VAO_Rain = 0;
    GLuint VBO_Rain = 0;
    int totalRainDrops;
    bool isReady;
    float lowerX, upperX, lowerZ, upperZ;
};

#endif
