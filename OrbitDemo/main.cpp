/*
    同济计算机系图形学课程作业2：行星模拟。
    by 2051565
*/

#include <ToyGraph/Engine.h>
#include <iostream>
#include <ToyGraph/Scene/SceneManager.h>
#include "./PlanetScene.h"

using namespace std;

const int SCR_WIDTH = 1400;
const int SCR_HEIGHT = 900;


int main(const int argc, const char* argv[]) {

    auto& runtime = AppRuntime::getInstance("orbit demo", SCR_WIDTH, SCR_HEIGHT);
    auto& sceneMgr = SceneManager::getInstance();
    sceneMgr.navigateTo(PlanetScene::constructor);
    runtime.run();

    return 0;
}
