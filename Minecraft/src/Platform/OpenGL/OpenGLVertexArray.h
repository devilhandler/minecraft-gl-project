#pragma once

#include "Minecraft/Renderer/VertexArray.h"

namespace Minecraft
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Minecraft::Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Minecraft::Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Minecraft::Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; };
		virtual const Minecraft::Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; };
	private:
		uint32_t m_RendererID;
		std::vector<Minecraft::Ref<VertexBuffer>> m_VertexBuffers;
		Minecraft::Ref<IndexBuffer> m_IndexBuffer;
	};
}