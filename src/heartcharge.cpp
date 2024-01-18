//
// Created by Dråfølin on 18.01.2024.
//

#include "heartcharge.h"

#include <utility>
#include "resources.h"

HeartCharge::HeartCharge(QString id) : Item(QString("%1/heart.png").arg(GameFramework::imagesPath()), std::move(id)) {

}
