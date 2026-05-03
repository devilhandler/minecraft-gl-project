#pragma once

#include <memory>

#ifdef RE_PLATFORM_WINDOWS
	#if RE_DYNAMIC_LINK
		#ifdef RE_BUILD_DLL
			#define RE_API __declspec(dllexport)
		#else
			#define RE_API __declspec(dllimport)
		#endif
	#else
		#define RE_API
	#endif
#else
	#error Rogue Engine currently only support Windows!
#endif

#ifdef RE_DEBUG
	#define RE_ENABLE_ASSERTS
#endif

#ifdef RE_ENABLE_ASSERTS
	#define RE_ASSERT(x, ...) { if (!(x)) { RE_ERROR("Assertion Failed: {0}", __VA_ARGS__); } }
	#define RE_CORE_ASSERT(x, ...) { if (!(x)) { RE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); } }
#else
	#define RE_ASSERT(x, ...)
	#define RE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define RE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Rogue
{
	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}