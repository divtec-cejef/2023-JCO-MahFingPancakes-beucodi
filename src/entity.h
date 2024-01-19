//
// Created by Dråfølin on 20.12.2023.
//

#ifndef ENTITY_H
#define ENTITY_H

#include "body.h"

class HeartIcon;

//! La clase entité est une classe abstraite qui représente un object "vivant" dans le jeu.
//! Elle est utilisée pour les ennemis et le joueur.
class Entity : public Body {
public:
    virtual void takeDamage(int damage);

    virtual void pack();

    virtual void unpack();

    virtual void initialize();

protected:
    explicit Entity(const QString &rImagePath, QGraphicsItem *pParent = nullptr);

    virtual void die() = 0;

    //! Santé maximale de l'entité.
    int m_maxHealth = 0;
    //! Santé actuelle de l'entité.
    int m_health;
    //! Sprite de la barre de vie de l'entité
    QList<HeartIcon *> m_pHealthBar = QList<HeartIcon *>();

    void updateHealthBar();
};


#endif //ENTITY_H
