//
// Created by Dråfølin on 19.12.2023.
//

#ifndef LEVELBUILDER_H
#define LEVELBUILDER_H

#include "sprite.h"
#include "utilities.h"
#include <thread>

class QPoint;

class Level;

class GameCore;

class Player;

class LevelBuilder {
public:
    explicit LevelBuilder(QPoint levelId);

    ~LevelBuilder();

    Level *build(const GameCore *pCore, Player *pPlayer, GameFramework::Direction enteredFrom);

    [[nodiscard]] QPoint levelId() const;

private:
    QList<Sprite *> m_pSprites = QList<Sprite *>();
    QPoint m_levelId = QPoint(0, 0);

    void loadNeighbouringLevels() const;

    Level *m_pLevel = nullptr;
    QThread *m_discoveryThread = nullptr;;
    QPointF m_spawnPoint = QPointF(0, 0);
};


#endif //LEVELBUILDER_H
