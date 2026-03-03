#include "mcpch.h"
#include "Minecraft/Renderer/Renderer2D.h"

#include "Minecraft/Renderer/VertexArray.h"
#include "Minecraft/Renderer/Shader.h"
#include "Minecraft/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Minecraft
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data{ nullptr };

	void Renderer2D::Init()
	{
		MC_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		// Square test
		s_Data->QuadVertexArray = VertexArray::Create();
		float squareVertices[4 * 5]
		{
			//	  X		  Y       Z       T1      T2
				-0.5f,  -0.5f,   0.0f,   0.0f,   0.0f,
				 0.5f,	-0.5f,   0.0f,   1.0f,   0.0f,
				 0.5f,	 0.5f,   0.0f,   1.0f,   1.0f,
				-0.5f,	 0.5f,   0.0f,   0.0f,   1.0f
		};
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout
		({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		MC_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		MC_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		MC_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MC_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform{ glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f}) };
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		MC_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform{ glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f}) };
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}