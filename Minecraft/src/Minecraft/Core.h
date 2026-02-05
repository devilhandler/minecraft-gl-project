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

#ifdef MC_ENABLE_ASSERTS
	#define MC_ASSERT(x, ...) { if (!(x)) { MC_ERROR("Assertion Failed: {0}", __VA_ARGS__); } }
	#define MC_CORE_ASSERT(x, ...) { if (!(x)) { MC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); } }
#else
	#define MC_ASSERT(x, ...)
	#define MC_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define MC_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)