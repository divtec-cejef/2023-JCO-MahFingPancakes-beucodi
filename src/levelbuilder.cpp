//
// Created by Dråfølin on 19.12.2023.
//

#include "levelbuilder.h"

#include <QPoint>
#include <QRect>
#include <QThread>

#include "utilities.h"
#include "resources.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <player.h>
#include <QPainter>
#include <regex>
#include <thread>

#include "door.h"
#include "enemy.h"
#include "fallingplatform.h"
#include "fragileplatform.h"
#include "solidplatform.h"
#include "transparentplatform.h"
#include "level.h"
#include "gamecore.h"
#include "jumpingpancake.h"
#include "jumpcharge.h"
#include "heartcharge.h"
#include "regenpod.h"
#include "finalboss.h"
#include "flyingpancake.h"


//! Constructeur de la classe LevelBuilder
//! \param levelId : position du niveau
LevelBuilder::LevelBuilder(QPoint levelId) {
    m_levelId = levelId;
    const auto levelPath = GameFramework::resourcesPath() +
                           "/levels/" +
                           QString::number(levelId.x()) + "-" +
                           QString::number(levelId.y()) + ".lvl";
    std::ifstream levelFile;
    levelFile.open(levelPath.toStdString(), std::ios::in);
    auto lvlData = std::stringstream();
    if (levelFile.is_open()) {
        std::string line;
        while (std::getline(levelFile, line)) {
            if (line.empty())
                continue;

	    if (std::regex_search(line, std::regex("^ *$")))
		continue;

            std::smatch spriteData;

            if (std::regex_search(line, spriteData,
                                  std::regex("\\/\\/.*")))
                continue;

            if (std::regex_search(line, spriteData,
                                  std::regex("platform([A-Za-z]*)\\(([0-9]*), ?([0-9]*), ?([0-9]*), ?([0-9]*)\\)"))) {
                std::string platformType = spriteData[1].str();
                QRect rect = {
                        stoi(spriteData[2].str()),
                        stoi(spriteData[3].str()),
                        stoi(spriteData[4].str()),
                        stoi(spriteData[5].str())
                };

                if (platformType == "Solid")
                    m_pSprites.append(new SolidPlatform(rect));
                else if (platformType == "Fragile")
                    m_pSprites.append(new FragilePlatform(rect));
                else if (platformType == "Transparent")
                    m_pSprites.append(new TransparentPlatform(rect));
                else if (platformType == "Falling")
                    m_pSprites.append(new FallingPlatform(rect));
                else
                    throw std::runtime_error("Unknown platform type");
                continue;
            }

            if (std::regex_search(line, spriteData,
                                  std::regex(
                                          "door\\(([0-9]*), ?([0-9]*), ?([0-9\\-]*), ?([0-9\\-]*), ?(right|left|up|down)\\)"))) {
                QPoint pos(
                        stoi(spriteData[1].str()),
                        stoi(spriteData[2].str())
                );
                QPoint targetLevel(
                        stoi(spriteData[3].str()),
                        stoi(spriteData[4].str())
                );

                auto enterDirStr = spriteData[5].str();
                GameFramework::Direction enterDirection;

                if (enterDirStr == "left")
                    enterDirection = GameFramework::LEFT;
                else if (enterDirStr == "right")
                    enterDirection = GameFramework::RIGHT;
                else if (enterDirStr == "up")
                    enterDirection = GameFramework::UP;
                else if (enterDirStr == "down")
                    enterDirection = GameFramework::DOWN;
                else
                    enterDirection = GameFramework::NEUTRAL;

                m_pSprites.append(new Door(pos, targetLevel, enterDirection));

                continue;
            }

            if (std::regex_search(line, spriteData,
                                  std::regex("spawnPoint\\(([0-9]*), ?([0-9]*)\\)"))) {
                m_spawnPoint = {
                        stod(spriteData[1].str()),
                        stod(spriteData[2].str())
                };
                continue;
            }

            if (std::regex_search(line, spriteData, std::regex("enemy([A-Za-z]*)\\(([0-9]*), ?([0-9]*)\\)"))) {
                QPoint pos(
                        stoi(spriteData[2].str()),
                        stoi(spriteData[3].str())
                );
                std::string enemyType = spriteData[1].str();
                if (enemyType == "Jmp")
                    m_pSprites.append(new JumpingPancake(pos));
                else if (enemyType == "Boss")
                    m_pSprites.append(new FinalBoss(pos));
		else if (enemyType == "Fly")
		    m_pSprites.append(new FlyingPancake(pos));
                else
                    throw std::runtime_error("unknown enemy type");
                continue;
            }

            if (std::regex_search(line, spriteData, std::regex(
                    "sprite\\(([0-9]*),([0-9]*), ?([0-9\\.]*), ?\"([a-zA-Z0-9\\/\\-\\._]*)\"\\)"))) {
                QPoint pos(
                        stoi(spriteData[1].str()),
                        stoi(spriteData[2].str())
                );
                QString spritePath(spriteData[4].str().c_str());
                float scale = stof(spriteData[3].str());
                auto newSprite = new Sprite(
                        QString("%1/%2")
                                .arg(GameFramework::imagesPath())
                                .arg(spritePath)
                );
                newSprite->setScale(scale);
                newSprite->setOffset(-newSprite->sceneBoundingRect().width() / 2,
                                     -newSprite->sceneBoundingRect().height() / 2);
                newSprite->setPos(pos);
                newSprite->setZValue(-3);
                m_pSprites.append(newSprite);
                continue;
            }

            if (std::regex_search(line, spriteData, std::regex("text\\(([0-9]*), ?([0-9]*), ?([0-9]*), ?\"(.*)\"\\)"))) {
                QPoint pos(
                        stoi(spriteData[1].str()),
                        stoi(spriteData[2].str())
                );
                int fontSize = stoi(spriteData[3].str());
                QPixmap pix(500, 200);
                pix.fill(QColorConstants::Transparent);
                QPainter painter(&pix);
                painter.setFont(QFont("Arial", fontSize));
                painter.setPen(QPen(QColorConstants::White));
                painter.drawText(QRectF(1, 1 + fontSize, 500, 200), Qt::AlignCenter, spriteData[4].str().c_str());
                auto newSprite = new Sprite(QPixmap::fromImage(pix.toImage().convertToFormat(QImage::Format_ARGB32)));
                newSprite->setOffset(-newSprite->boundingRect().width() / 2, -newSprite->boundingRect().height() / 2);
                newSprite->setZValue(-4);
                newSprite->setPos(pos);
                m_pSprites.append(newSprite);
                continue;
            }

            if (std::regex_search(line, spriteData, std::regex("item([A-Za-z]*)\\(([0-9]*), ?([0-9]*), ?\"(.*)\"\\)"))) {
                QString itemType(spriteData[1].str().c_str());
                QPoint pos(
                        stoi(spriteData[2].str()),
                        stoi(spriteData[3].str())
                );
                QString itemId(spriteData[4].str().c_str());

                if (itemType == "Jmp") {
                    auto newItem = new JumpCharge(itemId);
                    newItem->setPos(pos);
                    m_pSprites.append(newItem);
                } else if (itemType == "Hlth") {
                    auto newItem = new HeartCharge(itemId);
                    newItem->setPos(pos);
                    m_pSprites.append(newItem);
                } else {
                    throw std::runtime_error("Unknown item type");
                }
                continue;
            }

            if (std::regex_search(line, spriteData, std::regex("regenPod\\(([0-9]*), ?([0-9]*)\\)"))) {
                QPoint pos(
                        stoi(spriteData[1].str()),
                        stoi(spriteData[2].str())
                );

                auto newSprite = new RegenPod();
                newSprite->setPos(pos);
                newSprite->setZValue(-4);
                m_pSprites.append(newSprite);

                continue;
            }
            qDebug() << "Couldn't parse element with content:\n"
                     << line;
            throw std::runtime_error("Unknown sprite type");
        }
        levelFile.close();
    } else {
        std::cerr << "Unable to open file";
        throw std::runtime_error("Unable to open file");
    }
}

