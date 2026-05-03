#pragma once

#include "Rogue/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Rogue
{
	class Log
	{
	public:
		static void Init();
		
		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define MC_CORE_TRACE(...)     ::Rogue::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MC_CORE_INFO(...)      ::Rogue::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MC_CORE_WARN(...)      ::Rogue::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MC_CORE_ERROR(...)     ::Rogue::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MC_CORE_CRITICAL(...)  ::Rogue::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define MC_TRACE(...)          ::Rogue::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MC_INFO(...)           ::Rogue::Log::GetClientLogger()->info(__VA_ARGS__)
#define MC_WARN(...)           ::Rogue::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MC_ERROR(...)          ::Rogue::Log::GetClientLogger()->error(__VA_ARGS__)
#define MC_CRITICAL(...)       ::Rogue::Log::GetClientLogger()->critical(__VA_ARGS__)