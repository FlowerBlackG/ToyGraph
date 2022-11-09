

#include <ToyGraph/AppRuntime.h>
#include <ToyGraph/Scene/SceneManager.h>
#include "./NahidaScene.h"

#include <iostream>

using namespace std;

const int SCR_WIDTH = 1800;
const int SCR_HEIGHT = 1200;

int main() {
    
    auto& runtime = AppRuntime::getInstance("scene demo", SCR_WIDTH, SCR_HEIGHT);

    auto& sceneMgr = SceneManager::getInstance();

    sceneMgr.navigateTo(NahidaScene::constructor);
    
    runtime.run();

    return 0;
}
