#include "Rain.hpp"
#include <cstdlib>
#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "Shader.hpp"

Rain::Rain(int maxRainDrops, gps::Shader rainShaderProgram)
    : totalRainDrops(maxRainDrops), shaderProgram(rainShaderProgram), isReady(false),
    lowerX(-5.0f), upperX(5.0f), lowerZ(-5.0f), upperZ(5.0f) 
{
    rainDrops.resize(totalRainDrops);
}

void Rain::initializeRainSystem() {
    for (int i = 0; i < totalRainDrops; i++) {
        rainDrops[i].position = generateRandomStartPosition();

        float velocity = 300.0f + static_cast<float>(rand() % 61);
        rainDrops[i].velocity = glm::vec3(0.0f, -velocity, 0.0f);

        rainDrops[i].delayBeforeFall = static_cast<float>(rand() % 6000) / 1000.0f;
        rainDrops[i].isActive = false;
    }

    glGenVertexArrays(1, &VAO_Rain);
    glGenBuffers(1, &VBO_Rain);

    glBindVertexArray(VAO_Rain);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Rain);

    glBufferData(GL_ARRAY_BUFFER,
        2 * totalRainDrops * sizeof(glm::vec3),
        nullptr,
        GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    isReady = true;
}

void Rain::updateRainDrops(float deltaTime) {
    if (!isReady) return;

    for (int i = 0; i < totalRainDrops; i++) {
        auto& drop = rainDrops[i];

        if (!drop.isActive) {
            drop.delayBeforeFall -= deltaTime;
            if (drop.delayBeforeFall <= 0.0f) {
                drop.isActive = true;
            }
        }

        if (drop.isActive) {
            drop.position += drop.velocity * deltaTime;
            if (drop.position.y < -300.0f) {
                drop.position = generateRandomStartPosition();

                float fallSpeed = 60.0f + static_cast<float>(rand() % 150);
                drop.velocity = glm::vec3(0.0f, -fallSpeed, 0.0f);

                drop.delayBeforeFall = static_cast<float>(rand() % 6000) / 1000.0f;
                drop.isActive = false;
            }
        }
    }
}

void Rain::uploadRainDataToGPU() {
    if (!isReady) return;

    std::vector<glm::vec3> rainLines(2 * totalRainDrops);

    for (int i = 0; i < totalRainDrops; i++) {
        glm::vec3 startPosition = rainDrops[i].position;
        glm::vec3 endPosition = startPosition + glm::vec3(0.0f, 2.0f, 0.0f);

        rainLines[2 * i] = startPosition;
        rainLines[2 * i + 1] = endPosition;
    }

    glBindVertexArray(VAO_Rain);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Rain);

    glBufferSubData(GL_ARRAY_BUFFER,
        0,
        rainLines.size() * sizeof(glm::vec3),
        rainLines.data());

    glBindVertexArray(0);
}

void Rain::renderRain(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
    if (!isReady) return;

    shaderProgram.useShaderProgram();

    GLint rainColorLocation = glGetUniformLocation(shaderProgram.shaderProgram, "rainColor");
    glUniform3f(rainColorLocation, 0.0f, 0.0f, 1.0f);

    GLint projectionLocation = glGetUniformLocation(shaderProgram.shaderProgram, "projection");
    GLint viewLocation = glGetUniformLocation(shaderProgram.shaderProgram, "view");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glBindVertexArray(VAO_Rain);
    glDrawArrays(GL_LINES, 0, 2 * totalRainDrops);
    glBindVertexArray(0);
}


glm::vec3 Rain::generateRandomStartPosition() {
    float xPos = lowerX + static_cast<float>(rand()) / RAND_MAX * (upperX - lowerX);
    float zPos = lowerZ + static_cast<float>(rand()) / RAND_MAX * (upperZ - lowerZ);
    float yPos = 3.0f;
    return glm::vec3(xPos, yPos, zPos);
}

void Rain::setSpawnAreaLimits(float minX, float maxX, float minZ, float maxZ) {
    lowerX = minX;
    upperX = maxX;
    lowerZ = minZ;
    upperZ = maxZ;
}

void Rain::cleanUpRainSystem() {
    if (!isReady) return;

    if (VAO_Rain) {
        glDeleteVertexArrays(1, &VAO_Rain);
        VAO_Rain = 0;
    }
    if (VBO_Rain) {
        glDeleteBuffers(1, &VBO_Rain);
        VBO_Rain = 0;
    }

    rainDrops.clear();
    isReady = false;
}




