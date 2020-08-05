-- Glad premake5.lua file

project "Glad"
	kind "StaticLib"
    language "C"

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("intermediates/" .. outputDirectory .. "/%{prj.name}")

	files
	{
        "src/glad.c",
        "include/glad/glad.h",
        "include/KHR/khrplatform.h"
    }

    includedirs
    {
        "include/"
	}

	filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"
