//
// Created by Dråfølin on 19.12.2023.
//

#include "level.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "gamecore.h"

#include <fstream>
#include <iostream>
#include <player.h>
#include <string>
#include <sstream>
#include <regex>
#include <thread>

#include "door.h"
#include "fallingplatform.h"
#include "fragileplatform.h"
#include "solidplatform.h"
#include "transparentplatform.h"

constexpr int SCENE_WIDTH = 1280;

//! Constructeur de la classe Level
//! \param levelId : position du niveau
//! \param pCanvas : pointeur vers le canvas
Level::Level(QPoint levelId, GameCanvas* pCanvas)
{
    m_pCanvas = pCanvas;
    const auto levelPath = GameFramework::resourcesPath() +
        "/levels/" +
        QString::number(levelId.x()) + "-" +
        QString::number(levelId.y()) + ".lvl";
    std::ifstream levelFile;
    levelFile.open(levelPath.toStdString(), std::ios::in);
    auto lvlData = std::stringstream();
    if (levelFile.is_open())
    {
        std::string line;
        while (std::getline(levelFile, line))
        {
            auto spriteType = line.substr(0, line.find('('));

            if (std::regex_search(spriteType, std::regex("platform.*")))
            {
                auto spriteRect = line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1);
                QRect rect;
                rect.setX(std::stoi(spriteRect.substr(0, spriteRect.find(','))));
                spriteRect = spriteRect.substr(spriteRect.find(',') + 1, spriteRect.size() - spriteRect.find(',') - 1);
                rect.setY(std::stoi(spriteRect.substr(0, spriteRect.find(','))));
                spriteRect = spriteRect.substr(spriteRect.find(',') + 1, spriteRect.size() - spriteRect.find(',') - 1);
                rect.setWidth(std::stoi(spriteRect.substr(0, spriteRect.find(','))));
                spriteRect = spriteRect.substr(spriteRect.find(',') + 1, spriteRect.size() - spriteRect.find(',') - 1);
                rect.setHeight(std::stoi(spriteRect.substr(0, spriteRect.find(','))));

                if (spriteType == "platformSolid")
                    m_pSprites.append(new SolidPlatform(rect));
                else if (spriteType == "platformFragile")
                    m_pSprites.append(new FragilePlatform(rect));
                else if (spriteType == "platformTransparent")
                    m_pSprites.append(new TransparentPlatform(rect));
                else if (spriteType == "platformFalling")
                    m_pSprites.append(new FallingPlatform(rect));
                else
                    throw std::runtime_error("Unknown platform type");
                continue;
            }

            auto spritePoint = line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1);
            QPoint spritePos;
            spritePos.setX(std::stoi(spritePoint.substr(0, spritePoint.find(','))));
            spritePoint = spritePoint.substr(spritePoint.find(',') + 1, spritePoint.size() - spritePoint.find(',') - 1);
            auto ylimitPos = spritePoint.find(',');
            if (ylimitPos == std::string::npos)
                ylimitPos = spritePoint.find(')');
            spritePos.setY(std::stoi(spritePoint.substr(0, ylimitPos)));

            if (spriteType == "door")
            {
                QPoint targetLevel(0, 0);
                GameFramework::Direction enterDirection = GameFramework::NEUTRAL;
                spritePoint = spritePoint.substr(spritePoint.find(',') + 1,
                                                 spritePoint.size() - spritePoint.find(',') - 1);
                targetLevel.setX(std::stoi(spritePoint.substr(0, spritePoint.find(','))));
                spritePoint = spritePoint.substr(spritePoint.find(',') + 1,
                                                 spritePoint.size() - spritePoint.find(',') - 1);
                targetLevel.setY(std::stoi(spritePoint.substr(0, spritePoint.find(','))));
                spritePoint = spritePoint.substr(spritePoint.find(',') + 1,
                                                 spritePoint.size() - spritePoint.find(',') - 1);
                auto enterDirStr = spritePoint.substr(0, spritePoint.find(')'));
                if (enterDirStr == "left")
                    enterDirection = GameFramework::LEFT;
                else if (enterDirStr == "right")
                    enterDirection = GameFramework::RIGHT;
                else if (enterDirStr == "up")
                    enterDirection = GameFramework::UP;
                else if (enterDirStr == "down")
                    enterDirection = GameFramework::DOWN;

                m_pSprites.append(new Door(spritePos, targetLevel, enterDirection, this));

                continue;
            }
            throw std::runtime_error("Unknown sprite type");
        }
        levelFile.close();
    }
    else
    {
        std::cerr << "Unable to open file";
        throw std::runtime_error("Unable to open file");
    }
    qDebug() << "Level parsed.";
}

