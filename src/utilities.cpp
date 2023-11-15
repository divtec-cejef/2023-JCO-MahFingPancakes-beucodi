#include "utilities.h"

#include <QApplication>
#include <QScreen>

namespace GameFramework {

    //! \return le rapport entre la largeur de l'écran et sa hauteur.
    double screenRatio() {
        return static_cast<double>(screenSize().width()) / screenSize().height();
    }

    //! \return la taille en pixels de l'écran.
    QSize screenSize() {
        //QDesktopWidget* pDefaultScreen = QApplication::desktop();
        QScreen* pDefaultScreen = QGuiApplication::screens().first();
        QRect ScreenRect = pDefaultScreen->geometry(); // pDefaultScreen->screenGeometry();
        return ScreenRect.size();
    }

    //! Cache le curseur de la souris.
    //! \see showMouseCursor()
    void hideMouseCursor() {
        qApp->setOverrideCursor(QCursor(Qt::BlankCursor));
    }

    //! Rend apparent le curseur de la souris.
    //! \see hideMouseCursor()
    void showMouseCursor() {
        qApp->restoreOverrideCursor();
    }

    //! Convertit une distance en mètres en une distance en pixels.
    //! \param meter Distance en mètres.
    qreal meterToPx(qreal meter)
    {
        return meter * METER_PX_RATIO;
    }

    //! Convertit une distance en pixels en une distance en mètres.
    //! \param px Distance en pixels.
    qreal pxToMeter(qreal px)
    {
        return px / METER_PX_RATIO;
    }
}
