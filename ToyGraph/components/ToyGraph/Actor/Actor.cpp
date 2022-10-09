/*
 * ��Ա�ࡣ
 * 
 * ����ʱ�䣺2022��9��20�� ���Ϻ��мζ�����ͤ��
 */

#include <cmath>

#include "ToyGraph/Actor.h"

#include <cstdio>

Actor::Actor() {
    this->updateDirectionVectors();
}

/* -------- setters and getters. -------- */

Actor& Actor::setPosition(const glm::vec3& position) {
    this->position = position;
    return *this;
}

glm::vec3& Actor::getPositionRef() {
    return this->position;
}

const glm::vec3& Actor::getPosition() {
    return this->position;
}

Actor& Actor::setYaw(const float yaw) {
    this->yaw = yaw;
    this->directionVectorsIsLatest = false;
    return *this;
}

float Actor::getYaw() {
    return this->yaw;
}

Actor& Actor::setPitch(const float pitch) {
    this->pitch = pitch;
    this->directionVectorsIsLatest = false;
    return *this;
}

float Actor::getPitch() {
    return this->pitch;
}

Actor& Actor::setRoll(const float roll) {
    this->roll = roll;
    this->directionVectorsIsLatest = false;
    return *this;
}

float Actor::getRoll() {
    return this->roll;
}

const glm::vec3& Actor::getDirectionVectorUp() {
    this->updateDirectionVectors();
    return this->directionVectorUp;
}

const glm::vec3& Actor::getDirectionVectorRight() {
    this->updateDirectionVectors();
    return this->directionVectorRight;
}

const glm::vec3& Actor::getDirectionVectorFront() {
    this->updateDirectionVectors();
    return this->directionVectorFront;
}

/* -------- һ�㷽���� -------- */

void Actor::move(float distance, const glm::vec3& direction) {
    auto movement = glm::normalize(direction);
    movement *= distance;
    this->position += movement;
}

/* -------- �Զ����߷����� -------- */

void Actor::updateDirectionVectors() {

    // todo: �ݲ�֧�� roll��

    if (this->directionVectorsIsLatest) {
        return;
    }

    this->directionVectorFront = glm::normalize(glm::vec3(
        // x
        cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw)),

        // y
        sin(glm::radians(this->pitch)),

        // z
        sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch))
    ));

    this->directionVectorRight = glm::normalize(glm::cross(
        this->directionVectorFront,
        glm::vec3(0.0f, 1.0f, 0.0f)
    ));

    this->directionVectorUp = glm::normalize(glm::cross(
        this->directionVectorRight, this->directionVectorFront
    ));

    this->directionVectorsIsLatest = true;

}
