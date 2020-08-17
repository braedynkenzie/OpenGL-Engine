#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Engine {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void SetClearColour(const glm::vec4& clearColour) override;
		void Clear() override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	};

}
