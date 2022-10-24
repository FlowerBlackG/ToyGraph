/*
 * ToyGraph 引擎。
 *
 * 创建于2022年9月21日
 * 创建于上海市嘉定区安亭镇
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
