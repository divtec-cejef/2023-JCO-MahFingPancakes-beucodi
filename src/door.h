//
// Created by Dråfølin on 19.12.2023.
//

#ifndef DOOR_H
#define DOOR_H

#include "utilities.h"
#include "sprite.h"

class Level;

/**
 * @brief Classe représentant une porte.
 *
 * Une porte est un sprite qui permet de changer de niveau.
 */
class Door final : public Sprite {
Q_OBJECT

public:
    Door(QPoint pos, QPoint target, GameFramework::Direction dir);

    void travel();

    [[nodiscard]] QPoint targetLevel() const;

signals:

    void doorEntered(QPoint targetLevel, GameFramework::Direction dir);

private:
    QPoint m_targetLevel = QPoint(0, 0);
    QImage *m_pImage = nullptr;
    GameFramework::Direction m_dir = GameFramework::NEUTRAL;
};


#endif //DOOR_H
