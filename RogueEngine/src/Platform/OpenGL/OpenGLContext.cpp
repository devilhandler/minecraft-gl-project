#include "repch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

namespace Rogue
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RE_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		RE_PROFILE_FUNCTION();

		// If it is using the AMD iGPU it will go for the compat profile, if we use the nvidia gpu it will use the core profile
		// why? i don't know.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwMakeContextCurrent(m_WindowHandle);
		int status{ gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) };
		RE_CORE_ASSERT(status, "Failed to initialize Glad!");

		RE_CORE_INFO("OpenGL Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		RE_CORE_INFO("OpenGL Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		RE_CORE_INFO("OpenGL Version: {0}", (const char*)glGetString(GL_VERSION));

		#ifdef RE_ENABLE_ASSERTS
		int versionMajor{}, versionMinor{};
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		RE_CORE_ASSERT
		(
			versionMajor > 4 || (versionMajor == 4 && versionMinor >= 6), 
			"Rogue Engine requires OpenGL 4.6!"
		);
		#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		RE_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}