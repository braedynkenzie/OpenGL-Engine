#include "EnginePCH.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Engine {

	YAML::Emitter& operator<<(YAML::Emitter& outputYAML, const glm::vec3& vec3)
	{
		outputYAML << YAML::Flow;
		outputYAML << YAML::BeginSeq;
		outputYAML << vec3.x;
		outputYAML << vec3.y;
		outputYAML << vec3.z;
		outputYAML << YAML::EndSeq;
		return outputYAML;
	}

	YAML::Emitter& operator<<(YAML::Emitter& outputYAML, const glm::vec4& vec4)
	{
		outputYAML << YAML::Flow;
		outputYAML << YAML::BeginSeq;
		outputYAML << vec4.x;
		outputYAML << vec4.y;
		outputYAML << vec4.z;
		outputYAML << vec4.w;
		outputYAML << YAML::EndSeq;
		return outputYAML;
	}

	static void SerializeEntity(YAML::Emitter& outputYAML, Entity entity)
	{
		// Begin serializing the entity
		outputYAML << YAML::BeginMap; // Entity start
		outputYAML << YAML::Key << "Entity";
		outputYAML << YAML::Value << "TODO unique entity ID goes here"; // TODO

		// Serialize all of the entity's components
		if (entity.HasComponent<TagComponent>())
		{
			outputYAML << YAML::Key << "TagComponent";
			outputYAML << YAML::BeginMap; // TagComponent start
			TagComponent& tagComponent = entity.GetComponent<TagComponent>();
			outputYAML << YAML::Key << "Tag";
			outputYAML << YAML::Value << tagComponent.Tag;
			outputYAML << YAML::EndMap; // TagComponent end
		}
		if (entity.HasComponent<TransformComponent>())
		{
			outputYAML << YAML::Key << "TransformComponent";
			outputYAML << YAML::BeginMap; // TransformComponent start
			TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
			outputYAML << YAML::Key << "Translation";
			outputYAML << YAML::Value << transformComponent.Translation;
			outputYAML << YAML::Key << "Rotation";
			outputYAML << YAML::Value << transformComponent.Rotation;
			outputYAML << YAML::Key << "Scale";
			outputYAML << YAML::Value << transformComponent.Scale;
			outputYAML << YAML::EndMap; // TransformComponent end
		}
		if (entity.HasComponent<CameraComponent>())
		{
			outputYAML << YAML::Key << "CameraComponent";
			outputYAML << YAML::BeginMap; // CameraComponent start
			CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
			outputYAML << YAML::Key << "IsPrimaryCamera";
			outputYAML << YAML::Value << cameraComponent.IsPrimaryCamera;
			outputYAML << YAML::Key << "HasFixedAspectRatio";
			outputYAML << YAML::Value << cameraComponent.HasFixedAspectRatio;

			// Separate the CameraComponent.Camera into its own YAML map
			outputYAML << YAML::Key << "Camera";
			outputYAML << YAML::BeginMap; // Camera start
			outputYAML << YAML::Key << "ProjectionType";
			outputYAML << YAML::Value << (int)cameraComponent.Camera.GetProjectionType();
			outputYAML << YAML::Key << "PerspectiveFOVy";
			outputYAML << YAML::Value << cameraComponent.Camera.GetPerspectiveFOVy();
			outputYAML << YAML::Key << "PerspectiveNearBound";
			outputYAML << YAML::Value << cameraComponent.Camera.GetPerspectiveNearBound();
			outputYAML << YAML::Key << "PerspectiveFarBound";
			outputYAML << YAML::Value << cameraComponent.Camera.GetPerspectiveFarBound();
			outputYAML << YAML::Key << "OrthographicSize";
			outputYAML << YAML::Value << cameraComponent.Camera.GetOrthographicSize();
			outputYAML << YAML::Key << "OrthographicNearBound";
			outputYAML << YAML::Value << cameraComponent.Camera.GetOrthographicNearBound();
			outputYAML << YAML::Key << "OrthographicFarBound";
			outputYAML << YAML::Value << cameraComponent.Camera.GetOrthographicFarBound();
			outputYAML << YAML::EndMap; // Camera end

			outputYAML << YAML::EndMap; // CameraComponent end
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			outputYAML << YAML::Key << "SpriteRendererComponent";
			outputYAML << YAML::BeginMap; // SpriteRendererComponent start
			SpriteRendererComponent& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			outputYAML << YAML::Key << "Colour";
			outputYAML << YAML::Value << spriteRendererComponent.Colour;
			outputYAML << YAML::EndMap; // SpriteRendererComponent end
		}
		outputYAML << YAML::EndMap; // Entity end
	}

	void SceneSerializer::SerializeText(const std::string& filepath)
	{
		YAML::Emitter outputYAML;

		// Begin writing YAML key-value pairs 
		outputYAML << YAML::BeginMap;
		outputYAML << YAML::Key << "Scene";
		outputYAML << YAML::Value << "TODO Scene name goes here"; // TODO

		outputYAML << YAML::Key << "Entities";
		outputYAML << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = Entity{ entityID, m_Scene.get() };
			if (!entity)
				return;
			SerializeEntity(outputYAML, entity);
		});

		outputYAML << YAML::EndSeq;
		outputYAML << YAML::EndMap;

		// Finally, write the YAML output as text (to the given filepath)
		std::ofstream fout(filepath); // TODO create intermediate directories if nonexistent
		fout << outputYAML.c_str();
	}

	void SceneSerializer::SerializeBinary(const std::string& filepath)
	{
		// TODO implement
		ENGINE_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::DeserializeText(const std::string& filepath)
	{
		// TODO implement
		ENGINE_CORE_ASSERT(false, "Not implemented");
		return false;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		// TODO implement
		ENGINE_CORE_ASSERT(false, "Not implemented");
		return false;
	}

}