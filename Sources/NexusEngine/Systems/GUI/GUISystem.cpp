#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUISystem.h"

#include "NexusEngine/External/ImGui.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(GUISystem)

	GUISystem::GUISystem()
		: OnGui()
	{

	}

	GUISystem::~GUISystem()
	{
	}

	void GUISystem::LoadConfig(NxFr::StringView Name)
	{
		ImGui::LoadConfig(GetConfigPath(Name));
	}

	void GUISystem::SaveConfig(NxFr::StringView Name)
	{
		ImGui::SaveConfig(GetConfigPath(Name));
	}

	NxFr::Path GUISystem::GetConfigPath(NxFr::StringView Name)
	{
		NxFr::String FullName = Name.ToString();

		if (FullName.IsEmpty())
		{
			FullName = "imgui";
#if NEXUS_EDITOR
			FullName += "_editor";
#else
			FullName += "_app";
#endif
		}

		FullName += ".ini";

		NxFr::String Directory = NxFr::Path::Combine(NxFr::Paths::Configs, "ImGui");
		NxFr::Directory(Directory).Create();

		return NxFr::Path(Directory) + FullName;
	}

	void GUISystem::OnInitialize()
	{
		System::OnInitialize();

		void* Window = Application::GetSystem<WindowSystem>()->GetNativeWindow();

		ImGui::Initialize(Window);
		LoadConfig();
	}

	void GUISystem::OnShutdown()
	{
		SaveConfig();
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
