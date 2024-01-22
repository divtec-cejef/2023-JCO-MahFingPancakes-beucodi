//
// Created by Dråfølin on 12.12.2023.
//

#ifndef BODY_H
#define BODY_H

#include "sprite.h"

class Platform;

/**
 * @brief Classe représentant un corps physique.
 *
 * Un corps physique est un sprite qui peut se déplacer et interagir avec d'autres corps physiques.
 */
class Body : public Sprite {
public:
    void tick(long long elapsedTimeInMilliseconds) override;

    [[nodiscard]] bool isAirborne() const;

    explicit Body(const QString &rImagePath, QGraphicsItem *pParent = nullptr);

    [[nodiscard]] QPointF velocity() const;

    void setVelocity(const QPointF &velocity);

protected:
    //! Vitesse du corps, en mètre par seconde.
    QPointF m_velocity = QPointF(0, 0);
    //! Accélération du corps, en mètre par seconde au carré.
    QPointF m_acceleration = QPointF(0, 0);
    //! Vitesse horizontale maximale du corps, en mètre par seconde.
    qreal m_maxSpeedX = 5;
    //! Vitesse verticale maximale du corps, en mètre par seconde.
    qreal m_terminalVelocity = 100;

    void computeGravity(long long elapsedTimeInMilliseconds);

private:
    void collideWithPlatform(Platform *platform);
};


#endif //BODY_H
