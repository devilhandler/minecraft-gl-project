#include "mcpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Minecraft
{
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		MC_CORE_ASSERT(false, "RendererAPI: None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGLShader(filepath);
		}

		MC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		MC_CORE_ASSERT(false, "RendererAPI: None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGLShader(vertexSource, fragmentSource);
		}

		MC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}