/*
 * 相机封装类。
 * 
 * 创建时间：2022年9月20日 于上海市嘉定区安亭镇
 */
#include "ToyGraphCommon/EngineCompileOptions.h"
#include "ToyGraph/Camera.h"

using namespace std;

/* ------------ setters and getters. ------------ */

Camera& Camera::setFov(float fov) {
    this->fov = fov;
    return *this;
}

float Camera::getFov() {
    return this->fov;
}

Camera& Camera::setMovementSpeed(float movementSpeed) {
    this->movementSpeed = movementSpeed;
    return *this;
}

float Camera::getMovementSpeed() {
    return this->movementSpeed;
}

/* ------------ 工具方法。 ------------ */

glm::mat4 Camera::getViewMatrix() {
    glm::mat4 view = glm::lookAt(
        this->getPosition(), 
        this->getPosition() + this->getDirectionVectorFront(), 
        this->getDirectionVectorUp()
    );

    return view;
}
