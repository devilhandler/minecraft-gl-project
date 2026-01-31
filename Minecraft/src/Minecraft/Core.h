#pragma once

#ifdef MC_PLATFORM_WINDOWS
	#ifdef MC_BUILD_DLL
		#define MC_API __declspec(dllexport)
	#else
		#define MC_API __declspec(dllimport)
	#endif
#else
	#error This minecraft clone project only support Windows!
#endif

#define BIT(x) (1 << x)