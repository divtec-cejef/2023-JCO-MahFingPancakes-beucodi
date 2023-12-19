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

class Level final
{
public:
    explicit Level(QPoint levelId, GameCanvas* pCanvas);
    GameScene* loadLevel(Player* pPlayer, const GameCore* pCore, GameFramework::Direction enteredFrom);
    [[nodiscard]] QPoint levelId() const;

private:
    QPoint m_levelId = QPoint(0, 0);
    QPoint m_spawnPoint = QPoint(0, 0);
    GameCanvas* m_pCanvas = nullptr;
    GameScene* m_pScene = nullptr;
    QList<Sprite*> m_pSprites = QList<Sprite*>();
};


#endif //LEVEL_H
