#include <RogueEngine.h>
#include <Rogue/Core/EntryPoint.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Platform/OpenGL/OpenGLShader.h>

#include <imguidock/imgui.h>

#include "Sandbox2D.h"

class ExampleLayer : public Rogue::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.f / 720.f, true)
	{ 
		// OpenGL related things
		// Vertex Array
		m_VertexArray = Rogue::VertexArray::Create();

		// Vertex Buffer
		float vertices[3 * 3]
		{
			//	X		Y			Z	
				-0.5f,	-0.5f,		0.0f,
				0.5f,	-0.5f,		0.0f,
				0.0f,	0.5f,		0.0f
		};

		Rogue::Ref<Rogue::VertexBuffer> vertexBuffer;
		vertexBuffer = Rogue::VertexBuffer::Create(vertices, sizeof(vertices));
		Rogue::BufferLayout layout = {
			{ Rogue::ShaderDataType::Float3, "a_Position" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		uint32_t indices[3]{ 0, 1, 2 };
		Rogue::Ref<Rogue::IndexBuffer> indexBuffer;
		indexBuffer = Rogue::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSource{ R"(
			#version 460 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
			}
		)" };

		std::string fragmentSource{ R"(
			#version 460 core
			
			layout(location = 0) out vec4 FragColor;

			uniform vec3 u_Color;

			void main()
			{
				FragColor = vec4(u_Color, 1.0f);
			}
		)" };

		m_Shader = Rogue::Shader::Create("Triangle", vertexSource, fragmentSource);

		// Square test
		m_SquareVA = Rogue::VertexArray::Create();
		float squareVertices[4 * 5]
		{
			//	  X		  Y       Z       T1      T2
				-0.5f,  -0.5f,   0.0f,   0.0f,   0.0f,
				 0.5f,	-0.5f,   0.0f,   1.0f,   0.0f,
				 0.5f,	 0.5f,   0.0f,   1.0f,   1.0f,
				-0.5f,	 0.5f,   0.0f,   0.0f,   1.0f
		};
		Rogue::Ref<Rogue::VertexBuffer> squareVB{ Rogue::VertexBuffer::Create(squareVertices, sizeof(squareVertices)) };
		squareVB->SetLayout({
			{ Rogue::ShaderDataType::Float3, "a_Position" },
			{ Rogue::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
		Rogue::Ref<Rogue::IndexBuffer> squareIB{ Rogue::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)) };
		m_SquareVA->SetIndexBuffer(squareIB);

		m_SquareShader = Rogue::Shader::Create("SquareGridShader", vertexSource, fragmentSource);

		// m_SquareTextureShader
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		// m_SquareTextureShader = Rogue::Shader::Create("assets/shaders/Texture.glsl");
		m_Texture = Rogue::Texture2D::Create("assets/textures/grass.png");
		m_ChernoLogoTexture = Rogue::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Rogue::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Rogue::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Rogue::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);
		// MC_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		Rogue::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Rogue::RenderCommand::Clear();

		Rogue::Renderer::BeginScene(m_CameraController.GetCamera());
		{
			glm::mat4 scale{ glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)) };

			glm::vec4 redColor{ 0.8f, 0.2f, 0.3f, 1.0f };
			glm::vec4 blueColor{ 0.2f, 0.3f, 0.8f, 1.0f };

			Rogue::Ref<Rogue::OpenGLShader> m_SquareShaderPointer{ std::dynamic_pointer_cast<Rogue::OpenGLShader>(m_SquareShader) };
			Rogue::Ref<Rogue::OpenGLShader> m_ShaderPointer{ std::dynamic_pointer_cast<Rogue::OpenGLShader>(m_Shader) };

			m_SquareShaderPointer->Bind();
			m_ShaderPointer->Bind();

			//Rogue::MaterialRef material = new Rogue::Material();
			//Rogue::MaterialInstanceRef materialInstance = new Rogue::MaterialInstance(material);

			//materialInstance->Set("u_Color", redColor);
			//squareMesh->SetMaterial(materialInstance);

			for (unsigned int y{ 0 }; y < 20; ++y)
			{
				for (unsigned int x{ 0 }; x < 20; ++x)
				{
					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform{ glm::translate(glm::mat4(1.0f), pos) * scale };
					if (x % 2 == 0)
						m_SquareShaderPointer->UploadUniformFloat3("u_Color", m_SquareColor);
					else
						m_SquareShaderPointer->UploadUniformFloat3("u_Color", glm::vec3(0.8f) - m_SquareColor);
					Rogue::Renderer::Submit(m_SquareShader, m_SquareVA, transform);
				}
			}

			auto textureShader{ m_ShaderLibrary.Get("Texture") };

			m_Texture->Bind();
			Rogue::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
			m_ChernoLogoTexture->Bind();
			Rogue::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			// Triangle
			// Rogue::Renderer::Submit(m_Shader, m_VertexArray);
		}
		Rogue::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Rogue::Event& event) override
	{
		m_CameraController.OnEvent(event);

		if (event.GetEventType() == Rogue::EventType::WindowResize)
		{
			auto& re{ (Rogue::WindowResizeEvent&)event };

			//float zoom{ (float)re.GetWidth() / 1280.0f };
			//m_CameraController.SetZoomLevel(zoom);
		}
	}
private:
	Rogue::ShaderLibrary m_ShaderLibrary;
	Rogue::Ref<Rogue::VertexArray> m_VertexArray;
	Rogue::Ref<Rogue::Shader> m_Shader;
	
	Rogue::Ref<Rogue::VertexArray> m_SquareVA;
	Rogue::Ref<Rogue::Shader> m_SquareShader;

	Rogue::Ref<Rogue::Texture2D> m_Texture, m_ChernoLogoTexture;

	Rogue::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor{ 0.8f, 0.2f, 0.3f };
};

//class Sandbox3D : public Rogue::Layer 
//{
//public:
//
//private:
//	Rogue::ShaderLibrary m_ShaderLibrary;
//	Rogue::Ref<Rogue::VertexArray> m_VertexArray;
//	Rogue::Ref<Rogue::Shader> m_Shader;
//
//	Rogue::Ref<Rogue::VertexArray> m_SquareVA;
//	Rogue::Ref<Rogue::Shader> m_SquareShader;
//
//	Rogue::Ref<Rogue::Texture2D> m_Texture;
//	Rogue::Mesh mesh;
//
//	Rogue::CameraController m_CameraController;
//};

class Sandbox : public Rogue::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Rogue::Application* Rogue::CreateApplication()
{
	return new Sandbox();
}