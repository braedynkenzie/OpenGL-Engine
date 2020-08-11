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
IncludeDirectories["Glad"] = "OpenGL-Engine/vendor/Glad/include/"
IncludeDirectories["Imgui"] = "OpenGL-Engine/vendor/imgui/"
IncludeDirectories["glm"] = "OpenGL-Engine/vendor/glm/"

group "Dependencies"
	-- Include GLFW premake5.lua file into this file
	include "OpenGL-Engine/vendor/GLFW/"
	-- Include Glad premake5.lua file into this file
	include "OpenGL-Engine/vendor/Glad/"
	-- Include Imgui premake5.lua file into this file
	include "OpenGL-Engine/vendor/imgui/"
group ""

project "OpenGL-Engine"
	location "OpenGL-Engine"
	kind "SharedLib"
	language "C++"
	staticruntime "Off"

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
		"%{IncludeDirectories.GLFW}",
		"%{IncludeDirectories.Glad}",
		"%{IncludeDirectories.Imgui}",
		"%{IncludeDirectories.glm}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "c++17"
		systemversion "latest"

		defines {
			"ENGINE_PLATFORM_WINDOWS",
			"ENGINE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("IF NOT EXIST ../bin/" .. outputDirectory .. "/Sandbox mkdir ../bin/" .. outputDirectory .. "/Sandbox/"),
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDirectory .. "/Sandbox/")
		}

	filter "configurations:Debug"
		defines {
			"ENGINE_DEBUG",
			-- "ENGINE_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ENGINE_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("intermediates/" .. outputDirectory .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"OpenGL-Engine/vendor/spdlog/include/",
		"OpenGL-Engine/src/",
		"OpenGL-Engine/vendor/",
		"%{IncludeDirectories.glm}"
	}

	links {
		"OpenGL-Engine"
	}

	filter "system:windows"
		cppdialect "c++17"
		systemversion "latest"

		defines {
			"ENGINE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines {
			"ENGINE_DEBUG",
			-- "ENGINE_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ENGINE_DIST"
		runtime "Release"
		optimize "On"
