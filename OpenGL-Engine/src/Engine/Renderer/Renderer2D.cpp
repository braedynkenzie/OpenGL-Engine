#include "EnginePCH.h"
#include "Renderer2D.h"

#include "Engine\Renderer\Shader.h"
#include "Engine\Renderer\VertexArray.h"

#include <glm\ext\matrix_transform.hpp>

// TEMPORARY
//#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	Renderer2DStorage Renderer2D::s_Data;

	void Renderer2D::Init()
	{
		ENGINE_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		//float quadVertices[] = { 
		//	//   Vertex positions     --   Texture coords
		//		0.5f,  0.5f,  0.0f,	   		1.0f, 1.0f,		// Top right
		//	   -0.5f,  0.5f,  0.0f,	   		0.0f, 1.0f,		// Top left
		//		0.5f, -0.5f,  0.0f,	   		1.0f, 0.0f,		// Bottom right
		//	   -0.5f, -0.5f,  0.0f,    		0.0f, 0.0f		// Bottom left
		//};

		//Ref<VertexBuffer> quadVB;
		//quadVB = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVerticesPerDraw * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Colour" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		// Create a heap allocated buffer to store all batched QuadVertices
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVerticesPerDraw];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndicesPerDraw];
		
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndicesPerDraw; i += 6)
		{
			// Triangle 1
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			// Triangle 2
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		//uint32_t quadIndices[6] = { 0, 1, 2,
		//							2, 1, 3 };
		Ref<IndexBuffer> quadIB;
		//quadIB = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
		quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndicesPerDraw);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices; // TODO need to manage this memory differently once multithreading is implemented

		// Set any default texture data
		// Requires debugging
	/*	s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	*/	//
	// Fix for now
		s_Data.WhiteTexture = Texture2D::Create("assets/textures/white_texture.png");

		// Create shader program and set default uniforms
		s_Data.TexturedQuadShader = Shader::Create("assets/shaders/TexturedQuad.glsl");
		s_Data.TexturedQuadShader->Bind();
		s_Data.WhiteTexture->Bind(0);
		s_Data.TexturedQuadShader->SetInt("u_Texture", 0);
		int32_t textureSamplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			textureSamplers[i] = i;
		s_Data.TexturedQuadShader->SetIntArray("u_Textures", textureSamplers, s_Data.MaxTextureSlots);

		// Set any default texture slots
		//for (auto texSlotRef : s_Data.TextureSlotRefs)
		//	texSlotRef = nullptr;
		s_Data.TextureSlotRefs[0] = s_Data.WhiteTexture;

		// Create vertex position data (to be multiplied by the model matrices)
		s_Data.QuadVertexPositions[0] = glm::vec4(-0.5, -0.5, 0.0f, 1.0f);
		s_Data.QuadVertexPositions[1] = glm::vec4( 0.5, -0.5, 0.0f, 1.0f);
		s_Data.QuadVertexPositions[2] = glm::vec4( 0.5,  0.5, 0.0f, 1.0f);
		s_Data.QuadVertexPositions[3] = glm::vec4(-0.5,  0.5, 0.0f, 1.0f);
	}

	void Renderer2D::Shutdown()
	{
		ENGINE_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& orthoCamera)
	{
		ENGINE_PROFILE_FUNCTION();

		// Bind the ViewProjectionMatrix for all default shaders
		s_Data.TexturedQuadShader->Bind();
		s_Data.TexturedQuadShader->SetMat4("u_ViewProjectionMatrix", orthoCamera.GetViewProjectionMatrix());

		// Set/Reset all pointers and indexes
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		ENGINE_PROFILE_FUNCTION();

		uint32_t vertexBufferDataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, vertexBufferDataSize);

		FlushBatch();
	}

	void Renderer2D::FlushBatch()
	{
		// Bind all required textures to their appropriate slots
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlotRefs[i]->Bind(i);
		// Make sure the QuadVertexArray is bound
		s_Data.QuadVertexArray->Bind(); 
		// Draw the entire batch of quads
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::CheckBatch()
	{
		if (s_Data.QuadIndexCount == s_Data.MaxIndicesPerDraw)
		{
			// Maximum quads reached in this batch, flush
			EndScene();
			// Reset pointers
			s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
			s_Data.QuadIndexCount = 0;
		}
	}

	void Renderer2D::SetBatchCount(const uint32_t maxNumQuadsPerDraw)
	{
		EndScene();
		s_Data.MaxQuadsPerDraw = maxNumQuadsPerDraw;
		s_Data.MaxVerticesPerDraw = 4 * maxNumQuadsPerDraw;
		s_Data.MaxIndicesPerDraw = 6 * maxNumQuadsPerDraw;
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 colour)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, colour);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 colour)
	{
		ENGINE_PROFILE_FUNCTION();

		const float textureIndex = 0.0f; // White texture index

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(size.x, size.y, 1.0f));

		// Set the data for a new QuadVertex in the current batch
		//
		// QuadVertex: bottom left
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Colour = colour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: bottom right
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Colour = colour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: top right
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Colour = colour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: top left
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Colour = colour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		// Check if batch is full and flush
		CheckBatch();
	}

	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float angleRadians, glm::vec4 colour)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angleRadians, colour);
	}

	void Renderer2D::DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float angleRadians, glm::vec4 colour)
	{
		ENGINE_PROFILE_FUNCTION();

		const float textureIndex = 0.0f; // White texture index

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(size.x, size.y, 1.0f));

		// Set the data for a new QuadVertex in the current batch
		//
		// QuadVertex: bottom left
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Colour = colour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: bottom right
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Colour = colour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: top right
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Colour = colour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: top left
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Colour = colour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		// Check if batch is full and flush
		CheckBatch();
	}

	void Renderer2D::DrawTexturedQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor, const glm::vec4 tintColour)
	{
		DrawTexturedQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColour);
	}

	void Renderer2D::DrawTexturedQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor, const glm::vec4 tintColour)
	{
		ENGINE_PROFILE_FUNCTION();

		float textureIndex = 0.0f;
		// Check if texture is already bound to some texture slot
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*texture.get() == *s_Data.TextureSlotRefs[i].get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		// If texture is not already bound to a slot, bind it to the next available texture slot
		if (textureIndex == 0.0f)
		{
			textureIndex = s_Data.TextureSlotIndex;
			s_Data.TextureSlotRefs[textureIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(size.x, size.y, 1.0f));

		// Set the data for a new QuadVertex in the current batch
		//
		// QuadVertex: bottom left
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[0];;
		s_Data.QuadVertexBufferPtr->Colour = tintColour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: bottom right
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Colour = tintColour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: top right
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Colour = tintColour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: top left
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Colour = tintColour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		// Check if batch is full and flush
		CheckBatch();
	}

	void Renderer2D::DrawRotatedTexturedQuad(glm::vec2 position, glm::vec2 size, float angleRadians, Ref<Texture2D> texture, float tilingFactor, const glm::vec4 tintColour)
	{
		DrawRotatedTexturedQuad({ position.x, position.y, 0.0f }, size, angleRadians, texture, tilingFactor, tintColour);
	}

	void Renderer2D::DrawRotatedTexturedQuad(glm::vec3 position, glm::vec2 size, float angleRadians, Ref<Texture2D> texture, float tilingFactor, const glm::vec4 tintColour)
	{
		ENGINE_PROFILE_FUNCTION();

		float textureIndex = 0.0f;
		// Check if texture is already bound to some texture slot
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*texture.get() == *s_Data.TextureSlotRefs[i].get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		// If texture is not already bound to a slot, bind it to the next available texture slot
		if (textureIndex == 0.0f)
		{
			textureIndex = s_Data.TextureSlotIndex;
			s_Data.TextureSlotRefs[textureIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(size.x, size.y, 1.0f));

		// Set the data for a new QuadVertex in the current batch
		//
		// QuadVertex: bottom left
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Colour = tintColour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: bottom right
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Colour = tintColour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: top right
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Colour = tintColour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
		// QuadVertex: top left
		s_Data.QuadVertexBufferPtr->Position = modelMatrix * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Colour = tintColour;
		s_Data.QuadVertexBufferPtr->TextureCoordinates = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		// Check if batch is full and flush
		CheckBatch();
	}
}
