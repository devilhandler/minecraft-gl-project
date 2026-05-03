#include "mcpch.h"

#include "Minecraft/Renderer/RenderCommand.h"

namespace Minecraft
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}