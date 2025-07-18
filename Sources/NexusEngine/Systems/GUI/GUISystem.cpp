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
		ImGui::Initialize(Window);
	}

	void GUISystem::OnShutdown()
	{
		ImGui::Shutdown();
		
		System::OnShutdown();
	}

	void GUISystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		ImGui::Frame();
		ImGui::Render();
	}
}
