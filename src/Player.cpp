//
// Created by Dråfølin on 14.11.2023.
//

#include "player.h"

#include "door.h"
#include "resources.h"
#include "gamescene.h"
#include "jumpcharge.h"

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
    if (pos().y() > m_pParentScene->height())
        die();

    for (const auto sprite : m_pParentScene->collidingSprites(sceneBoundingRect()))
    {
        if (const auto door = dynamic_cast<Door*>(sprite))
        {
            qDebug() << "Travelling";
            door->travel();
        }
    }

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

    // Réinitialise les charges quand sur le sol
    if (!isAirborne())
    {
        m_jumpCharges = m_maxJumpCharges;
        updateJumpCharges();
    }

    if (m_jumpCharges > 0 && m_keysPressed.contains(Qt::Key_Space) && m_hasReleasedJump)
        jump();

    if (isAirborne())
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
    updateJumpCharges();
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

//! Permet de mettre à jour les sprites de charges de saut
void Player::updateJumpCharges()
{
    if (m_pJumpChargesSprites.length() == m_jumpCharges)
        return;

    for (const auto sprite : m_pJumpChargesSprites)
    {
        if (!sprite)
            continue;
        m_pParentScene->removeSpriteFromScene(sprite);
        sprite->deleteLater();
    }

    m_pJumpChargesSprites.clear();

    while (m_pJumpChargesSprites.length() != m_jumpCharges)
    {
        const auto newSprite = new JumpCharge();
        const auto finalJumpBarWidth = newSprite->sceneBoundingRect().width() * m_maxJumpCharges;
        const auto jumpBarOffset = (m_pParentScene->width() - finalJumpBarWidth) / 2;
        newSprite->setPos(
            jumpBarOffset + m_pJumpChargesSprites.length() * newSprite->sceneBoundingRect().width(),
            m_pParentScene->height() - newSprite->sceneBoundingRect().height()
        );
        m_pParentScene->addSpriteToScene(newSprite);
        m_pJumpChargesSprites.append(newSprite);
    }
}

//! Permet de tuer le joueur
//! \note Fais planter le jeu pour l'instant
void Player::die() const
{
    delete m_pParentScene;
}

//! "Emplaqute" Le joueur; Permet de retirer tous les sprites de la scène générés par le joueur
void Player::pack()
{
    for (const auto sprite : m_pJumpChargesSprites)
    {
        m_pParentScene->removeSpriteFromScene(sprite);
        m_pParentScene->unregisterSpriteFromTick(sprite);
        sprite->deleteLater();
    }
    m_pJumpChargesSprites.clear();
    m_pParentScene->removeSpriteFromScene(this);
    m_pParentScene->unregisterSpriteFromTick(this);
}
