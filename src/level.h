//
// Created by Dråfølin on 19.12.2023.
//

#ifndef LEVEL_H
#define LEVEL_H

#include "gamecanvas.h"
#include "gamescene.h"
#include <QPoint>
#include <utilities.h>

class QString;
class Player;

class Level final : public QObject
{
public:
    explicit Level(QPoint levelId, GameCanvas* pCanvas);
    ~Level() override;
    void loadLevel(Player* pPlayer, GameCore* pCore, GameFramework::Direction enteredFrom);
    [[nodiscard]] QPoint levelId() const;
    void changeLevel(QPoint targetLevel, GameFramework::Direction dir);
    QList<Level*> neighbouringLevels() const;
    bool isLoaded() const;
    GameScene* scene() const;

private:
    QPoint m_levelId = QPoint(0, 0);
    QPoint m_spawnPoint = QPoint(0, 0);
    GameCanvas* m_pCanvas = nullptr;
    GameScene* m_pScene = nullptr;
    QList<Sprite*> m_pSprites = QList<Sprite*>();
    QList<Level*> m_pConnectedLevels = QList<Level*>();
    GameCore* m_pCore = nullptr;
    Player* m_pPlayer = nullptr;
    bool m_loaded = false;

    void loadNeighbouringLevels();
};


#endif //LEVEL_H
