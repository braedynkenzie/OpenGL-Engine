#include "EnginePCH.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Engine {

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}
	
	void OpenGLRendererAPI::SetClearColour(const glm::vec4& clearColour)
	{
		glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		// TODO make it possible to clear other types of buffers (ie. stencil buffers)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}
