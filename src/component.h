#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "entity.h;"
class Entity;
struct Component
{
	Entity* m_pParent;
	unsigned int m_id;
};
#endif /*__COMPONENT_H__*/