#include "NexusApp/Core/NexusAppEntryPoint.h"

#include "NexusApp/Application/ApplicationApp.h"

namespace NxAp
{
	namespace EntryPoint
	{
		NxEn::Application* CreateApplication()
		{
			return new ApplicationApp();
		}
	}
}
