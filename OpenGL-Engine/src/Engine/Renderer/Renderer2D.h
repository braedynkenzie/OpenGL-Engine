#pragma once

#include "RenderCommand.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Shader.h"

namespace Engine {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColourShader;
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

	private:
		static Renderer2DStorage* s_Data;

	};
}

