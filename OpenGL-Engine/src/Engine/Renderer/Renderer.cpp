#include "EnginePCH.h"
#include "Renderer.h"

namespace Engine {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		// The plan is to have this submit a RenderCommand to a render command queue, which can then be optimized and executed when EndScene() is called
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}

