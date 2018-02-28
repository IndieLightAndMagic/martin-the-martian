#ifndef __SHIPSPRITEFACTORY_H__
#define __SHIPSPRITEFACTORY_H__

#include "entity.h"

struct SpriteShipFactory {


    static UPEntity CreateDefaultShipEntity(GTech2D::Tech2D* ptech)
    {
        auto defaultShipEntity = EntityFactory::CreateEntity();
        auto defaultShipTexture = ptech->LoadTexture("hero.png");
        auto defaultSpriteComponent = UPSpriteComponent(new SpriteComponent(defaultShipEntity->m_id));
        defaultSpriteComponent->texture = defaultShipTexture;
        defaultShipEntity->m_components.push_back(defaultSpriteComponent);
        return defaultShipEntity;
    }

};


#endif /* __SHIPSPRITEFACTORY_H__ */