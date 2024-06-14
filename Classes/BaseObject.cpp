//
// Created by 蔡忱儒 on 2024/6/11.
//

#include "BaseObject.h"

bool BaseObject::init()
{
    if (!Sprite::init())
    {
        return false;
    }
    return true;
}
