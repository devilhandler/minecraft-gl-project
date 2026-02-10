#include "mcpch.h"
#include "Application.h"

#include <glad/glad.h>

#include "Input.h"

namespace Minecraft
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:			return GL_FLOAT;
			case ShaderDataType::Float2:		return GL_FLOAT;
			case ShaderDataType::Float3:		return GL_FLOAT;
			case ShaderDataType::Float4:		return GL_FLOAT;
			case ShaderDataType::Mat3:			return GL_FLOAT;
			case ShaderDataType::Mat4:			return GL_FLOAT;
			case ShaderDataType::Int:			return GL_INT;
			case ShaderDataType::Int2:			return GL_INT;
			case ShaderDataType::Int3:			return GL_INT;
			case ShaderDataType::Int4:			return GL_INT;
			case ShaderDataType::Bool:			return GL_BOOL;
		}

		MC_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

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

		float vertices[3 * 7]
		{
			//	X		Y			Z		R	G		B	A
				-0.5f,	-0.5f,		0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				0.5f,	-0.5f,		0.0f, 0.2f, 0.3f, 1.0f, 1.0f,
				0.0f,	0.5f,		0.0f, 0.8f, 0.7f, 0.0f, 1.0f,
		};

		// Vertex Buffer
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "aPosition", true },
				{ ShaderDataType::Float4, "aColor", true }
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index{ 0 };
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(),
				(const void*)element.Offset	// or use nullptr
			); 
			index++;
		}

		// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// glEnableVertexAttribArray(0);
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // or use nullptr
		
		// Index Buffer
		uint32_t indices[3]
		{
			0, 1, 2
		};
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		/*glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);*/

		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSource{R"(
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
		)"};

		std::string fragmentSource{R"(
			#version 460 core
			
			layout(location = 0) out vec4 FragColor;
			in vec3 vPosition;
			in vec4 vColor;

			void main()
			{
				FragColor = vec4(vPosition * 0.5f + 0.5f, 1.0f);
				FragColor = vColor;
			}
		)"};

		m_Shader.reset(new Shader(vertexSource, fragmentSource));

		// Shader
		// in the video, Cherno has the driver that already has a shader compiled but mine don't have one :P
		/*m_ProgramID = glCreateProgram();
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
		glDeleteShader(fs);*/
		
	}

	Application::~Application()
	{
		// glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteVertexArrays(1, &m_VertexArray);
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
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// glUseProgram(m_ProgramID);
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, (void*)0);

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