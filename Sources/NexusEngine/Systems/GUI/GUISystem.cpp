#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUISystem.h"

#include "NexusEngine/External/ImGui.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(GUISystem)

	GUISystem::GUISystem()
		: OnGui(), Config("")
	{

	}

	GUISystem::~GUISystem()
	{
	}

	void GUISystem::OnInitialize()
	{
		System::OnInitialize();

		Config = NxFr::Paths::Configs + "imgui.ini";
		void* Window = Application::GetSystem<WindowSystem>()->GetNativeWindow();
		ImGui::Initialize(Window, Config);
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
		OnGui.Invoke(TimeStep);
		ImGui::Render();
	}
}
