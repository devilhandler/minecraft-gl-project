#include <Minecraft.h>

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
			//	X		Y			Z		R	G		B	A
				-0.5f,	-0.5f,		0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				0.5f,	-0.5f,		0.0f, 0.2f, 0.3f, 1.0f, 1.0f,
				0.0f,	0.5f,		0.0f, 0.8f, 0.7f, 0.0f, 1.0f,
		};

		std::shared_ptr<Minecraft::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Minecraft::VertexBuffer::Create(vertices, sizeof(vertices)));
		Minecraft::BufferLayout layout = {
			{ Minecraft::ShaderDataType::Float3, "aPosition" },
			{ Minecraft::ShaderDataType::Float4, "aColor" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		uint32_t indices[3]{ 0, 1, 2 };
		std::shared_ptr<Minecraft::IndexBuffer> indexBuffer;
		indexBuffer.reset(Minecraft::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSource{ R"(
			#version 460 core
			
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec4 aColor;

			uniform mat4 uViewProjection;

			out vec3 vPosition;
			out vec4 vColor;

			void main()
			{
				vPosition = aPosition + 0.5;
				vColor = aColor;
				gl_Position = uViewProjection * vec4(aPosition, 1.0f);
			}
		)" };

		std::string fragmentSource{ R"(
			#version 460 core
			
			layout(location = 0) out vec4 FragColor;
			in vec3 vPosition;
			in vec4 vColor;

			void main()
			{
				FragColor = vec4(vPosition * 0.5f + 0.5f, 1.0f);
				FragColor = vColor;
			}
		)" };

		m_Shader.reset(new Minecraft::Shader(vertexSource, fragmentSource));

		// Square test
		m_SquareVA.reset(Minecraft::VertexArray::Create());
		float squareVertices[4 * 7]
		{
			//	X		Y			Z		R	G		B	A
				-0.5f,	-0.5f,		0.0f, 0.2f, 0.8f, 0.2f, 1.0f,
				0.5f,	-0.5f,		0.0f, 0.8f, 0.7f, 0.0f, 1.0f,
				0.5f,	0.5f,		0.0f, 0.8f, 0.7f, 1.0f, 1.0f,
				-0.5f,	0.5f,		0.0f, 0.5f, 0.5f, 0.5f, 1.0f
		};
		std::shared_ptr<Minecraft::VertexBuffer> squareVB; /*= std::make_shared<VertexBuffer>(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));*/
		squareVB.reset(Minecraft::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Minecraft::ShaderDataType::Float3, "aPosition" },
			{ Minecraft::ShaderDataType::Float4, "aColor" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Minecraft::IndexBuffer> squareIB; /*= std::make_shared<IndexBuffer>(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));*/
		squareIB.reset(Minecraft::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// since it's the same shaders anyways
		/*std::string vertexSource2{ R"(
			#version 460 core

			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec4 aColor;
			out vec3 vPosition;
			out vec4 vColor;

			void main()
			{
				vPosition = aPosition + 0.5;
				vColor = aColor;
				gl_Position = vec4(aPosition, 1.0f);
			}
		)" };

		std::string fragmentSource2{ R"(
			#version 460 core

			layout(location = 0) out vec4 FragColor;
			in vec3 vPosition;
			in vec4 vColor;

			void main()
			{
				FragColor = vec4(vPosition * 0.5f + 0.5f, 1.0f);
				FragColor = vColor;
			}
		)" };*/

		m_SquareShader.reset(new Minecraft::Shader(vertexSource, fragmentSource));
	}

	void OnUpdate(Minecraft::Timestep ts) override
	{
		MC_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

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
			Minecraft::Renderer::Submit(m_SquareShader, m_SquareVA);
			Minecraft::Renderer::Submit(m_Shader, m_VertexArray);
		}
		Minecraft::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override 
	{

	}

	void OnEvent(Minecraft::Event& event) override
	{
		
	}
private:
	std::shared_ptr<Minecraft::VertexArray> m_VertexArray;
	std::shared_ptr<Minecraft::Shader> m_Shader;

	std::shared_ptr<Minecraft::VertexArray> m_SquareVA;
	std::shared_ptr<Minecraft::Shader> m_SquareShader;

	Minecraft::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition{};
	float m_CameraMoveSpeed{ 5.0f };

	float m_CameraRotation{ 0.0f };
	float m_CameraRotationSpeed{ 180.0f };
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