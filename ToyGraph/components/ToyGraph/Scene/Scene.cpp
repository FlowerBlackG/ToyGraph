/*

    ³¡¾°¡£
    part of the ToyGraph project

    ´´½¨£º2022.11.6

*/

#include <ToyGraph/Scene/Scene.h>

#include <ToyGraph/Actor.h>

#include <iostream>

using namespace std;

Scene::~Scene() {

    cout << "Scene::~Scene obj: " << this << endl;

    for (auto it : actors) {
        delete it.second;
    }
}

void Scene::tick(float deltaSeconds) {
    for (auto it : actors) {
        it.second->tick(deltaSeconds);
    }
}

void Scene::render() {
    for (auto it : actors) {
        it.second->render();
    }
}

void Scene::addActor(Actor* actor, int id) {
    if (id < 0) {
        id = actors.size();
    }

    actor->id = id;
    this->actors[id] = actor;
}

void Scene::removeActor(int id) {
    if (this->actors.count(id)) {
        delete this->actors[id];
        this->actors.erase(id);
    }
}
