#pragma once

#include "RenderCommand.h"

namespace Engine {

	class Renderer
	{
	public:
		static void BeginScene(); 
		static void EndScene(); 
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray); 

		inline static RendererAPI::API GetRenderingAPI() { return RendererAPI::GetAPI(); }
		inline static void SetRenderingAPI(const RendererAPI::API& renderingAPI) { RendererAPI::SetAPI(renderingAPI); }

	};

}