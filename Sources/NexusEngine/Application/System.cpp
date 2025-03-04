#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/System.h"

#include "NexusEngine/Application/Application.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(System)

	void System::OnInitialize()
	{
		Application::GetInstance()->RegisterSystem(GetObjectType(), this);
	}

	void System::OnShutdown()
	{
		Application::GetInstance()->UnregisterSystem(GetObjectType());
	}
}
