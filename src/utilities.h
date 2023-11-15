/**
  \file
  \brief    Méthodes utilitaires diverses.
  \author   JCO
  \date     janvier 2015
  \version  1.0
*/
#ifndef UTILITIES_H
#define UTILITIES_H

#include <QSize>

//!
//! Espace de noms contenant les fonctions utilitaires.
//!
namespace GameFramework {
    double screenRatio();
    QSize screenSize();

    void hideMouseCursor();
    void showMouseCursor();

    //! Rapport entre un mètre et un pixel.
    constexpr qreal METER_PX_RATIO = 40;

    //! Constante de gravité, en mètre par seconde au carré.
    constexpr qreal GRAVITY = 9.81;

    qreal meterToPx(qreal meter);
    qreal pxToMeter(qreal px);

}
#endif // UTILITIES_H
