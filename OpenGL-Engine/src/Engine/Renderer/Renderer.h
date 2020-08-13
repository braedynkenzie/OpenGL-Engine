#pragma once

namespace Engine {

	enum class RenderingAPI
	{
		None = 0, OpenGL = 1 
		// Vulkan, Direct3D, etc.. 

	};

	class Renderer
	{
	public:
		inline static RenderingAPI& GetRenderingAPI() { return s_RenderingAPI; }
		inline static RenderingAPI& SetRenderingAPI(RenderingAPI renderingAPI) { s_RenderingAPI = renderingAPI; }

		// TODO 

	private:
		static RenderingAPI s_RenderingAPI;

	};

}