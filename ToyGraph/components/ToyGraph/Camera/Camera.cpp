/*
 * �����װ�ࡣ
 * 
 * ����ʱ�䣺2022��9��20�� ���Ϻ��мζ�����ͤ��
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

/* ------------ ���߷����� ------------ */

glm::mat4 Camera::getViewMatrix() {
    glm::mat4 view = glm::lookAt(
        this->getPosition(), 
        this->getPosition() + this->getDirectionVectorFront(), 
        this->getDirectionVectorUp()
    );

    return view;
}
