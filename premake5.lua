workspace "Minecraft"
	architecture "x64"
	startproject "Sandbox"

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
IncludeDir["glm"] = "Minecraft/vendor/glm"
IncludeDir["stb_image"] = "Minecraft/vendor/stb_image"

group "Dependencies"
	include "Minecraft/vendor/GLFW"
	include "Minecraft/vendor/Glad"
	include "Minecraft/vendor/imguidock"

group ""

project "Minecraft"
	location "Minecraft"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mcpch.h"
	pchsource "Minecraft/src/mcpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imguidock}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"imguidock",
		"opengl32.lib",
		"dwmapi.lib"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	buildoptions "/utf-8"

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"MC_PLATFORM_WINDOWS",
			"MC_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "MC_DEBUG"
		runtime "Debug"
		-- buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "MC_RELEASE"
		runtime "Release"
		-- buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "MC_DIST"
		runtime "Release"
		-- buildoptions "/MD"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++23"
	staticruntime "on"

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
		"Minecraft/src",
		"Minecraft/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Minecraft"
	}

	buildoptions "/utf-8"

	filter "system:windows"
		-- staticruntime "On"
		systemversion "latest"

		defines
		{
			"MC_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MC_DEBUG"
		runtime "Debug"
		-- buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "MC_RELEASE"
		runtime "Release"
		-- buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "MC_DIST"
		runtime "Release"
		-- buildoptions "/MD"
		optimize "on"