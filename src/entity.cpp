//
// Created by Dråfølin on 20.12.2023.
//

#include "entity.h"

#include <resources.h>

#include "gamescene.h"
#include "hearticon.h"

//! Constructeur de la classe Entity
Entity::Entity(QString const& rImagePath, QGraphicsItem* pParent): Body(rImagePath, pParent)
{
    m_health = 0;
}

//! Permet d'initialiser l'entité, p.ex ajouter la barre de vie
void Entity::init()
{
    m_health = m_maxHealth;
    updateHealthBar();
}

//! Permet de mettre à jour la barre de vie de l'entité
void Entity::updateHealthBar()
{
    // Guard pour éviter les calculs inutiles
    if (m_pHealthBar.size() == m_health)
        return;

    // Itération de m_pHealthBar pour supprimer tous les sprites
    for (auto* pHeart : m_pHealthBar)
    {
        m_pParentScene->removeSpriteFromScene(pHeart);
        pHeart->deleteLater();
    }
    m_pHealthBar.clear();

    // Réinstantiation et insertion des sprites
    for (int i = 0; i < m_health; ++i)
    {
        const auto pHeart = new HeartIcon(this);
        m_pParentScene->addSpriteToScene(pHeart);
        QRectF sceneRect = pHeart->sceneBoundingRect();
        sceneRect.setWidth(sceneRect.width() * HeartIcon::SCALE);
        sceneRect.setHeight(sceneRect.height() * HeartIcon::SCALE);
        const auto OVERLAP_FACTOR = 3.0;
        const auto OFFSET_FOR_CENTER = sceneBoundingRect().width() / 2.0;

        pHeart->setPos(i * sceneRect.width() / OVERLAP_FACTOR + OFFSET_FOR_CENTER, -sceneRect.height());
        m_pHealthBar.append(pHeart);
    }
}

//! Permet de faire subir des dégâts à l'entité
//! \param damage : les dégâts à infliger
void Entity::takeDamage(const int damage)
{
    m_health -= damage;
    if (m_health <= 0)
        die();
    updateHealthBar();
}

//! Permet de faire mourir l'entité
void Entity::die()
{
    qInfo() << "Entity died";
}

//! Permet d'"empaqueter" l'entité, c'est-à-dire d'enlever tous les sprites qu'elle a instancié
void Entity::pack()
{
    for (auto* pHeart : m_pHealthBar)
    {
        m_pParentScene->removeSpriteFromScene(pHeart);
        pHeart->deleteLater();
    }
    m_pHealthBar.clear();
}
