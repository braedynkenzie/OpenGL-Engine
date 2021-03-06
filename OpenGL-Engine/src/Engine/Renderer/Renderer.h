#pragma once

#include "RenderCommand.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Shader.h"

namespace Engine {

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera); 
		static void EndScene(); 

		static void Init();
		static void Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix = glm::mat4(1.0f));
		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetRenderingAPI() { return RendererAPI::GetAPI(); }
		inline static void SetRenderingAPI(const RendererAPI::API& renderingAPI) { RendererAPI::SetAPI(renderingAPI); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Scope<SceneData> s_SceneData;
	};

}