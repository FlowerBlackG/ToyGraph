/* 2051565 GTY */

#include <ToyGraphCommon/EngineCompileOptions.h>

#include "Planet.h"

void Planet::updateTransform(float deltaT) {

    // 自转。
    float yaw = this->getYaw();
    this->setYaw(yaw + deltaT * this->selfRotationSpeed);

    this->revolutionRotation += this->revolutionSpeed * deltaT;

    // 公转。
    auto pos = glm::vec3(0.0f, 0.0f, 0.0f);
    pos.x = glm::cos(glm::radians(this->revolutionRotation)) * this->orbitRadius;

    pos.z = glm::sin(glm::radians(this->revolutionRotation)) * this->orbitRadius;

    this->setPosition(pos + this->rotationCenter);
}
