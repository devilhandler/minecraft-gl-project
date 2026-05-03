#include "mcpch.h"
#include "Minecraft/Renderer/GraphicsContext.h"

#include "Minecraft/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Minecraft {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    MC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		MC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}