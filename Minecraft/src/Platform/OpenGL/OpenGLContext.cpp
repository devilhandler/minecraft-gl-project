#include "mcpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

namespace Minecraft
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		MC_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		// If it is using the AMD iGPU it will go for the compat profile, if we use the nvidia gpu it will use the core profile
		// why? i don't know.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwMakeContextCurrent(m_WindowHandle);
		int status{ gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) };
		MC_CORE_ASSERT(status, "Failed to initialize Glad!");

		MC_CORE_INFO("OpenGL Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		MC_CORE_INFO("OpenGL Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		MC_CORE_INFO("OpenGL Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}