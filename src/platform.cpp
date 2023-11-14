//
// Created by Dråfølin on 14.11.2023.
//

#include "platform.h"
#include "sprite.h"

Platform::Platform(QRect rect) {
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    m_pImage->fill(Qt::green);
    setPixmap(QPixmap::fromImage(*m_pImage));
}
