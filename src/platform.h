//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLATFORM_H
#define PLATFORM_H

#include "sprite.h"
#include "utilities.h"

class Body;

class Platform : public Sprite {
Q_OBJECT;

public:
    virtual GameFramework::Direction collisionSide(Body *body);

    ~Platform() override;

signals:

    void queuedForDeletion(Platform *platform);

protected:
    explicit Platform(QRect rect);

    QImage *m_pImage = nullptr;
};


#endif //PLATFORM_H
