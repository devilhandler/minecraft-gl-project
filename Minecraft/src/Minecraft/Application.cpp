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
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);


		// Vertex Buffer
		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3]
		{
			//	X		Y			Z
				-0.5f,	-0.5f,		0.0f,
				0.5f,	-0.5f,		0.0f,
				0.0f,	0.5f,		0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // or use nullptr
		
		// Index Buffer
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3]
		{
			0, 1, 2
		};

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Shader
		// in the video, Cherno has the driver that already has a shader compiled but mine don't have one :P
		m_ProgramID = glCreateProgram();
		const char* vertexShaderSource = "#version 460 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
			"}\n\0";
		const char* fragmentShaderSource = "#version 460 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.5f, 1.0f);\n"
			"}\n\0";

		int success;
		char infoLog[512];

		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vs, 1, &vertexShaderSource, NULL);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vs, 512, NULL, infoLog);
			MC_CORE_INFO("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{0}\n", infoLog);
		}
		glShaderSource(fs, 1, &fragmentShaderSource, NULL);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fs, 512, NULL, infoLog);
			MC_CORE_INFO("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{0}\n", infoLog);
		}

		glAttachShader(m_ProgramID, vs);
		glAttachShader(m_ProgramID, fs);
		glLinkProgram(m_ProgramID);
		glGetShaderiv(m_ProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
			MC_CORE_INFO("ERROR::PROGRAM::LINKING_FAILED\n{0}\n", infoLog);
		}

		MC_CORE_INFO("Successfully created shader!");
		glDeleteShader(vs);
		glDeleteShader(fs);
		
	}

	Application::~Application()
	{
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteVertexArrays(1, &m_VertexArray);
		glDeleteProgram(m_ProgramID);
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
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(m_ProgramID);
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

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