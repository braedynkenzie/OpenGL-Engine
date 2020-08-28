#include "EnginePCH.h"
#include "Renderer2D.h"

#include "Engine\Renderer\Shader.h"
#include "Engine\Renderer\VertexArray.h"

#include <glm\ext\matrix_transform.hpp>

// TEMPORARY
//#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	Renderer2DStorage* Renderer2D::s_Data;

	void Renderer2D::Init()
	{
		// Initialize static Renderer2D data on the heap
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();

		float quadVertices[] = { 
			//   Vertex positions     --   Texture coords
				0.5f,  0.5f,  0.0f,	   		1.0f, 1.0f,		// Top right
			   -0.5f,  0.5f,  0.0f,	   		0.0f, 1.0f,		// Top left
				0.5f, -0.5f,  0.0f,	   		1.0f, 0.0f,		// Bottom right
			   -0.5f, -0.5f,  0.0f,    		0.0f, 0.0f		// Bottom left
		};

		Ref<VertexBuffer> quadVB;
		quadVB = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		quadVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoords" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(quadVB);

		uint32_t quadIndices[6] = { 0, 1, 2,
									2, 1, 3 };
		Ref<IndexBuffer> quadIB;
		quadIB = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(quadIB);

		s_Data->FlatColourShader = Shader::Create("assets/shaders/FlatColour.glsl");
		s_Data->TexturedQuadShader = Shader::Create("assets/shaders/TexturedQuad.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& orthoCamera)
	{
		// TODO fix this hot garbage
		s_Data->FlatColourShader->Bind();
		s_Data->FlatColourShader->SetMat4("u_ViewProjectionMatrix", orthoCamera.GetViewProjectionMatrix());
		s_Data->TexturedQuadShader->Bind();
		s_Data->TexturedQuadShader->SetMat4("u_ViewProjectionMatrix", orthoCamera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 colour)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, colour);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 colour)
	{
		// Bind flat colour shader and set its uniforms
		s_Data->FlatColourShader->Bind();
		s_Data->FlatColourShader->SetFloat4("u_Colour", colour);
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, { size.x, size.y, 1.0f });
		s_Data->FlatColourShader->SetMat4("u_ModelMatrix", modelMatrix);

		// Draw the quad
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawTexturedQuad(glm::vec3 position, glm::vec2 size, Ref<Texture> texture, float angle)
	{
		// Bind textured quad shader and set its uniforms
		s_Data->TexturedQuadShader->Bind();
		texture->Bind(0);
		s_Data->TexturedQuadShader->SetInt("u_Texture", 0);
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, { size.x, size.y, 1.0f });
		s_Data->TexturedQuadShader->SetMat4("u_ModelMatrix", modelMatrix);

		// Draw the quad
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}
