#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "component.h"

#include <vector>
#include <memory>
class Component;
class Entity
{
public:
	unsigned int m_id;
	std::vector<std::unique_ptr<Component> > m_components;

};
using UPEntity = std::unique_ptr<Entity>;
class EntityFactory {
public:
	static unsigned int m_int;
	static UPEntity CreateEntity(){
		auto pEnt = UPEntity(new Entity());
		pEnt->m_id = ++m_int;
		return pEnt;
	}
};
unsigned int EntityFactory::m_int = 0;

#endif /*__ENTITY_H__*/

