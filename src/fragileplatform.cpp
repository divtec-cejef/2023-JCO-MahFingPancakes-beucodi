//
// Created by Dråfølin on 12.12.2023.
//

#include "fragileplatform.h"

#include <gamescene.h>
#include <QPainter>
#include <QRandomGenerator>

#include "body.h"

//! Constructeur de la classe FragilePlatform
//! \param rect rectangle de la plateforme
FragilePlatform::FragilePlatform(const QRect rect) : Platform(rect) {
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

    for (auto y = 0; y < rect.height(); y++) {
        auto line = (QRgba64 *) m_pImage->scanLine(y);
        for (auto x = 0; x < rect.width(); x++) {
            // 20 % de chance de dessiner la rouille
            qDebug() << QRandomGenerator::global()->bounded(0, 100);
            if (QRandomGenerator::global()->bounded(0, 100) > 20)
                continue;

            QRgba64 &rgb = line[x];
            rgb = QRgba64::fromRgba(
                    124,
                    82,
                    59,
                    255
            );
        }
    }

    setPixmap(QPixmap::fromImage(*m_pImage));
}

//! Retourne la direction dans la quelle le joueur touche la plate-forme, du point de vue de celle-ci
//! Active en plus le fait que la plateforme se brise
//! \param body le joueur
GameFramework::Direction FragilePlatform::collisionSide(Body *body) {
    const auto detected = Platform::collisionSide(body);
    if (detected != GameFramework::UP)
        return detected;

    m_isBreaking = true;

    return GameFramework::NEUTRAL;
}

//! Met à jour la plateforme
//! \param elapsedTimeInMilliseconds temps écoulé depuis la dernière mise à jour
void FragilePlatform::tick(long long elapsedTimeInMilliseconds) {
    if (m_isBreaking) {
        m_pImage->fill(QColor(0, 255, 255, m_breakingState));
        setPixmap(QPixmap::fromImage(*m_pImage));
        m_breakingState -= 20;
    }

    if (m_breakingState <= 0) {
        m_isBreaking = false;
        emit queuedForDeletion(this);
    }
}
