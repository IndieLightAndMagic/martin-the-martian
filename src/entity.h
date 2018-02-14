#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "component.h"

#include <vector>

class Component;
class Entity
{
	unsigned int m_id;
	std::vector<Componet*> m_components;
};
#endif /*__ENTITY_H__*/

