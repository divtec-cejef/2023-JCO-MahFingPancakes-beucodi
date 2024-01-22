//
// Created by Dråfølin on 16.01.2024.
//

#ifndef MINDSIGNAL_H
#define MINDSIGNAL_H

#include "sprite.h"

//! \brief Classe représentant une attaque mentale
//!
//! Lorsque le joueur effectue une attaque mentale, il envoie une onde de choc
//! qui endommage les ennemis.
class MindSignal final : public Sprite {
Q_OBJECT

public:
    explicit MindSignal(QGraphicsItem *pParent = nullptr);

    void setDirection(QPointF goalPos);

    void tick(long long elapsedTimeInMilliseconds) override;

private:
    //! La direction de l'attaque
    QPointF m_deltaPos = QPointF(0, 0);
    //! La vitesse de l'attaque, en mètres par seconde
    static constexpr qreal ATTACK_SPREAD_SPEED = 20;
    //! La taille de l'attaque, en mètres
    static constexpr qreal ATTACK_SIZE = 0.5;
    //! La puissance de l'attaque
    static constexpr qreal ATTACK_POWER = 1;

signals:

    void queueForDeletion(MindSignal *);
};


#endif //MINDSIGNAL_H
