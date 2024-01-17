//
// Created by Dråfølin on 17.01.2024.
//

#include "jumpcharge.h"
#include "resources.h"

JumpCharge::JumpCharge(QString id) : Item(QString("%1/jumpcharge.png").arg(GameFramework::imagesPath()), id) {

}
