#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Systems/System.h"

namespace NxEn
{
	System::System()
	{
	}

	System::~System()
	{
	}

	void System::OnInitialize()
	{
		SetTickable(true);
	}

	void System::OnShutdown()
	{
	}

	void System::OnTick(float TimeStep)
	{
	}
}
