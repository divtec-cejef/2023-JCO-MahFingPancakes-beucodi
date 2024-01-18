//
// Created by Dråfølin on 18.01.2024.
//

#include "regenpod.h"
#include "resources.h"
#include "utilities.h"

RegenPod::RegenPod() : Sprite(QString("%1/regen.png").arg(GameFramework::imagesPath())) {
    setScale(GameFramework::meterToPx(1) / width());
}

void RegenPod::tick(long long int elapsedTimeInMilliseconds) {
    Sprite::tick(elapsedTimeInMilliseconds);
    m_blinkCounter += (int) elapsedTimeInMilliseconds;
    if (m_blinkCounter > 1000)
        m_blinkCounter = 0;

    setOpacity(m_blinkCounter < 500 ? m_blinkCounter / 500.0 : (1000 - m_blinkCounter) / 500.0);
}
