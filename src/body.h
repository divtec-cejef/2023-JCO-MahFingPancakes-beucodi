//
// Created by Dråfølin on 12.12.2023.
//

#ifndef BODY_H
#define BODY_H

#include "sprite.h"

class Platform;

class Body: public Sprite {
public:
    virtual void tick(long long elapsedTimeInMilliseconds);
    bool isAirborne() const;
    Body(const QString& rImagePath, QGraphicsItem* pParent = nullptr);
    QPointF velocity() const;

protected:
    //! Vitesse du corps, en mètre par seconde.
    QPointF m_velocity = QPointF(0, 0);
    //! Accélération du corps, en mètre par seconde au carré.
    QPointF m_acceleration = QPointF(0, 0);
    const qreal MAX_SPEED_X = 5;
    const qreal TERMINAL_VELOCITY = 100;
    void computeGravity();

private:
    void collideWithPlatform(Platform* platform);
};



#endif //BODY_H
