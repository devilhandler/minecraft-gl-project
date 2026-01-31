workspace "Minecraft"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++23"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MC_PLATFORM_WINDOWS",
			"MC_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "MC_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "MC_DIST"
		optimize "On"

	buildoptions {"/utf-8"}

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
		symbols "On"

	filter "configurations:Release"
		defines "MC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "MC_DIST"
		optimize "On"

	buildoptions {"/utf-8"}