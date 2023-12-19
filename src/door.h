//
// Created by Dråfølin on 19.12.2023.
//

#ifndef DOOR_H
#define DOOR_H

#include "utilities.h"
#include "sprite.h"

class Level;

class Door final: public Sprite {
public:
    Door(QPoint pos, QPoint target, GameFramework::Direction dir, Level* pLevel);
    void travel() const;
    [[nodiscard]] QPoint targetLevel() const;

private:
    Level* m_pParentLevel = nullptr;
    QPoint m_targetLevel = QPoint(0, 0);
    QImage* m_pImage = nullptr;
    GameFramework::Direction m_dir = GameFramework::NEUTRAL;
};



#endif //DOOR_H
