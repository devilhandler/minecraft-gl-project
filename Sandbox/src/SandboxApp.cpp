#include <Minecraft.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Platform/OpenGL/OpenGLShader.h>

#include <imguidock/imgui.h>

class ExampleLayer : public Minecraft::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{ 
		// OpenGL related things
		// Vertex Array
		m_VertexArray.reset(Minecraft::VertexArray::Create());

		// Vertex Buffer
		float vertices[3 * 7]
		{
			//	X		Y			Z	
				-0.5f,	-0.5f,		0.0f,
				0.5f,	-0.5f,		0.0f,
				0.0f,	0.5f,		0.0f
		};

		Minecraft::Ref<Minecraft::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Minecraft::VertexBuffer::Create(vertices, sizeof(vertices)));
		Minecraft::BufferLayout layout = {
			{ Minecraft::ShaderDataType::Float3, "a_Position" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		uint32_t indices[3]{ 0, 1, 2 };
		Minecraft::Ref<Minecraft::IndexBuffer> indexBuffer;
		indexBuffer.reset(Minecraft::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader.reset(Minecraft::Shader::Create(vertexSource, fragmentSource));

		// Square test
		m_SquareVA.reset(Minecraft::VertexArray::Create());
		float squareVertices[4 * 5]
		{
			//	  X		  Y       Z       T1      T2
				-0.5f,  -0.5f,   0.0f,   0.0f,   0.0f,
				 0.5f,	-0.5f,   0.0f,   1.0f,   0.0f,
				 0.5f,	 0.5f,   0.0f,   1.0f,   1.0f,
				-0.5f,	 0.5f,   0.0f,   0.0f,   1.0f
		};
		Minecraft::Ref<Minecraft::VertexBuffer> squareVB;
		squareVB.reset(Minecraft::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Minecraft::ShaderDataType::Float3, "a_Position" },
			{ Minecraft::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
		Minecraft::Ref<Minecraft::IndexBuffer> squareIB;
		squareIB.reset(Minecraft::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_SquareShader.reset(Minecraft::Shader::Create(vertexSource, fragmentSource));

		// m_SquareTextureShader
		std::string squareTextureVertexSource{ R"(
			#version 460 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
			}
		)" };

		std::string squareTextureFragmentSource{ R"(
			#version 460 core
			
			layout(location = 0) out vec4 FragColor;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				FragColor = texture(u_Texture, v_TexCoord);
			}
		)" };

		m_SquareTextureShader.reset(Minecraft::Shader::Create(squareTextureVertexSource, squareTextureFragmentSource));
		m_Texture = Minecraft::Texture2D::Create("assets/textures/grass.png");
		m_ChernoLogoTexture = Minecraft::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Minecraft::OpenGLShader>(m_SquareTextureShader)->Bind();
		std::dynamic_pointer_cast<Minecraft::OpenGLShader>(m_SquareTextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Minecraft::Timestep ts) override
	{
		// MC_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if (Minecraft::Input::IsKeyPressed(MC_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		if (Minecraft::Input::IsKeyPressed(MC_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		if (Minecraft::Input::IsKeyPressed(MC_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		if (Minecraft::Input::IsKeyPressed(MC_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Minecraft::Input::IsKeyPressed(MC_KEY_LEFT))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Minecraft::Input::IsKeyPressed(MC_KEY_RIGHT))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Minecraft::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Minecraft::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Minecraft::Renderer::BeginScene(m_Camera);
		{
			glm::mat4 scale{ glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)) };

			glm::vec4 redColor{ 0.8f, 0.2f, 0.3f, 1.0f };
			glm::vec4 blueColor{ 0.2f, 0.3f, 0.8f, 1.0f };

			Minecraft::Ref<Minecraft::OpenGLShader> m_SquareShaderPointer{ std::dynamic_pointer_cast<Minecraft::OpenGLShader>(m_SquareShader) };
			Minecraft::Ref<Minecraft::OpenGLShader> m_ShaderPointer{ std::dynamic_pointer_cast<Minecraft::OpenGLShader>(m_Shader) };

			m_SquareShaderPointer->Bind();
			m_ShaderPointer->Bind();

			//Minecraft::MaterialRef material = new Minecraft::Material();
			//Minecraft::MaterialInstanceRef materialInstance = new Minecraft::MaterialInstance(material);

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
					Minecraft::Renderer::Submit(m_SquareShader, m_SquareVA, transform);
				}
			}

			m_Texture->Bind();
			Minecraft::Renderer::Submit(m_SquareTextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
			m_ChernoLogoTexture->Bind();
			Minecraft::Renderer::Submit(m_SquareTextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			// Triangle
			// Minecraft::Renderer::Submit(m_Shader, m_VertexArray);
		}
		Minecraft::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Minecraft::Event& event) override
	{
		
	}
private:
	Minecraft::Ref<Minecraft::VertexArray> m_VertexArray;
	Minecraft::Ref<Minecraft::Shader> m_Shader;
	
	Minecraft::Ref<Minecraft::VertexArray> m_SquareVA;
	Minecraft::Ref<Minecraft::Shader> m_SquareShader, m_SquareTextureShader;

	Minecraft::Ref<Minecraft::Texture2D> m_Texture, m_ChernoLogoTexture;

	Minecraft::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition{};
	float m_CameraMoveSpeed{ 5.0f };

	float m_CameraRotation{ 0.0f };
	float m_CameraRotationSpeed{ 180.0f };

	glm::vec3 m_SquareColor{ 0.8f, 0.2f, 0.3f };
};

class Sandbox : public Minecraft::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Minecraft::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Minecraft::Application* Minecraft::CreateApplication()
{
	return new Sandbox();
}