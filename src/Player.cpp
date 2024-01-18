//
// Created by Dråfølin on 14.11.2023.
//

#include "player.h"

#include "door.h"
#include "enemy.h"
#include "resources.h"
#include "gamescene.h"
#include "jumpicon.h"
#include "mindsignal.h"
#include "item.h"
#include "jumpcharge.h"
#include "heartcharge.h"
#include "regenpod.h"

//! Constructeur de player
Player::Player() : Entity(GameFramework::imagesPath() + "/Ghost GIF Frames/frame_00_delay-0.03s.gif") {
    m_maxSpeedX = 3;
    m_maxHealth = 3;
    for (int i = 1; i < 39; i++) {
        QString currentI = "0" + QString::number(i);
        currentI = currentI.sliced(currentI.size() - 2, 2);
        addAnimationFrame(
                QString("%1/Ghost GIF Frames/frame_%2_delay-0.03s.gif")
                        .arg(GameFramework::imagesPath()).arg(currentI)
        );
    }
    setAnimationSpeed(100);
    setDebugModeEnabled(true);
    setScale(40.0 / width());
    startAnimation();
}

//! Permet d'initialiser le joueur, p.ex ajouter la barre de vie
void Player::initialize() {
    Entity::initialize();
    updateJumpIcons();
}

//! Permet de mettre a jour le joueur et d'appliquer ses physiques
//! \param elapsedTimeInMilliseconds temps écoulé depuis le dernier appel de cette fonction
void Player::tick(const long long elapsedTimeInMilliseconds) {
    if (m_invincibilityTimeLeft > 0) {
        setOpacity((m_invincibilityTimeLeft % 250) / 100.0 + .25);
        m_invincibilityTimeLeft -= static_cast<int>(elapsedTimeInMilliseconds);
    } else {
        setOpacity(1);
        manageEnemyCollisions();
    }

    if (pos().y() > m_pParentScene->height()) {
        takeDamage(1);
        resetPos();
    }

    for (const auto sprite: m_pParentScene->collidingSprites(sceneBoundingRect())) {
        if (const auto door = dynamic_cast<Door *>(sprite))
            door->travel();

        if (const auto item = dynamic_cast<Item *>(sprite)) {
            if (const auto jumpCharge = dynamic_cast<JumpCharge *>(item)) {
                m_maxJumpIcons++;
                m_jumpIcons++;
                updateJumpIcons();
            } else if (const auto healthCharge = dynamic_cast<HeartCharge * >(item)) {
                m_maxHealth++;
                m_health++;
                updateHealthBar();
            }
            m_pParentScene->removeSpriteFromScene(sprite);
            sprite->deleteLater();
            auto pickedUpUpgrades = data(PICKED_UP_UPGRADES).toList();
            pickedUpUpgrades.append(item->getId());
            setData(PICKED_UP_UPGRADES, pickedUpUpgrades);
        }

        if (dynamic_cast<RegenPod *>(sprite)) {
            m_health = m_maxHealth;
            updateHealthBar();
        }
    }

    // Inputs horizontaux du joueur (A&D)
    if (m_playerInput.x() > 0) {
        m_acceleration.setX(ACCELERATION);
    } else if (m_playerInput.x() < 0) {
        m_acceleration.setX(-ACCELERATION);
    } else {
        if (isAirborne())
            m_acceleration.setX(-m_velocity.x() * qSqrt(PLAYER_FRICTION));
        else
            m_acceleration.setX(-m_velocity.x() * PLAYER_FRICTION);
    }

    // Réinitialise les charges quand sur le sol
    if (!isAirborne()) {
        m_jumpIcons = m_maxJumpIcons;
        updateJumpIcons();
    }

    if (m_jumpIcons > 0 && m_keysPressed.contains(Qt::Key_Space) && m_hasReleasedJump)
        jump();

    if (isAirborne())
        computeGravity(elapsedTimeInMilliseconds);

    Body::tick(elapsedTimeInMilliseconds);
}

//! Permet de faire sauter le joueur
void Player::jump() {
    if (isAirborne())
        m_jumpIcons--;
    m_velocity.setY(PLAYER_JUMP_FORCE);
    m_hasReleasedJump = false;
    updateJumpIcons();
}

//! Permet de s'occuper et d'enregistrer les touches pressées
void Player::keyPressed(const int key) {
    m_keysPressed.append(key);
    switch (key) {
        case Qt::Key_A:
            m_playerInput.setX(m_playerInput.x() - 1);
            break;

        case Qt::Key_D:
            m_playerInput.setX(m_playerInput.x() + 1);
            break;

        default:
            break;
    }
}

