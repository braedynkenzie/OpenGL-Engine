#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Engine {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;
		void SetClearColour(const glm::vec4& clearColour) override;
		void Clear() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};

}
