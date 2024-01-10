//
// Created by Dråfølin on 19.12.2023.
//

#ifndef LEVEL_H
#define LEVEL_H

#include "gamecanvas.h"
#include "gamescene.h"
#include <QPoint>
#include "levelbuilder.h"

class QString;
class Player;

class Level final : public QObject
{
public:
    explicit Level(GameCanvas* pCanvas, Player* pPlayer, QPoint levelId);
    ~Level() override;
    [[nodiscard]] QList<LevelBuilder*> neighbouringLevels() const;
    [[nodiscard]] GameScene* scene() const;
    [[nodiscard]] QPoint levelId() const;
    void appendLevel(LevelBuilder* level);
    void setSpawnPoint(QPointF spawnPoint);
    void init() const;

private:
    QPoint m_levelId = QPoint(0, 0);
    GameCanvas* m_pCanvas = nullptr;
    GameScene* m_pScene = nullptr;
    QList<LevelBuilder*> m_pConnectedLevels = QList<LevelBuilder*>();
    Player* m_pPlayer = nullptr;
    QPointF m_spawnPoint = QPointF(0, 0);
};


#endif //LEVEL_H
