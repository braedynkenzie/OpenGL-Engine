workspace "OpenGL-Engine"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

startproject "Sandbox"

-- Other module include directories
IncludeDirectories = {}
IncludeDirectories["GLFW"] = "OpenGL-Engine/vendor/GLFW/include/"

-- Include GLFW premake5.lua file into this .lua file
include "OpenGL-Engine/vendor/GLFW/"

project "OpenGL-Engine"
	location "OpenGL-Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("intermediates/" .. outputDirectory .. "/%{prj.name}")

	pchheader "EnginePCH.h"
	pchsource "OpenGL-Engine/src/EnginePCH.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include/",
		"%{prj.name}/src/",
		"%{IncludeDirectories.GLFW}"
	}

	links {
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"ENGINE_PLATFORM_WINDOWS",
			"ENGINE_BUILD_DLL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDirectory .. "/Sandbox/")
		}

	filter "configurations:Debug"
		defines {
			"ENGINE_DEBUG",
			"ENGINE_ENABLE_ASSERTS"
		}
		symbols "On"
	
	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "ENGINE_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("intermediates/" .. outputDirectory .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"OpenGL-Engine/vendor/spdlog/include/",
		"OpenGL-Engine/src/"
	}

	links {
		"OpenGL-Engine"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"ENGINE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines {
			"ENGINE_DEBUG",
			-- "ENGINE_ENABLE_ASSERTS"
		}
		symbols "On"
	
	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "ENGINE_DIST"
		optimize "On"
