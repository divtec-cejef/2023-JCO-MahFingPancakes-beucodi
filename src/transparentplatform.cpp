//
// Created by Dråfølin on 05.12.2023.
//

#include <QPainter>
#include "transparentplatform.h"
#include "body.h"

//! Constructeur de la classe TransparentPlatform
TransparentPlatform::TransparentPlatform(const QRect rect) : Platform(rect) {
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_RGBA64);
    m_pImage->fill(QColor::fromRgb(
            148,
            148,
            148
    ));
    QPainter painter(m_pImage);
    auto detailColor = QColor::fromRgb(
            80,
            80,
            80
    );
    painter.setPen(detailColor);
    painter.drawRect(0, 0, rect.width() - 1, rect.height() - 1);

    const int boltSize = 4;
    const int boltOffset = 3;

    // Bolts
    painter.fillRect(
            QRect(
                    boltOffset,
                    boltOffset,
                    boltSize,
                    boltSize
            ),
            detailColor
    );
    painter.fillRect(
            QRect(rect.width() - boltOffset - boltSize,
                  boltOffset,
                  boltSize,
                  boltSize
            ),
            detailColor
    );
    setPixmap(QPixmap::fromImage(*m_pImage));
}

//! Permet de définir quel côté de la plateforme est en collision avec le corps
//! \param body : pointeur vers le corps avec lequel la plateforme est en collision
//! \return le côté de la plateforme en collision avec le corps
GameFramework::Direction TransparentPlatform::collisionSide(Body *body) {
    if (Platform::collisionSide(body) != GameFramework::UP)
        return GameFramework::NEUTRAL;

    // Relatif à la plateforme
    const int overlapTop = body->bottom() - top();
    return overlapTop > (body->velocity().y() + 1) ? GameFramework::NEUTRAL : GameFramework::UP;
}
