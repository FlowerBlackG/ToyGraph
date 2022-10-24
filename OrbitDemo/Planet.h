/* 2051565 GTY */
/*
    ���ǡ�
    for �γ���ҵ��
*/

#pragma once

#include <ToyGraphCommon/EngineCompileOptions.h>
#include <ToyGraph/Engine.h>

class Planet : public Actor {
public:
    /** ���·�λ�� */
    void updateTransform(float deltaT);

public: 
    /** ����뾶�� */
    float orbitRadius = 0.0f;

    /** ��ת�ٶȡ���ÿ�롣 */
    float selfRotationSpeed = 0.0f;

    /** ��ת�ٶȡ���ÿ�롣 */
    float revolutionSpeed = 0.0f;

    /** ��ת�Ƕȡ� */
    float revolutionRotation = 0.0f;

    /** ��ת���ġ� */
    glm::vec3 rotationCenter = glm::vec3(0, 0, 0);

    /** ��ɫ�˾��� */
    glm::vec3 colorFilter;
};
