//
// Created by Dråfølin on 19.12.2023.
//

#include "door.h"
#include "level.h"

//! Constructeur de la classe Door
//! \param pos : position de la porte
//! \param target : position du niveau cible
//! \param dir : direction de la porte
Door::Door(const QPoint pos, const QPoint target, const GameFramework::Direction dir)
{
    m_dir = dir;
    const QRect rect(pos, QSize(10, 64));
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    setPos(rect.x(), rect.y());
    m_targetLevel = target;
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    m_pImage->fill(Qt::blue);
    setPixmap(QPixmap::fromImage(*m_pImage));
}

//! Permet d'accéder à la position du niveau cible
//! \return position du niveau cible
QPoint Door::targetLevel() const
{
    return m_targetLevel;
}

//! Permet de changer de niveau
void Door::travel()
{
    emit doorEntered(m_targetLevel, m_dir);
}
