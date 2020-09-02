#include "EnginePCH.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"

// TEMPORARY
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		ENGINE_PROFILE_FUNCTION();

		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		ENGINE_PROFILE_FUNCTION();

	}

	void Renderer::Init()
	{
		ENGINE_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix)
	{
		ENGINE_PROFILE_FUNCTION();

		// The plan is to have this submit a RenderCommand to a render command queue, which can then be optimized and executed when EndScene() is called
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", modelMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		ENGINE_PROFILE_FUNCTION();

		RenderCommand::SetViewport(0, 0, width, height);
	}

}

