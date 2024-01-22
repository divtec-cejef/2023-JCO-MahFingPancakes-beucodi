//
// Created by Dråfølin on 17.01.2024.
//

#ifndef MAHFINGPANCAKES_ITEM_H
#define MAHFINGPANCAKES_ITEM_H


#include "body.h"

//! \brief Classe générique pour les objets ramassables
class Item : public Body {
public:
    void tick(long long elapsedTimeInMilliseconds) override;

    [[nodiscard]] QString getId() const;

protected:
    Item(const QString &rImagePath, QString id, QGraphicsItem *parent = nullptr);

    //! Identifiant de l'objet ramassable. Devrait être unique pour chaque objet ramassable.
    QString m_id;

    //! Taille effective d'un objet ramassable, en mètre.
    static constexpr qreal m_size = 1.5;
};


#endif //MAHFINGPANCAKES_ITEM_H
