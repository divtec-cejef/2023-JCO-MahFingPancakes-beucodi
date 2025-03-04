//
// Created by Dråfølin on 05.12.2023.
//

#include <QPainter>
#include "fallingplatform.h"

//! Constructeur de la classe FallingPlatform.
//! \param rect Rectangle de la plateforme.
FallingPlatform::FallingPlatform(QRect rect) : Platform(rect) {
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

    setPixmap(QPixmap::fromImage(*m_pImage));
}

//! Méthode de mise à jour de la plateforme.
//! \param elapsedTimeInMilliseconds Temps écoulé depuis la dernière mise à jour, en millisecondes.
void FallingPlatform::tick(const long long elapsedTimeInMilliseconds) {
    const auto deltaMs = static_cast<qreal>(elapsedTimeInMilliseconds);
    if (m_isFalling) {
        m_fallSpeed += GameFramework::meterToPx(GameFramework::GRAVITY) * deltaMs / 1000;
        setY(y() + m_fallSpeed * deltaMs / 1000);
    }

    if (y() > GameFramework::screenSize().height())
            emit queuedForDeletion(this);
}

//! Méthode de collision avec un autre corps.
//! \param body Corps avec lequel la plateforme est en collision.
//! \return Direction de la collision.
GameFramework::Direction FallingPlatform::collisionSide(Body *body) {
    const auto collisionSide = Platform::collisionSide(body);
    if (collisionSide == GameFramework::Direction::UP) {
        m_isFalling = true;
    }
    return collisionSide;
}
