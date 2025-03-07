#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(System)

	void System::OnInitialize()
	{
		Application::GetInstance()->RegisterSystem(GetObjectType(), this);
		SetTickable(true);
	}

	void System::OnShutdown()
	{
		Application::GetInstance()->UnregisterSystem(GetObjectType());
	}
}
