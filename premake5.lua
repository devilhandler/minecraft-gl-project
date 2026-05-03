workspace "RogueEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	multiprocessorcompile "on"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
EngineDir = "RogueEngine"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = EngineDir .. "/vendor/GLFW/include"
IncludeDir["Glad"] = EngineDir .. "/vendor/Glad/include"
IncludeDir["imguidock"] = EngineDir .. "/vendor/imguidock"
IncludeDir["glm"] = EngineDir .. "/vendor/glm"
IncludeDir["stb_image"] = EngineDir .. "/vendor/stb_image"

group "Dependencies"
	include (EngineDir .. "/vendor/GLFW")
	include (EngineDir .. "/vendor/Glad")
	include (EngineDir .. "/vendor/imguidock")

group ""

project "RogueEngine"
	location (EngineDir)
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "repch.h"
	pchsource (EngineDir .. "/src/repch.cpp")

	files
	{
		(EngineDir .. "/src/**.h"),
		(EngineDir .. "/src/**.cpp"),
		(EngineDir .. "/vendor/stb_image/**.h"),
		(EngineDir .. "/vendor/stb_image/**.cpp"),
		(EngineDir .. "/vendor/glm/glm/**.hpp"),
		(EngineDir .. "/vendor/glm/glm/**.inl")
	}

	includedirs
	{
		EngineDir .. "/src",
		EngineDir .. "/vendor/spdlog/include",
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
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
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
		EngineDir .. "/vendor/spdlog/include",
		EngineDir .. "/src",
		EngineDir .. "/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"RogueEngine"
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

project "RogueCanvas"
	location "RogueCanvas"
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
		EngineDir .. "/vendor/spdlog/include",
		EngineDir .. "/src",
		EngineDir .. "/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"RogueEngine"
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