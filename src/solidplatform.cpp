//
// Created by Dråfølin on 05.12.2023.
//

#include <QPainter>
#include "solidplatform.h"

//! Constructeur de la classe SolidPlatform
SolidPlatform::SolidPlatform(const QRect rect) : Platform(rect) {
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
            QRect(
                    boltOffset,
                    rect.height() - boltOffset - boltSize,
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
    painter.fillRect(
            QRect(rect.width() - boltOffset - boltSize,
                  rect.height() - boltOffset - boltSize,
                  boltSize,
                  boltSize
            ),
            detailColor
    );
    setPixmap(QPixmap::fromImage(*m_pImage));
}
