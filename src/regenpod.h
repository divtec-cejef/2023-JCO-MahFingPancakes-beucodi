//
// Created by Dråfølin on 18.01.2024.
//

#ifndef MAHFINGPANCAKES_REGENPOD_H
#define MAHFINGPANCAKES_REGENPOD_H


#include "sprite.h"

class RegenPod : public Sprite {
public:
    RegenPod();

    void tick(long long elapsedTimeInMilliseconds) override;

private:
    int m_blinkCounter = 0;
};


#endif //MAHFINGPANCAKES_REGENPOD_H
