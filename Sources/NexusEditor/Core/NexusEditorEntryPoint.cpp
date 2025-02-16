#include "NexusEditor/Core/NexusEditorEntryPoint.h"

#include "NexusEditor/Application/ApplicationEditor.h"

namespace NxEd
{
	namespace EntryPoint
	{
		NxEn::Application* CreateApplication()
		{
			return new ApplicationEditor();
		}
	}
}
