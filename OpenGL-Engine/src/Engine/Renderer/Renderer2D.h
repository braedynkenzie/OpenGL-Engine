#pragma once

#include "RenderCommand.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Shader.h"

// TEMPORARY?
#include "Engine\Renderer\Texture.h"

namespace Engine {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColourShader;
		Ref<Shader> TexturedQuadShader;
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& orthoCamera);
		static void EndScene();

		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 colour);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 colour);

		static void DrawTexturedQuad(glm::vec3 position, glm::vec2 size, Ref<Texture> texture, float angle = 0.0f);

	private:
		static Renderer2DStorage* s_Data;

	};
}

