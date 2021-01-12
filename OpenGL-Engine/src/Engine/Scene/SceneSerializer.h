#pragma once

#include "Engine\Scene\Scene.h"

namespace Engine {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene> scene)
			: m_Scene(scene) {}

		//~SceneSerializer() = default;

		void SerializeText(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool DeserializeText(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;

	};
}

