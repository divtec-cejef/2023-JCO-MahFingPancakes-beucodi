//
// Created by Dråfølin on 19.12.2023.
//

#include "door.h"
#include "level.h"
#include <QRgba64>
#include "enemy.h"
#include "player.h"

//! Constructeur de la classe Door
//! \param pos : position de la porte
//! \param target : position du niveau cible
//! \param dir : direction de la porte
Door::Door(const QPoint pos, const QPoint target, const GameFramework::Direction dir) {
    m_dir = dir;
    const QRect rect(pos, QSize(10, 64));
    setPos(rect.x(), rect.y());
    m_targetLevel = target;
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_RGBA64);
    m_pImage->fill(Qt::transparent);
    for (auto y = 0; y < rect.height(); y++) {
        auto *line = reinterpret_cast<QRgba64 *>(m_pImage->scanLine(y));
        for (auto x = 0; x < rect.width(); x++) {
            QRgba64 &rgb = line[x];
            rgb = QRgba64::fromRgba(
                    148,
                    148,
                    148,
                    255.0
            );
        }
    }
    setPixmap(QPixmap::fromImage(*m_pImage));
    setTransformOriginPoint(QPointF(rect.width() / 2.0, rect.height() / 2.0));

    switch (dir) {
        case GameFramework::LEFT:
            setRotation(0);
            break;
        case GameFramework::RIGHT:
            setRotation(180);
            break;
        case GameFramework::UP:
            setRotation(90);
            break;
        case GameFramework::DOWN:
            setRotation(270);
            break;
        default:
            break;
    }
}

//! Permet d'accéder à la position du niveau cible
//! \return position du niveau cible
QPoint Door::targetLevel() const {
    return m_targetLevel;
}

//! Permet de changer de niveau
void Door::travel() {
    emit doorEntered(m_targetLevel, m_dir);
}

//! Cadence de la porte
//! \param elapsedTimeInMilliseconds : Temps depuis le dernier appel à cette fonction
void Door::tick(long long elapsedTimeInMilliseconds) {
    auto anyEnemyLeft = false;
    for (auto sprite: parentScene()->sprites()) {
    	if (dynamic_cast<Enemy*>(sprite)) {
	    anyEnemyLeft = true;
	    break;
	}
    }

    if(!anyEnemyLeft && !m_isDoorEnabled) {
        m_isDoorEnabled = true;
	for (auto y = 0; y < m_pImage->height(); y++) {
            auto *line = reinterpret_cast<QRgba64 *>(m_pImage->scanLine(y));
            for (auto x = 0; x < m_pImage->width(); x++) {
                QRgba64 &rgb = line[x];
                rgb = QRgba64::fromRgba(
                    255,
                    255,
                    255,
                    static_cast<quint8>((x / static_cast<qreal>(m_pImage->width())) * 255.0)
                );
            }
        }
	setPixmap(QPixmap::fromImage(*m_pImage));
    }
}

//! Définit si la porte est traversable ou non
bool Door::isTravellable() const {
    return m_isDoorEnabled;
}
