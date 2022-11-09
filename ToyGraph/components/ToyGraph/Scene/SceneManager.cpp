/*

    场景管理器。
    part of the ToyGraph project.

    创建：2022.11.6

*/

#include <ToyGraph/Scene/SceneManager.h>
#include <ToyGraph/Scene/Scene.h>
#include <ToyGraph/AppRuntime.h>
#include <functional>

#include <iostream>

using namespace std;

SceneManager& SceneManager::getInstance() {
    static SceneManager instance;
    return instance;
}

SceneManager::~SceneManager() {
    for (auto it : sceneStack) {
        delete it;
    }
}


Scene* SceneManager::navigateTo(function<class Scene* ()> constructor) {

    if (!this->sceneStack.empty()) {
        this->sceneStack.back()->onPause();
    }

    this->sceneStack.push_back(constructor());
    AppRuntime::getInstance();

    return this->sceneStack.back();
}

Scene* SceneManager::redirectTo(function<class Scene* ()> constructor) {
    Scene* scene = constructor();

    this->sceneStack.push_back(scene);
    return scene;
}
    
void SceneManager::navigateBack(int delta) {
    if (delta <= 0) {
        return;
    }

    if (delta >= this->sceneStack.size()) {
        delta = this->sceneStack.size() - 1;
    }

    Scene* target = sceneStack[sceneStack.size() - delta - 1];
    target->onReload();

    cout << "navi back: " << delta << endl;

    for (int counter = 0; counter < delta; counter++) {
        delete this->sceneStack.back();
        this->sceneStack.pop_back();
    }

}

// public
Scene* SceneManager::currentScene() {
    if (this->sceneStack.empty()) {
        return nullptr;
    }

    return this->sceneStack.back();
}
