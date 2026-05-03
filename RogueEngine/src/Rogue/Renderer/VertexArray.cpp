#include "repch.h"
#include "Rogue/Renderer/VertexArray.h"

#include "Rogue/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Rogue
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexArray>();
		}

		RE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}