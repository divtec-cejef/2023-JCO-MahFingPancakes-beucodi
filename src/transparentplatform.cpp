//
// Created by Dråfølin on 05.12.2023.
//

#include "transparentplatform.h"

TransparentPlatform::TransparentPlatform(QRect rect): Platform(rect)
{
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    m_pImage->fill(Qt::red);
    setPixmap(QPixmap::fromImage(*m_pImage));
}
