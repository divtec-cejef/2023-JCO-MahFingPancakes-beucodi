//
// Created by Dråfølin on 17.01.2024.
//

#include "item.h"
#include "utilities.h"

#include <utility>

//! Constructeur de l'objet ramassable
//! \param rImagePath : Chemin vers l'image de l'objet ramassable
//! \param id : Identifiant de l'objet ramassable
//! \param parent : Objet parent
Item::Item(QString rImagePath, QString id, QGraphicsItem *parent)
        : Body(rImagePath, parent) {
    m_id = std::move(id);
    setScale(GameFramework::meterToPx(m_size) / sceneBoundingRect().width());
}

//! Cadance de l'objet ramassable
//! \param elapsedTimeInMilliseconds : Temps écoulé depuis le dernier appel à cette fonction
void Item::tick(long long int elapsedTimeInMilliseconds) {
    computeGravity(elapsedTimeInMilliseconds);
    Body::tick(elapsedTimeInMilliseconds);
}
