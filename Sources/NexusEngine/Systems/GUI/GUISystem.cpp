#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUISystem.h"

#include "NexusEngine/External/ImGui.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(GUISystem)

	GUISystem::GUISystem()
	{
	}

	GUISystem::~GUISystem()
	{
	}

	void GUISystem::OnInitialize()
	{
		System::OnInitialize();

		void* Window = Application::GetSystem<WindowSystem>()->GetNativeWindow();
		Imgui::Initialize(Window);
	}

	void GUISystem::OnShutdown()
	{
		Imgui::Shutdown();
		
		System::OnShutdown();
	}

	void GUISystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		Imgui::Frame();
		Imgui::Render();
	}
}
