/**
  \file
  \brief    Déclaration de la classe GameCore.
  \author   JCO
  \date     septembre 2018
*/
#ifndef GAMECORE_H
#define GAMECORE_H

#include <QObject>
#include <QPointF>
#include "utilities.h"

class GameCanvas;
class GameScene;
class Player;
class Platform;
class Sprite;
class Level;

//! \brief Classe qui gère la logique du jeu.
//!
//! Dans son état actuel, cette classe crée une scène vide, délimite
//! sa surface avec une ligne blanche puis démarre la cadence du jeu.
class GameCore : public QObject
{
    Q_OBJECT

public:
    explicit GameCore(GameCanvas* pGameCanvas, QObject* parent = nullptr);
    ~GameCore() override;

    void keyPressed(int key);
    void keyReleased(int key);

    void mouseMoved(QPointF newMousePosition);
    void mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);

    void tick(long long elapsedTimeInMilliseconds);

    [[nodiscard]] Level* currentLevel() const;
    [[nodiscard]] GameCanvas* canvas() const;

public slots:
    void changeLevel(QPoint targetLevel, GameFramework::Direction dir);

signals:
    void notifyMouseMoved(QPointF newMousePosition);
    void notifyMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyKeyPressed(int key);
    void notifyKeyReleased(int key);

private:
    GameCanvas* m_pGameCanvas = nullptr;
    Player* m_pPlayer = nullptr;
    QList<Platform*> m_pPlatforms = {};
    Level* m_pLevel = nullptr;

public slots:
    void spriteQueuedForDeletion(Sprite* pSprite);
    void onPlayerDied();
};


#endif // GAMECORE_H