//! Charge le niveau
//! \param pPlayer : pointeur vers le joueur
//! \param pCore : pointeur vers le GameCore
//! \param enteredFrom : direction d'entrée dans le niveau
void Level::loadLevel(Player* pPlayer, GameCore* pCore, const GameFramework::Direction enteredFrom)
{
    m_loaded = true;
    m_pPlayer = pPlayer;
    m_pCore = pCore;
    m_pScene = m_pCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    m_pCanvas->setCurrentScene(m_pScene);
    if (m_pPlayer->parentScene() != nullptr)
    {
        m_pPlayer->parentScene()->unregisterSpriteFromTick(m_pPlayer);
        m_pPlayer->parentScene()->removeSpriteFromScene(m_pPlayer);
    }
    m_pScene->addSpriteToScene(m_pPlayer);
    m_pScene->registerSpriteForTick(m_pPlayer);
    m_pPlayer->updateJumpCharges();
    for (const auto sprite : m_pSprites)
    {
        m_pScene->addSpriteToScene(sprite);
        m_pScene->registerSpriteForTick(sprite);
        if (const auto pPlatform = dynamic_cast<Platform*>(sprite))
            connect(pPlatform, &Platform::queuedForDeletion,
                    pCore, &GameCore::spriteQueuedForDeletion);
    }
    m_pSprites.clear();

    switch (enteredFrom)
    {
    case GameFramework::LEFT:
        break;
    case GameFramework::RIGHT:
        break;
    case GameFramework::UP:
        break;
    case GameFramework::DOWN:
        break;
    case GameFramework::NEUTRAL:
        break;
    }
    m_pPlayer->setPos(0, 0);

    new std::thread(&Level::loadNeighbouringLevels, this);
}

//! Précharge les niveaux voisins
void Level::loadNeighbouringLevels()
{
    for (const auto sprite : m_pScene->sprites())
    {
        if (const auto door = dynamic_cast<Door*>(sprite))
            m_pConnectedLevels.append(new Level(door->targetLevel(), m_pCanvas));
    }
}


//! Retourne la position du niveau
//! \returns la position du niveau
QPoint Level::levelId() const
{
    return m_levelId;
}

//! Destructeur de la classe Level
Level::~Level()
{
    if (isLoaded())
    {
        for (const auto sprite : m_pScene->sprites())
        {
            m_pScene->unregisterSpriteFromTick(sprite);
            m_pScene->removeSpriteFromScene(sprite);
        }
        m_pScene->sprites().clear();
        delete m_pScene;
        m_pScene = nullptr;
    }

    for (const auto level : m_pConnectedLevels)
        if (!level->isLoaded())
            delete level;
}

//! Permet de changer de niveau
//! \param targetLevel : position du niveau cible
//! \param dir : direction d'entrée dans le niveau
void Level::changeLevel(const QPoint targetLevel, GameFramework::Direction dir)
{
    m_pCore->changeLevel(targetLevel, dir);
}

//! Liste les niveaux voisins
//! \returns la liste des niveaux voisins
QList<Level*> Level::neighbouringLevels() const
{
    return m_pConnectedLevels;
}

//! Accesseur pour l'état de chargement du niveau
//! \returns si le niveau est chargé
bool Level::isLoaded() const
{
    return m_loaded;
}

//! Accesseur pour la scène du niveau
//! \returns la scène du niveau
GameScene* Level::scene() const
{
    return m_pScene;
}
