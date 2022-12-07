/* 2051565 GTY */
/*
    行星。
    for 课程作业。
*/

#pragma once

#include <ToyGraphCommon/EngineCompileOptions.h>
#include <ToyGraph/Engine.h>

class Planet : public Actor {
public:
    /** 更新方位。 */
    void updateTransform(float deltaT);

    virtual void tick(float deltaT) override {
        this->updateTransform(deltaT);
        Actor::tick(deltaT); // tick children
    }

    virtual void render(class Shader* pShader = nullptr) {
        pShader = &shader;
        shader.setMatrix4fv("model", this->getModelMatrix());

        this->pModel->draw(shader);
    }

public: 
    /** 轨道半径。 */
    float orbitRadius = 0.0f;

    /** 自转速度。度每秒。 */
    float selfRotationSpeed = 0.0f;

    /** 公转速度。度每秒。 */
    float revolutionSpeed = 0.0f;

    /** 自转角度。 */
    float revolutionRotation = 0.0f;

    /** 公转中心。 */
    glm::vec3 rotationCenter = glm::vec3(0, 0, 0);

    /** 颜色滤镜。 */
    glm::vec3 colorFilter;
};
