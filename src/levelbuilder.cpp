//
// Created by Dråfølin on 19.12.2023.
//

#include "levelbuilder.h"

#include <QPoint>
#include <QRect>

#include "utilities.h"
#include "resources.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <player.h>
#include <regex>
#include <thread>

#include "door.h"
#include "fallingplatform.h"
#include "fragileplatform.h"
#include "solidplatform.h"
#include "transparentplatform.h"
#include "level.h"
#include "gamecore.h"


//! Constructeur de la classe LevelBuilder
//! \param levelId : position du niveau
LevelBuilder::LevelBuilder(QPoint levelId)
{
    m_levelId = levelId;
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
            auto entryType = line.substr(0, line.find('('));

            if (std::regex_search(entryType, std::regex("platform.*")))
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

                if (entryType == "platformSolid")
                    m_pSprites.append(new SolidPlatform(rect));
                else if (entryType == "platformFragile")
                    m_pSprites.append(new FragilePlatform(rect));
                else if (entryType == "platformTransparent")
                    m_pSprites.append(new TransparentPlatform(rect));
                else if (entryType == "platformFalling")
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

            if (entryType == "door")
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

                m_pSprites.append(new Door(spritePos, targetLevel, enterDirection));

                continue;
            }
            if (entryType == "spawnPoint")
            {
                m_spawnPoint = spritePos;
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
}

//! Construit le niveau
//! \param pCore : pointeur vers le GameCore
//! \param pPlayer : pointeur vers le joueur
//! \param enteredFrom : direction d'entrée dans le niveau
Level* LevelBuilder::build(const GameCore* pCore, Player* pPlayer, const GameFramework::Direction enteredFrom)
{
    if (m_pLevel != nullptr)
        return m_pLevel;

    m_pLevel = new Level(pCore->canvas(), pPlayer, m_levelId);
    const Door* connectedDoor = nullptr;
    for (const auto sprite : m_pSprites)
    {
        m_pLevel->scene()->addSpriteToScene(sprite);
        m_pLevel->scene()->registerSpriteForTick(sprite);
        if (const auto pPlatform = dynamic_cast<Platform*>(sprite))
            QObject::connect(pPlatform, &Platform::queuedForDeletion,
                             pCore, &GameCore::spriteQueuedForDeletion);
        if (const auto pDoor = dynamic_cast<Door*>(sprite))
        {
            QObject::connect(pDoor, &Door::doorEntered,
                             pCore, &GameCore::changeLevel);
            if (pCore->currentLevel() != nullptr && pDoor->targetLevel() == pCore->currentLevel()->levelId())
                connectedDoor = pDoor;
        }
    }
    m_pLevel->setSpawnPoint(m_spawnPoint);

    auto spawnPos = m_spawnPoint;
    if (connectedDoor != nullptr)
    {
        spawnPos = connectedDoor->pos();
        const int DOOR_WIDTH = connectedDoor->width();
        const int DOOR_HEIGHT = connectedDoor->height();
        constexpr int DOOR_SAFE_MARGIN = 5;

        switch (enteredFrom)
        {
        case GameFramework::LEFT:
            spawnPos.setX(spawnPos.x() + DOOR_WIDTH + DOOR_SAFE_MARGIN);
            spawnPos.setY(spawnPos.y() + DOOR_HEIGHT - pPlayer->sceneBoundingRect().height());
            break;
        case GameFramework::RIGHT:
            spawnPos.setX(spawnPos.x() - pPlayer->sceneBoundingRect().width() - DOOR_SAFE_MARGIN);
            spawnPos.setY(spawnPos.y() + DOOR_HEIGHT - pPlayer->sceneBoundingRect().height());
            break;
        case GameFramework::UP:
            spawnPos.setY(spawnPos.y() + DOOR_WIDTH + DOOR_SAFE_MARGIN);
            break;
        case GameFramework::DOWN:
            spawnPos.setY(spawnPos.y() - pPlayer->sceneBoundingRect().height() - DOOR_SAFE_MARGIN);
            break;
        case GameFramework::NEUTRAL:
            break;
        }
    }
    pPlayer->setPos(spawnPos);
    pPlayer->setVelocity(QPointF(0, 0));
    m_discoveryThread = new std::thread(&LevelBuilder::loadNeighbouringLevels, this);
    m_pSprites.clear();
    return m_pLevel;
}

//! Précharge les niveaux voisins
void LevelBuilder::loadNeighbouringLevels() const
{
    for (const auto sprite : m_pLevel->scene()->sprites())
    {
        if (const auto door = dynamic_cast<Door*>(sprite))
            m_pLevel->appendLevel(new LevelBuilder(door->targetLevel()));
    }
}

//! Accesseur pour la position du niveau
//! \returns la position du niveau
QPoint LevelBuilder::levelId() const
{
    return m_levelId;
}

//! Destructeur de la classe LevelBuilder
LevelBuilder::~LevelBuilder()
{
    if (m_discoveryThread != nullptr)
        m_discoveryThread->join();

    for (const auto sprite : m_pSprites)
    {
        delete sprite;
    }

    m_pSprites.clear();
}
