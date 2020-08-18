#pragma once

#include "RenderCommand.h"
#include "Engine\Renderer\OrthographicCamera.h"
#include "Engine\Renderer/Shader.h"

namespace Engine {

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera); 
		static void EndScene(); 
		static void Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& modelMatrix = glm::mat4(1.0f));

		inline static RendererAPI::API GetRenderingAPI() { return RendererAPI::GetAPI(); }
		inline static void SetRenderingAPI(const RendererAPI::API& renderingAPI) { RendererAPI::SetAPI(renderingAPI); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* s_SceneData;
	};

}