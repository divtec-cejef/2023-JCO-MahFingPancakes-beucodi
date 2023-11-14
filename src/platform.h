//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLATFORM_H
#define PLATFORM_H
#include "sprite.h"


class Platform: public Sprite {
public:
    Platform(QRect rect);

private:
    QImage* m_pImage = nullptr;
};



#endif //PLATFORM_H
