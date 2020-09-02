#pragma once

#include "RendererAPI.h"

namespace Engine {

	// This class is used to dispatch calls to the RendererAPI implementation stored in s_RendererAPI
	class RenderCommand 
	{
	public:
		// Dispatch calls that also appear in RendererAPI
		inline static void Init()											
		{
			ENGINE_PROFILE_FUNCTION();

			s_RendererAPI->Init(); 
		}

		inline static void SetClearColour(const glm::vec4& clearColour)		
		{ 
			s_RendererAPI->SetClearColour(clearColour); 
		}

		inline static void Clear()											
		{
			s_RendererAPI->Clear(); 
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) 
		{ 
			s_RendererAPI->DrawIndexed(vertexArray); 
		}

	private:
		// This is a rendering API agnostic implementation of a RendererAPI
		static Scope<RendererAPI> s_RendererAPI;
	};
}

