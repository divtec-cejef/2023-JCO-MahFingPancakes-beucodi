//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Platform;

class JumpIcon;

class MindSignal;

//! La classe Player représente le joueur.
class Player final : public Entity {
Q_OBJECT

public:
    Player();

    void jump();

    void tick(long long elapsedTimeInMilliseconds) override;

    void pack() override;

    void unpack() override;

    void initialize() override;

    void setSpawnPoint(const QPointF &spawnPoint);

    //! Liste des clés des données internes (Player::data()) du joueur
    enum PlayerData {
        PICKED_UP_UPGRADES
    };

private:
    //! Vecteurs d'entrée du joueur
    QPointF m_playerInput = QPointF(0, 0);
    //! Liste des touches pressées
    QList<int> m_keysPressed = {};
    //! Nombre de charges de saut maximum
    //! \note n'inclus pas le saut quand on touche le sol
    int m_maxJumpIcons = 0;
    //! Nombre de charges de saut restantes
    int m_jumpIcons = m_maxJumpIcons;
    //! Indique si le joueur a relâché la touche de saut
    bool m_hasReleasedJump = true;
    //! Référence le point d'apparition du niveau
    QPointF m_spawnPoint = QPointF(0, 0);
    //! Temps restant d'invincibilité du joueur après avoir pris des dégâts, en millisecondes
    int m_invincibilityTimeLeft = 0;
    //! Vitesse de saut du joueur, en mètre par seconde.
    constexpr static qreal PLAYER_JUMP_FORCE = -6;
    //! Accélération du joueur, en mètre par seconde au carré.
    constexpr static qreal ACCELERATION = 12;
    //! Frottement du joueur.
    constexpr static qreal PLAYER_FRICTION = 16;
    //! Sprites de saut du joueur
    QList<JumpIcon *> m_pJumpIconsSprites = QList<JumpIcon *>();
    //! Temps d'invincibilité du joueur après avoir pris des dégâts, en millisecondes
    constexpr static int INVINCIBILITY_TIME = 1000;
    //! Liste des sprites de projectiles
    QList<MindSignal *> m_pAttackSprites = QList<MindSignal *>();

    void die() override;

    void takeDamage(int damage) override;

    void updateJumpIcons();

    void resetPos();

    void manageEnemyCollisions();

public slots:

    void keyPressed(int key);

    void keyReleased(int key);

    void mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);

    void mindSignalRemoved(MindSignal *obj);

signals:

    void playerDied();
};


#endif //PLAYER_H
