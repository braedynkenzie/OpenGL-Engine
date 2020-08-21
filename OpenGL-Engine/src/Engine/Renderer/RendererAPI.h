#pragma once

#include "VertexArray.h"

#include <glm\glm.hpp>
#include <memory>

namespace Engine {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
			// Vulkan, Direct3D, etc.. 

		};

	public:
		virtual void Init() = 0;
		virtual void SetClearColour(const glm::vec4& clearColour) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_RenderingAPI; }
		inline static void SetAPI(API newAPI) { s_RenderingAPI = newAPI; }
	private:
		static API s_RenderingAPI;

	};

}