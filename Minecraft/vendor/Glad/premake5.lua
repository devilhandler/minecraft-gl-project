project "Glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/GLAD/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		-- buildoptions { "-std=c11", "-lgdi32" }
		systemversion "latest"
		staticruntime "On" --staticly linking the runtime libraries

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"

	-- filter "configurations:Debug"
	-- 	runtime "Debug"
	-- 	symbols "on"
	-- 
	-- filter "configurations:Release"
	-- 	runtime "Release"
	-- 	optimize "on"