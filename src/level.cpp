//
// Created by Dråfølin on 19.12.2023.
//

#include "level.h"

#include <ostream>

#include "gamecanvas.h"
#include "player.h"
#include "levelbuilder.h"
#include "door.h"
#include "jumpingpancake.h"

constexpr int SCENE_WIDTH = 1280;
constexpr int SCENE_HEIGHT = SCENE_WIDTH * 9 / 16;


//! Charge le niveau
//! \param pCanvas : pointeur vers le canvas
//! \param pPlayer : pointeur vers le joueur
//! \param levelId : position du niveau
Level::Level(GameCanvas* pCanvas, Player* pPlayer, QPoint levelId)
{
    m_levelId = levelId;
    m_pCanvas = pCanvas;
    m_pPlayer = pPlayer;
    m_pScene = m_pCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::white));
    m_pCanvas->setCurrentScene(m_pScene);
    m_pPlayer->setParentScene(m_pScene);
    m_pPlayer->unpack();
}

//! Destructeur de la classe Level
Level::~Level()
{
    for (const auto connectedLvl : m_pConnectedLevels)
    {
        delete connectedLvl;
    }
    m_pConnectedLevels.clear();

    for (const auto sprite : m_pScene->sprites())
    {
        m_pScene->unregisterSpriteFromTick(sprite);
        sprite->setParentScene(m_pScene);
    }

    m_pScene->deleteLater();
    m_pScene = nullptr;
}

//! Liste les niveaux voisins
//! \returns la liste des niveaux voisins
QList<LevelBuilder*> Level::neighbouringLevels() const
{
    return m_pConnectedLevels;
}

//! Accesseur pour la scène du niveau
//! \returns la scène du niveau
GameScene* Level::scene() const
{
    return m_pScene;
}

//! Ajoute un niveau à la liste des niveaux voisins
//! \param level : le niveau à ajouter
void Level::appendLevel(LevelBuilder* level)
{
    m_pConnectedLevels.append(level);
}

//! Accesseur pour l'id du niveau
//! \returns l'id du niveau
QPoint Level::levelId() const
{
    return m_levelId;
}

//! Permet de définir le point d'apparition par défaut du joueur
//! \param spawnPoint : le point d'apparition
void Level::setSpawnPoint(const QPointF spawnPoint)
{
    m_spawnPoint = spawnPoint;
    m_pPlayer->setSpawnPoint(m_spawnPoint);
}

//! Initialise le niveau
void Level::initialize() const
{
    for (const auto sprite : m_pScene->sprites())
    {
        if (const auto entity = dynamic_cast<Entity*>(sprite))
            entity->initialize();
    }
}
