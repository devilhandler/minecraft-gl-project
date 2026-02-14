#pragma once

#include <memory>
#include "Minecraft/Renderer/Buffer.h"

namespace Minecraft
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Minecraft::Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Minecraft::Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Minecraft::Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Minecraft::Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}