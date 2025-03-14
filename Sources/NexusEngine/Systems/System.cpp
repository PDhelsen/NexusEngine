#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(System)

	uint64 FrameCout;

	void System::OnInitialize()
	{
		Application::GetInstance()->RegisterSystem(GetObjectType(), this);
		SetTickable(true);
	}

	void System::OnShutdown()
	{
		Application::GetInstance()->UnregisterSystem(GetObjectType());
	}

	void System::OnTick(float TimeStep)
	{
		NEXUS_LOG(Info, Default, "Tick %.2f - %s", TimeStep, GetObjectType().C());

		if (++FrameCout > 10)
		{
			Application::GetInstance()->Quit();
		}
	}
}
