#include "mcpch.h"
#include "Minecraft/Renderer/VertexArray.h"

#include "Minecraft/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Minecraft
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		MC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexArray>();
		}

		MC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}