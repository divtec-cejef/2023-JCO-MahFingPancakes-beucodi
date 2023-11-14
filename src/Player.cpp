//
// Created by Dråfølin on 14.11.2023.
//

#include "player.h"

#include "globals.h"

#include "resources.h"
#include "utilities.h"
#include "platform.h"

Player::Player(): Sprite(GameFramework::imagesPath() + "/player.png")
{
    setDebugModeEnabled(true);
}

bool Player::isAirborne() const
{
    auto colliding = collidingSprites();

    for (auto sprite : colliding)
    {
        if(auto platform = dynamic_cast<Platform*>(sprite))
        {
            return false;
        }
    }
    return true;
}


void Player::tick(qreal deltaMs)
{
    setPos(pos() + m_velocity * deltaMs / 1000);

    if(isAirborne())
    {
        m_velocity.setY(m_velocity.y() + Globals::GRAVITY * Globals::METER_PX_RATIO * deltaMs / 1000);
    } else
    {
        m_velocity.setY(0);
        auto colliding = collidingSprites();
        for (auto sprite : colliding)
        {
            if(auto platform = dynamic_cast<Platform*>(sprite))
            {
                setY(platform->top() - height() + 3);
            }
        }
    }
}

void Player::jump()
{
    m_velocity.setY(-Globals::GRAVITY * Globals::METER_PX_RATIO * 0.5);
}
