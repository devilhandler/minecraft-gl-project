#include "mcpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Minecraft
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}