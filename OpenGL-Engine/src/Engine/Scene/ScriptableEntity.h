#pragma once

#include "Entity.h"

namespace Engine {

	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:
		friend class Scene;
		Entity m_Entity;

	};

}
