/*
    Ëæ»ú¡£
    ´´½¨£º2022.10.22
*/

#include <ToyGraphCommon/EngineCompileOptions.h>
#include <ToyGraphCommon/Random.h>
#include <cstdlib>

using namespace std;

int Random::randInt(int l, int r) {
    int realL = l;
    int realR = r;
    
    if (r < l) {
        realL = r;
        realR = l;
    }

    return rand() % (realR - realL + 1) + realL;
}
