#pragma once

#include "RendererAPI.h"

namespace Engine {

	// This class is used to dispatch calls to the RendererAPI implementation stored in s_RendererAPI
	class RenderCommand 
	{
	public:
		// Dispatch calls that also appear in RendererAPI
		inline static void SetClearColour(const glm::vec4& clearColour)					{ s_RendererAPI->SetClearColour(clearColour); }
		inline static void Clear()														{ s_RendererAPI->Clear(); }
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }

	private:
		// This is a rendering API agnostic implementation of a RendererAPI
		static RendererAPI* s_RendererAPI;
	};
}

