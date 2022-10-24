/*
 * ToyGraph ���档
 *
 * ������2022��9��21��
 * �������Ϻ��мζ�����ͤ��
 */
#include "ToyGraphCommon/EngineCompileOptions.h"
#include "ToyGraph/Engine.h"


using namespace std;

/* ------------ public. ------------ */

Engine& Engine::getInstance() {
    return engineInstance;
}

Engine Engine::engineInstance;

/* ------------ public. ------------ */

Texture* Engine::getLoadedTexture(const string& filepath) {
    if (this->texturesLoaded.count(filepath)) {
        return &this->texturesLoaded[filepath];
    } else {
        return nullptr;
    }
}
