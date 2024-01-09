//
// Created by Dråfølin on 20.12.2023.
//

#ifndef PANCAKE_H
#define PANCAKE_H

#include "enemy.h"

class Pancake final: public Enemy {
public:
    Pancake();
private:
    const int MAX_HEALTH = 1;
};



#endif //PANCAKE_H
