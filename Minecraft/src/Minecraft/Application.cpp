#include "mcpch.h"
#include "Application.h"

#include <glad/glad.h>

#include "Input.h"

namespace Minecraft
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		MC_CORE_ASSERT(s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = std::make_unique<ImGuiLayer>();

		
		// OpenGL related things
		// Vertex Array
		m_VertexArray.reset(VertexArray::Create());

		// Vertex Buffer
		float vertices[3 * 7]
		{
			//	X		Y			Z		R	G		B	A
				-0.5f,	-0.5f,		0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				0.5f,	-0.5f,		0.0f, 0.2f, 0.3f, 1.0f, 1.0f,
				0.0f,	0.5f,		0.0f, 0.8f, 0.7f, 0.0f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		
		// Index Buffer
		uint32_t indices[3]{ 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSource{ R"(
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

		m_Shader.reset(new Shader(vertexSource, fragmentSource));

		// Square test
		m_SquareVA.reset(VertexArray::Create());
		float squareVertices[4 * 7]
		{
			//	X		Y			Z		R	G		B	A
				-0.5f,	-0.5f,		0.0f, 0.2f, 0.8f, 0.2f, 1.0f,
				0.5f,	-0.5f,		0.0f, 0.8f, 0.7f, 0.0f, 1.0f,
				0.5f,	0.5f,		0.0f, 0.8f, 0.7f, 1.0f, 1.0f,
				-0.5f,	0.5f,		0.0f, 0.5f, 0.5f, 0.5f, 1.0f
		};
		std::shared_ptr<VertexBuffer> squareVB; /*= std::make_shared<VertexBuffer>(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));*/
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB; /*= std::make_shared<IndexBuffer>(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));*/
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		
		std::string vertexSource2{ R"(
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
		)" };

		m_SquareShader.reset(new Shader(vertexSource2, fragmentSource2));
	}

	Application::~Application()
	{
		// glDeleteBuffers(1, &m_VertexBuffer);
		// glDeleteVertexArrays(1, &m_VertexArray);
		// glDeleteProgram(m_ProgramID);
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		// MC_CORE_INFO("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		//WindowResizeEvent e(1280, 720);
		//if (e.IsInCategory(EventCategoryApplication))
		//{
		//	MC_TRACE(e);
		//}
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_SquareShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, (void*)0);

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, (void*)0);

			// glUseProgram(m_ProgramID);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// ImGui Render
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			auto [x, y] = Input::GetMousePosition();
			// MC_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
		};
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}