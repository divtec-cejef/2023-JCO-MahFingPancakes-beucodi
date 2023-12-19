//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "body.h"

class Platform;
class JumpCharge;

class Player final : public Body
{
public:
    Player();
    void jump();
    void tick(long long elapsedTimeInMilliseconds) override;
    void updateJumpCharges();

private:
    //! Vecteurs d'entrée du joueur
    QPointF m_playerInput = QPointF(0, 0);
    //! Liste des touches pressées
    QList<int> m_keysPressed = {};
    //! Nombre de charges de saut maximum
    //! \note n'inclus pas le saut quand on touche le sol
    int m_maxJumpCharges = 2;
    //! Nombre de charges de saut restantes
    int m_jumpCharges = m_maxJumpCharges;
    //! Indique si le joueur a relâché la touche de saut
    bool m_hasReleasedJump = true;

    //! Vitesse horizontale maximale du joueur, en mètre par seconde.
    const qreal MAX_SPEED_X = 5;
    //! Accélération du joueur, en mètre par seconde au carré.
    const qreal PLAYER_ACCELERATION = 12;
    //! Vitesse de saut du joueur, en mètre par seconde.
    const qreal PLAYER_JUMP_FORCE = -7;
    //! Frottement du joueur, en mètre par seconde au carré.
    const qreal PLAYER_FRICTION = 0.5;
    //! Sprites de saut du joueur
    QList<JumpCharge*> m_pJumpChargesSprites = QList<JumpCharge*>();

    void die() const;

public slots:
    void keyPressed(int key);
    void keyReleased(int key);
};


#endif //PLAYER_H
