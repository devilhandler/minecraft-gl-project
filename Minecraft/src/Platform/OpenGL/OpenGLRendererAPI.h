#pragma once

#include "Minecraft/Renderer/RendererAPI.h"

namespace Minecraft
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Minecraft::Ref<VertexArray>& vertexArray) override;
	};
}