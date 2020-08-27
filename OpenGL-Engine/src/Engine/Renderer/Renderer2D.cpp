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
				0.5f,  0.5f,  0.0f,
			   -0.5f,  0.5f,  0.0f,
				0.5f, -0.5f,  0.0f,
			   -0.5f, -0.5f,  0.0f
		};

		Ref<VertexBuffer> quadVB;
		quadVB.reset(VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
		quadVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(quadVB);

		uint32_t quadIndices[6] = { 0, 1, 2,
									2, 1, 3 };
		Ref<IndexBuffer> quadIB;
		quadIB.reset(IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(quadIB);

		s_Data->FlatColourShader = Shader::Create("assets/shaders/FlatColour.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& orthoCamera)
	{
		s_Data->FlatColourShader->Bind();
		s_Data->FlatColourShader->SetMat4("u_ViewProjectionMatrix", orthoCamera.GetViewProjectionMatrix());
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

}
