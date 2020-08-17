#include "EnginePCH.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Engine {

	void OpenGLRendererAPI::SetClearColour(const glm::vec4& clearColour)
	{
		glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		// TODO make it possible to clear other types of buffers (ie. stencil buffers)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}