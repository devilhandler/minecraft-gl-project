workspace "Minecraft"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Minecraft/vendor/GLFW/include"
IncludeDir["Glad"] = "Minecraft/vendor/Glad/include"
IncludeDir["imguidock"] = "Minecraft/vendor/imguidock"

include "Minecraft/vendor/GLFW"
include "Minecraft/vendor/Glad"
include "Minecraft/vendor/imguidock"

project "Minecraft"
	location "Minecraft"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mcpch.h"
	pchsource "Minecraft/src/mcpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imguidock}",
	}

	links
	{
		"GLFW",
		"Glad",
		"imguidock",
		"opengl32.lib",
		"dwmapi.lib"
	}

	buildoptions "/utf-8"

	filter "system:windows"
		cppdialect "C++23"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MC_PLATFORM_WINDOWS",
			"MC_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "MC_DEBUG"
		staticruntime "off"
		runtime "Debug"
		-- buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "MC_RELEASE"
		staticruntime "off"
		runtime "Release"
		-- buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "MC_DIST"
		staticruntime "off"
		runtime "Release"
		-- buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"

	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Minecraft/vendor/spdlog/include",
		"Minecraft/src"
	}

	links
	{
		"Minecraft"
	}

	buildoptions "/utf-8"

	filter "system:windows"
		cppdialect "C++23"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MC_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MC_DEBUG"
		staticruntime "off"
		runtime "Debug"
		-- buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "MC_RELEASE"
		staticruntime "off"
		runtime "Release"
		-- buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "MC_DIST"
		staticruntime "off"
		runtime "Release"
		-- buildoptions "/MD"
		optimize "On"