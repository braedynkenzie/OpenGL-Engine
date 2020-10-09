#pragma once

#include "RenderCommand.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Shader.h"

#include "Engine\Renderer\Texture.h"
#include "Engine\Renderer\SubTexture2D.h"

namespace Engine {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Colour;
		glm::vec2 TextureCoordinates;
		float TextureIndex;
		float TilingFactor;
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& orthoCamera);
		static void EndScene();
		static void FlushBatch();
		static void CheckBatch();

		static void SetBatchCount(const uint32_t maxNumQuadsPerDraw);

		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 colour);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 colour);

		static void DrawQuad(glm::mat4 modelMatrix, glm::vec4 colour);
		static void DrawTexturedQuad(glm::mat4 modelMatrix, Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));

		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float angleRadians, glm::vec4 colour);
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float angleRadians, glm::vec4 colour);

		static void DrawTexturedQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));
		static void DrawTexturedQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));

		static void DrawRotatedTexturedQuad(glm::vec2 position, glm::vec2 size, float angleRadians, Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));
		static void DrawRotatedTexturedQuad(glm::vec3 position, glm::vec2 size, float angleRadians, Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));

		static void DrawSpritesheetQuad(glm::vec2 position, glm::vec2 size, Ref<SubTexture2D> subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));
		static void DrawSpritesheetQuad(glm::vec3 position, glm::vec2 size, Ref<SubTexture2D> subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));
		// Deprecated -- TODO change all calls from below functions to above functions
		static void DrawSpritesheetQuad(glm::vec2 position, glm::vec2 size, uint32_t spriteX, uint32_t spriteY, Ref<Texture2D> textureAtlas, float sheetWidth, float sheetHeight, float spriteWidth, float spriteHeight, uint32_t gapPixels, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));
		static void DrawSpritesheetQuad(glm::vec3 position, glm::vec2 size, uint32_t spriteX, uint32_t spriteY, Ref<Texture2D> textureAtlas, float sheetWidth, float sheetHeight, float spriteWidth, float spriteHeight, uint32_t gapPixels, float tilingFactor = 1.0f, const glm::vec4 tintColour = glm::vec4(1.0f));

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		//static RendererData s_Data;

	private:
		static void StartNewBatch();
	};
}

