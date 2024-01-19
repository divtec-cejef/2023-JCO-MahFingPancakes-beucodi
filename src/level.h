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

class Level final : public QObject {
public:
    explicit Level(GameCanvas *pCanvas, Player *pPlayer, QPoint levelId);

    ~Level() override;

    [[nodiscard]] QList<LevelBuilder *> neighbouringLevels() const;

    [[nodiscard]] GameScene *scene() const;

    [[nodiscard]] QPoint levelId() const;

    void appendLevel(LevelBuilder *level);

    void setSpawnPoint(QPointF spawnPoint);

    void initialize() const;

private:
    //! L'identifiant du niveau, basé sur le nom du fichier
    QPoint m_levelId = QPoint(0, 0);
    //! Le canvas du jeu
    GameCanvas *m_pCanvas = nullptr;
    //! La scène du niveau
    GameScene *m_pScene = nullptr;
    //! Les niveaux voisins
    QList<LevelBuilder *> m_pConnectedLevels = QList<LevelBuilder *>();
    //! Le joueur
    Player *m_pPlayer = nullptr;
    //! Le point de spawn du joueur
    QPointF m_spawnPoint = QPointF(0, 0);
};


#endif //LEVEL_H
