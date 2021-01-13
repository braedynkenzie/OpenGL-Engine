#include "EnginePCH.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

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
		outputYAML << YAML::Value << 123456; // TODO unique entity ID goes here 

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
		std::ifstream stream = std::ifstream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		// Parse/load the file into a YAML node
		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		// Get the name of the scene
		std::string sceneName = data["Scene"].as<std::string>();
		ENGINE_CORE_TRACE("Loading scene: {0}", sceneName);

		// Process the scene entities
		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				Entity deserializedEntity = m_Scene->CreateEntity();
				//TODO uint64_t uuid = entity["Entity"].as<uint64_t>();

				// TagComponent
				std::string entityName;
				YAML::Node& tagComponentNode = entity["TagComponent"];
				if (tagComponentNode)
				{
					TagComponent& tagComponent = deserializedEntity.GetComponent<TagComponent>();
					entityName = tagComponentNode["Tag"].as<std::string>();
					tagComponent.Tag = entityName;
				}

				// TransformComponent
				YAML::Node& transformComponentNode = entity["TransformComponent"];
				if (transformComponentNode)
				{
					TransformComponent& transformComponent = deserializedEntity.GetComponent<TransformComponent>();
					transformComponent.Translation = transformComponentNode["Translation"].as<glm::vec3>();
					transformComponent.Rotation = transformComponentNode["Rotation"].as<glm::vec3>();
					transformComponent.Scale = transformComponentNode["Scale"].as<glm::vec3>();
				}

				// CameraComponent
				YAML::Node& cameraComponentNode = entity["CameraComponent"];
				if (cameraComponentNode)
				{
					CameraComponent& cameraComponent = deserializedEntity.AddComponent<CameraComponent>();
					cameraComponent.IsPrimaryCamera		= cameraComponentNode["IsPrimaryCamera"].as<bool>();
					cameraComponent.HasFixedAspectRatio = cameraComponentNode["HasFixedAspectRatio"].as<bool>();
					YAML::Node& cameraNode = cameraComponentNode["Camera"];
					cameraComponent.Camera.SetProjectionType(cameraNode["ProjectionType"].as<int>());
					cameraComponent.Camera.SetPerspectiveFOVy(cameraNode["PerspectiveFOVy"].as<float>());
					cameraComponent.Camera.SetPerspectiveNearBound(cameraNode["PerspectiveNearBound"].as<float>());
					cameraComponent.Camera.SetPerspectiveFarBound(cameraNode["PerspectiveFarBound"].as<float>());
					cameraComponent.Camera.SetOrthographicSize(cameraNode["OrthographicSize"].as<float>());
					cameraComponent.Camera.SetOrthographicNearBound(cameraNode["OrthographicNearBound"].as<float>());
					cameraComponent.Camera.SetOrthographicFarBound(cameraNode["OrthographicFarBound"].as<float>());
				}

				// SpriteRendererComponent
				YAML::Node& spriteRendererComponentNode = entity["SpriteRendererComponent"];
				if (spriteRendererComponentNode)
				{
					SpriteRendererComponent& spriteRendererComponent = deserializedEntity.AddComponent<SpriteRendererComponent>();
					spriteRendererComponent.Colour = spriteRendererComponentNode["Colour"].as<glm::vec4>();
				}

				// TODO once uuid implemented
				//ENGINE_CORE_TRACE("Deserialized entity with ID: {0}, Name: {1}", uuid, entityName);
			}
		}
		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		// TODO implement
		ENGINE_CORE_ASSERT(false, "Not implemented");
		return false;
	}

}