#include "EnginePCH.h"
#include "Renderer.h"

namespace Engine {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		// The plan is to have this submit a RenderCommand to a render command queue, which can then be optimized and executed when EndScene() is called
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}

