//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Platform;
class JumpCharge;

//! La classe Player représente le joueur.
class Player final : public Entity
{
    Q_OBJECT

public:
    Player();
    void jump();
    void tick(long long elapsedTimeInMilliseconds) override;
    void pack() override;
    void unpack() override;
    void initialize() override;
    void setSpawnPoint(const QPointF& spawnPoint);

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
    //! Référence le point d'apparition du niveau
    QPointF m_spawnPoint = QPointF(0, 0);
    //! Temps restant d'invincibilité du joueur après avoir pris des dégâts, en millisecondes
    int m_invincibilityTimeLeft = 0;

    //! Vitesse horizontale maximale du joueur, en mètre par seconde.
    const qreal MAX_SPEED_X = 5;
    //! Vitesse de saut du joueur, en mètre par seconde.
    const qreal PLAYER_JUMP_FORCE = -7;
    //! Accélération du joueur, en mètre par seconde au carré.
    const qreal ACCELERATION = 12;
    //! Frottement du joueur, en mètre par seconde au carré.
    const qreal PLAYER_FRICTION = 0.5;
    //! Sprites de saut du joueur
    QList<JumpCharge*> m_pJumpChargesSprites = QList<JumpCharge*>();
    //! Temps d'invincibilité du joueur après avoir pris des dégâts, en millisecondes
    const int INVINCIBILITY_TIME = 1000;

    void die() override;
    void takeDamage(int damage) override;
    void updateJumpCharges();
    void resetPos();
    void manageEnemyCollisions();

public slots:
    void keyPressed(int key);
    void keyReleased(int key);

signals:
    void playerDied();
};


#endif //PLAYER_H
