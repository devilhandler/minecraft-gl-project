#pragma once

#include "Rogue/Scene/Entity.h"

namespace Rogue
{
	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}