workspace "OpenGL-Engine"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

startproject "Editor"

-- Other module include directories
IncludeDirectories = {}
IncludeDirectories["GLFW"] = "OpenGL-Engine/vendor/GLFW/include/"
IncludeDirectories["Glad"] = "OpenGL-Engine/vendor/Glad/include/"
IncludeDirectories["Imgui"] = "OpenGL-Engine/vendor/imgui/"
IncludeDirectories["glm"] = "OpenGL-Engine/vendor/glm/"
IncludeDirectories["stb_image"] = "OpenGL-Engine/vendor/stb_image/"

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
    kind "StaticLib"
    language "C++"
    cppdialect "c++17"
    staticruntime "on"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("intermediates/" .. outputDirectory .. "/%{prj.name}")

    pchheader "EnginePCH.h"
    pchsource "OpenGL-Engine/src/EnginePCH.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

        --"%{prj.name}/vendor/glm/glm/**.inl",
        --"%{prj.name}/vendor/glm/glm/**.cpp",

        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
    }

    includedirs {
        "%{prj.name}/vendor/spdlog/include/",
        "%{prj.name}/src/",
        "%{IncludeDirectories.GLFW}",
        "%{IncludeDirectories.Glad}",
        "%{IncludeDirectories.Imgui}",
        "%{IncludeDirectories.glm}",
        "%{IncludeDirectories.stb_image}"
    }

    links {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }
    
    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "ENGINE_PLATFORM_WINDOWS",
            "ENGINE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++17"
    staticruntime "on"

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
        systemversion "latest"

        defines {
            "ENGINE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        runtime "Release"
        optimize "on"

project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++17"
    staticruntime "on"

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
        systemversion "latest"

        defines {
            "ENGINE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        runtime "Release"
        optimize "on"
