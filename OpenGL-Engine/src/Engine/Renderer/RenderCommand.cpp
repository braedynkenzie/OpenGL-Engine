#include "EnginePCH.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Engine {

	// TODO in the future this will be done dynamically based on the RendererAPI::s_RenderingAPI
	Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();


}
