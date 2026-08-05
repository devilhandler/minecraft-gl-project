#include <repch.h>

#include "Rogue/Scene/Entity.h"

namespace Rogue
{
	Entity::Entity(entt::entity handle, Scene* scene) 
		: m_EntityHandle(handle), m_Scene(scene) 
	{
	
	}
}