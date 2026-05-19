#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Systems/System.h"

namespace NxEn
{
	NX_OBJECT_IMPLEMENTATION(System)

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
