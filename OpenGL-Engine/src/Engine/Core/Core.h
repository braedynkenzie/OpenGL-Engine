#pragma once

#include <memory>

// Platform detection using platform-specific defined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define ENGINE_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define HZ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define HZ_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define HZ_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define HZ_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End platform detection


#ifdef ENGINE_PLATFORM_WINDOWS
	#if ENGINE_DYNAMIC_LINK // DLL support
		#ifdef ENGINE_BUILD_DLL
			#define ENGINE_API __declspec(dllexport)
		#else
			#define ENGINE_API __declspec(dllimport)
		#endif // ENGINE_BUILD_DLL
#else 
	#define ENGINE_API
#endif
#else 
	#error Engine only supports Windows platform!
#endif 

#ifdef ENGINE_RELEASE
	#define ENGINE_PROFILING
#endif

#ifdef ENGINE_DEBUG
	#define ENGINE_ENABLE_ASSERTS
	#define ENGINE_PROFILING
#endif

#ifdef ENGINE_ENABLE_ASSERTS
	#define ENGINE_ASSERT(x, ...) { if(!(x)) { ENGINE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } } 
	#define ENGINE_CORE_ASSERT(x, ...) { if(!(x)) { ENGINE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ENGINE_ASSERT(x, ...) 
	#define ENGINE_CORE_ASSERT(x, ...)
#endif

#ifdef ENGINE_PROFILING
	#define ENGINE_PROFILE_BEGIN_SESSION(name, filepath) ::Engine::Instrumentor::Get().BeginSession(name, filepath)
	#define ENGINE_PROFILE_END_SESSION() ::Engine::Instrumentor::Get().EndSession()
	#define ENGINE_PROFILE_SCOPE(name) ::Engine::InstrumentationTimer timer##_LINE__(name);
	#define ENGINE_PROFILE_FUNCTION() ENGINE_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define ENGINE_PROFILE_BEGIN_SESSION(name, filepath)
	#define ENGINE_PROFILE_END_SESSION()
	#define ENGINE_PROFILE_SCOPE(name)
	#define ENGINE_PROFILE_FUNCTION()
#endif

#define BIT(x) (1 << x)

#define ENGINE_BIND_EVENT_FUNC(function) std::bind(&function, this, std::placeholders::_1)

namespace Engine {

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

}
