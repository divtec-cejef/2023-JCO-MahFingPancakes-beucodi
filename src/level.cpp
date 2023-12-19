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
#include <string>
#include <sstream>
#include <regex>

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

            std::regex platformPattern("platform.*");
            if(std::regex_search(spriteType, platformPattern))
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

                if(spriteType == "platformSolid")
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
            spritePos.setY(std::stoi(spritePoint.substr(0, spritePoint.find(','))));

            if(spriteType.empty())
                continue;
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

//! Charge le niveau
GameScene* Level::loadLevel(Player* pPlayer, const GameCore* pCore, const GameFramework::Direction enteredFrom)
{
    m_pScene = m_pCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    m_pCanvas->setCurrentScene(m_pScene);
    for (const auto sprite: m_pSprites)
    {
        m_pScene->addSpriteToScene(sprite);
        m_pScene->registerSpriteForTick(sprite);
        if(const auto pPlatform = dynamic_cast<Platform*>(sprite))
            QObject::connect(pPlatform, &Platform::queuedForDeletion,
                    pCore, &GameCore::spriteQueuedForDeletion);
    }

    switch(enteredFrom) {
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
    return m_pScene;
}

//! Retourne la position du niveau
//! \returns la position du niveau
QPoint Level::levelId() const
{
    return m_levelId;
}
