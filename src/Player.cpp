//
// Created by Dråfølin on 14.11.2023.
//

#include "player.h"
#include "resources.h"
#include "platform.h"

//! Constructeur de player
Player::Player(): Body(GameFramework::imagesPath() + "/Ghost GIF Frames/frame_00_delay-0.03s.gif")
{
    for (int i = 1; i < 39; i++)
    {
        QString currentI = "0" + QString::number(i);
        currentI = currentI.sliced(currentI.size() - 2, 2);
        addAnimationFrame(GameFramework::imagesPath() + "/Ghost GIF Frames/frame_" + currentI + "_delay-0.03s.gif");
    }
    setAnimationSpeed(100);
    setDebugModeEnabled(true);
    startAnimation();
}

//! Permet de mettre a jour le joueur et d'appliquer ses physiques
//! \param elapsedTimeInMilliseconds temps écoulé depuis le dernier appel de cette fonction
void Player::tick(const long long elapsedTimeInMilliseconds)
{
    // Inputs horizontaux du joueur (A&D)
    if (m_playerInput.x() > 0)
    {
        m_acceleration.setX(PLAYER_ACCELERATION);
    }
    else if (m_playerInput.x() < 0)
    {
        m_acceleration.setX(-PLAYER_ACCELERATION);
    }
    else
    {
        m_acceleration.setX(-m_velocity.x() / PLAYER_FRICTION);
    }

    // Saut du joueur; réinitialise les charges quand sur le sol
    if (isAirborne())
    {
        if (m_jumpCharges > 0 && m_keysPressed.contains(Qt::Key_Space) && m_hasReleasedJump)
            jump();
    }
    else
    {
        m_jumpCharges = m_maxJumpCharges;

        if (m_keysPressed.contains(Qt::Key_Space) && m_hasReleasedJump)
            jump();
    }

    computeGravity();
    Body::tick(elapsedTimeInMilliseconds);
}

//! Permet de faire sauter le joueur
void Player::jump()
{
    if (isAirborne())
        m_jumpCharges--;
    m_velocity.setY(PLAYER_JUMP_FORCE);
    m_hasReleasedJump = false;
}

//! Permet de s'occuper et d'enregistrer les touches pressées
void Player::keyPressed(const int key)
{
    m_keysPressed.append(key);
    switch (key)
    {
    case Qt::Key_A:
        m_playerInput.setX(m_playerInput.x() - 1);
        break;

    case Qt::Key_D:
        m_playerInput.setX(m_playerInput.x() + 1);
        break;

    default: break;
    }
}

//! Permet de s'occuper et d'enregistrer les touches relachées
void Player::keyReleased(const int key)
{
    m_keysPressed.removeAll(key);
    switch (key)
    {
    case Qt::Key_A:
        m_playerInput.setX(m_playerInput.x() + 1);
        break;

    case Qt::Key_D:
        m_playerInput.setX(m_playerInput.x() - 1);
        break;

    case Qt::Key_Space:
        m_hasReleasedJump = true;
        break;

    default: break;
    }
}
