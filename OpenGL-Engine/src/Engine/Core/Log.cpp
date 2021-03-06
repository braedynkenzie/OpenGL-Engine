#include "EnginePCH.h"
#include "Engine/Core/Log.h"

namespace Engine
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() 
	{
		// Logging format: [Timestamp] Name of Logger: Message
		spdlog::set_pattern("%^[%T] %n: %v%$"); 

		// Create Engine/Client loggers
		s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}
