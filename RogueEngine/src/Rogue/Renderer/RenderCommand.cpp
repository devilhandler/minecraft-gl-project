#include "repch.h"

#include "Rogue/Renderer/RenderCommand.h"

namespace Minecraft
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}