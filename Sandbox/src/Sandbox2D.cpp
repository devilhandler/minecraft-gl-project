#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Platform/OpenGL/OpenGLShader.h>

#include <imguidock/imgui.h>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
	
}

void Sandbox2D::OnAttach()
{
	// Square test
	m_VertexArray = Minecraft::VertexArray::Create();
	float squareVertices[4 * 3]
	{
		//	  X		  Y       Z       T1      T2
			-0.5f,  -0.5f,   0.0f,
			 0.5f,	-0.5f,   0.0f,
			 0.5f,	 0.5f,   0.0f,
			-0.5f,	 0.5f,   0.0f	
	};
	Minecraft::Ref<Minecraft::VertexBuffer> squareVB;
	squareVB = Minecraft::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout
	({
		{ Minecraft::ShaderDataType::Float3, "a_Position" },
	});
	m_VertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
	Minecraft::Ref<Minecraft::IndexBuffer> squareIB;
	squareIB = Minecraft::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(squareIB);

	m_Shader = Minecraft::Shader::Create("assets/shaders/Square.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Minecraft::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Minecraft::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Minecraft::RenderCommand::Clear();

	Minecraft::Renderer::BeginScene(m_CameraController.GetCamera());
	{
		glm::mat4 scale{ glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)) };

		Minecraft::Ref<Minecraft::OpenGLShader> m_ShaderPointer{ std::dynamic_pointer_cast<Minecraft::OpenGLShader>(m_Shader) };
		m_ShaderPointer->Bind();

		for (unsigned int y{ 0 }; y < 20; ++y)
		{
			for (unsigned int x{ 0 }; x < 20; ++x)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform{ glm::translate(glm::mat4(1.0f), pos) * scale };
				if (x % 2 == 0)
					m_ShaderPointer->UploadUniformFloat3("u_Color", m_SquareColor);
				else
					m_ShaderPointer->UploadUniformFloat3("u_Color", glm::vec3(0.8f) - m_SquareColor);
				Minecraft::Renderer::Submit(m_Shader, m_VertexArray, transform);
			}
		}
	}
	Minecraft::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Minecraft::Event& e)
{
	m_CameraController.OnEvent(e);
}