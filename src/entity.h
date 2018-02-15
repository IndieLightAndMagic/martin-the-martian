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
#endif /*__ENTITY_H__*/

