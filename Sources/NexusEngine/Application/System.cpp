#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/System.h"

#include "NexusEngine/Application/Application.h"

namespace NxEn
{
	System::System()
		: Enabled(true)
	{
		Application::GetInstance()->RegisterSystem(GetSystemName(), this);
	}

	System::~System()
	{
		Application::GetInstance()->UnregisterSystem(GetSystemName());
	}

	void System::OnInitialize()
	{
		NEXUS_LOG(Info, Default, "OnInitialize");
	}

	void System::OnShutdowm()
	{
		NEXUS_LOG(Info, Default, "OnShutdowm");
	}

	void System::OnTick()
	{
		NEXUS_LOG(Info, Default, "OnTick");
	}

	void System::OnEnable()
	{
		NEXUS_LOG(Info, Default, "OnEnable");
	}

	void System::OnDisable()
	{
		NEXUS_LOG(Info, Default, "OnDisable");
	}

	void System::Initialize()
	{
		OnInitialize();
		OnEnable();
	}

	void System::Shutdown()
	{
		OnDisable();
		OnShutdowm();
	}

	void System::Tick()
	{
		if (!Enabled)
		{
			return;
		}

		OnTick();
	}
}