//! Permet de s'occuper et d'enregistrer les touches relachées
void Player::keyReleased(const int key) {
    m_keysPressed.removeAll(key);
    switch (key) {
        case Qt::Key_A:
            m_playerInput.setX(m_playerInput.x() + 1);
            break;

        case Qt::Key_D:
            m_playerInput.setX(m_playerInput.x() - 1);
            break;

        case Qt::Key_Space:
            m_hasReleasedJump = true;
            break;

        default:
            break;
    }
}

//! Permet de mettre à jour les sprites de charges de saut
void Player::updateJumpIcons() {
    if (m_pJumpIconsSprites.length() == m_jumpIcons)
        return;

    for (const auto sprite: m_pJumpIconsSprites) {
        if (!sprite)
            continue;
        m_pParentScene->removeSpriteFromScene(sprite);
        sprite->deleteLater();
    }

    m_pJumpIconsSprites.clear();

    while (m_pJumpIconsSprites.length() != m_jumpIcons) {
        const auto newSprite = new JumpIcon();
        const auto finalJumpBarWidth = newSprite->sceneBoundingRect().width() * m_maxJumpIcons;
        const auto jumpBarOffset = (m_pParentScene->width() - finalJumpBarWidth) / 2;
        newSprite->setPos(
                jumpBarOffset +
                static_cast<qreal>(m_pJumpIconsSprites.length()) * newSprite->sceneBoundingRect().width(),
                m_pParentScene->height() - newSprite->sceneBoundingRect().height()
        );
        m_pParentScene->addSpriteToScene(newSprite);
        m_pJumpIconsSprites.append(newSprite);
    }
}

//! Permet de tuer le joueur
void Player::die() {
    emit playerDied();
}

//! Permet de faire subir des dégâts au joueur
void Player::takeDamage(int damage) {
    Entity::takeDamage(damage);
    m_invincibilityTimeLeft = INVINCIBILITY_TIME;
    qDebug() << "Player took" << damage << "damage";
}

//! "Emplaqute" Le joueur; Permet de retirer tous les sprites de la scène générés par le joueur
void Player::pack() {
    Entity::pack();
    for (const auto sprite: m_pJumpIconsSprites) {
        m_pParentScene->removeSpriteFromScene(sprite);
        m_pParentScene->unregisterSpriteFromTick(sprite);
        sprite->deleteLater();
    }
    m_pJumpIconsSprites.clear();

    for (const auto sprite: m_pAttackSprites) {
        m_pParentScene->removeSpriteFromScene(sprite);
        m_pParentScene->unregisterSpriteFromTick(sprite);
        sprite->deleteLater();
    }
    m_pAttackSprites.clear();
    m_pParentScene->removeSpriteFromScene(this);
    m_pParentScene->unregisterSpriteFromTick(this);
}

//! Permet de "déballer" le joueur; Permet de réinsérer tous les sprites de la scène qui appartiennent au joueur
void Player::unpack() {
    m_pParentScene->addSpriteToScene(this);
    Entity::unpack();
    m_pParentScene->registerSpriteForTick(this);
    updateJumpIcons();
}

//! Permet de réinitialiser la position du joueur au point d'apparition
void Player::resetPos() {
    setPos(m_spawnPoint);
    m_velocity = QPointF(0, 0);
    m_acceleration = QPointF(0, 0);
}

//! Permet de gérer les collisions avec les ennemis
void Player::manageEnemyCollisions() {
    for (const auto sprite: collidingSprites()) {
        if (const auto enemy = dynamic_cast<Enemy *>(sprite))
            takeDamage(enemy->getDamage());
    }
}

//! Permet de définir le point d'apparition du joueur
void Player::setSpawnPoint(const QPointF &spawnPoint) {
    m_spawnPoint = spawnPoint;
}

//! Permet de gérer les clics de souris, en l'occurence pour attaquer
void Player::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    if (buttons & Qt::LeftButton) {
        const auto newMindSignal = new MindSignal();
        auto center = sceneBoundingRect().size() / 2.0;
        newMindSignal->setPos(pos() + QPointF(center.width(), center.height()));
        newMindSignal->setDirection(mousePosition);
        m_pParentScene->addSpriteToScene(newMindSignal);
        m_pParentScene->registerSpriteForTick(newMindSignal);
        m_pAttackSprites.append(newMindSignal);
        connect(newMindSignal, &MindSignal::queueForDeletion, this, &Player::mindSignalRemoved);
    }
}

//! S'execute a la destruction d'un mind signal
//! \param obj le mind signal qui doit être détruit
void Player::mindSignalRemoved(MindSignal *obj) {
    m_pAttackSprites.removeAll(obj);
    m_pParentScene->removeSpriteFromScene(obj);
    obj->deleteLater();
}
