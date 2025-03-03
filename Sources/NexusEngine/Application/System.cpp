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
	}

	void System::OnShutdowm()
	{
	}

	void System::OnTick()
	{
	}

	void System::OnEnable()
	{
	}

	void System::OnDisable()
	{
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