//! Construit le niveau
//! \param pCore : pointeur vers le GameCore
//! \param pPlayer : pointeur vers le joueur
//! \param enteredFrom : direction d'entrée dans le niveau
Level *LevelBuilder::build(const GameCore *pCore, Player *pPlayer, const GameFramework::Direction enteredFrom) {
    if (m_pLevel != nullptr)
        return m_pLevel;

    m_pLevel = new Level(pCore->canvas(), pPlayer, m_levelId);
    const Door *connectedDoor = nullptr;
    for (const auto sprite: m_pSprites) {
        if (const auto item = dynamic_cast<Item *>(sprite)) {
            if (pPlayer->data(Player::PICKED_UP_UPGRADES).toList().contains(item->getId())) {
                delete item;
                continue;
            }
        }
        m_pLevel->scene()->addSpriteToScene(sprite);
        m_pLevel->scene()->registerSpriteForTick(sprite);
        if (const auto pPlatform = dynamic_cast<Platform *>(sprite))
            QObject::connect(pPlatform, &Platform::queuedForDeletion,
                             pCore, &GameCore::spriteQueuedForDeletion);
        if (const auto pDoor = dynamic_cast<Door *>(sprite)) {
            QObject::connect(pDoor, &Door::doorEntered,
                             pCore, &GameCore::changeLevel);
            // Dernière condition requise car NEUTRAL ne peut jamais se produire quand
            // on utilise une porte, uniquement si on est mort ou on recharge le jeu.
            // or, si on recharge le jeu, on ne veut pas que le joueur se repositionne
            // à la porte connectée
            if (
                    pCore->currentLevel() != nullptr &&
                    pDoor->targetLevel() == pCore->currentLevel()->levelId() &&
                    enteredFrom != GameFramework::NEUTRAL
                    )
                connectedDoor = pDoor;
        }

        if (const auto pEnemy = dynamic_cast<Enemy *>(sprite))
            pEnemy->linkPlayer(pPlayer);
    }
    m_pLevel->setSpawnPoint(m_spawnPoint);

    auto spawnPos = m_spawnPoint;
    if (connectedDoor != nullptr) {
        spawnPos = connectedDoor->pos();
        const int DOOR_WIDTH = connectedDoor->width();
        const int DOOR_HEIGHT = connectedDoor->height();
        constexpr int DOOR_SAFE_MARGIN = 5;

        switch (enteredFrom) {
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
    m_discoveryThread = QThread::create([this] { loadNeighbouringLevels(); });
    m_discoveryThread->start();
    m_pSprites.clear();
    return m_pLevel;
}

//! Précharge les niveaux voisins
void LevelBuilder::loadNeighbouringLevels() const {
    for (const auto sprite: m_pLevel->scene()->sprites()) {
        if (const auto door = dynamic_cast<Door *>(sprite))
            m_pLevel->appendLevel(new LevelBuilder(door->targetLevel()));
    }
}

//! Accesseur pour la position du niveau
//! \returns la position du niveau
QPoint LevelBuilder::levelId() const {
    return m_levelId;
}

//! Destructeur de la classe LevelBuilder
LevelBuilder::~LevelBuilder() {
    if (m_discoveryThread != nullptr)
        m_discoveryThread->wait();

    for (const auto sprite: m_pSprites) {
        delete sprite;
    }

    m_pSprites.clear();
}
