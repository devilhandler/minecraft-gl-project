#include "repch.h"

#include "Rogue/Renderer/RenderCommand.h"

namespace Rogue
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}