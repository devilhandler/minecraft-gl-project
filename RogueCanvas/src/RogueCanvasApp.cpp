#include <RogueEngine.h>
#include <Rogue/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Rogue
{
	class RougeCanvas : public Application
	{
	public:
		RougeCanvas()
			: Application("RougeCanvas")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~RougeCanvas()
		{

		}
	};

	Application* CreateApplication()
	{
		return new RougeCanvas();
	}
}