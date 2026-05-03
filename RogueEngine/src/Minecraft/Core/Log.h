#pragma once

#include "Minecraft/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Minecraft
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
#define MC_CORE_TRACE(...)     ::Minecraft::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MC_CORE_INFO(...)      ::Minecraft::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MC_CORE_WARN(...)      ::Minecraft::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MC_CORE_ERROR(...)     ::Minecraft::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MC_CORE_CRITICAL(...)  ::Minecraft::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define MC_TRACE(...)          ::Minecraft::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MC_INFO(...)           ::Minecraft::Log::GetClientLogger()->info(__VA_ARGS__)
#define MC_WARN(...)           ::Minecraft::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MC_ERROR(...)          ::Minecraft::Log::GetClientLogger()->error(__VA_ARGS__)
#define MC_CRITICAL(...)       ::Minecraft::Log::GetClientLogger()->critical(__VA_ARGS__)