#pragma once

#include "RenderCommand.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Shader.h"

// TEMPORARY?
#include "Engine\Renderer\Texture.h"

namespace Engine {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Colour;
		glm::vec2 TextureCoordinates;
	};

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		//Ref<Shader> FlatColourShader;
		Ref<Shader> TexturedQuadShader;
		Ref<Texture> WhiteTexture;

		const uint32_t MaxQuadsPerDraw    = 100; // TODO tweak based on batch rendering performance
		const uint32_t MaxVerticesPerDraw = 4 * MaxQuadsPerDraw;
		const uint32_t MaxIndicesPerDraw  = 6 * MaxQuadsPerDraw;

		uint32_t QuadIndexCount = 0; // add 6 each time we add a new quad to the current batch

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& orthoCamera);
		static void EndScene();
		static void FlushBatch();

		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 colour);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 colour);

		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float angleRadians, glm::vec4 colour);
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float angleRadians, glm::vec4 colour);

		static void DrawTexturedQuad(glm::vec2 position, glm::vec2 size, Ref<Texture> texture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));
		static void DrawTexturedQuad(glm::vec3 position, glm::vec2 size, Ref<Texture> texture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));

		static void DrawRotatedTexturedQuad(glm::vec2 position, glm::vec2 size, float angleRadians, Ref<Texture> texture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));
		static void DrawRotatedTexturedQuad(glm::vec3 position, glm::vec2 size, float angleRadians, Ref<Texture> texture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));

	private:
		static Renderer2DStorage s_Data;

	};
}

