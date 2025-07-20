#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUISystem.h"

#include "NexusEngine/External/ImGui.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(GUISystem)

	GUISystem::GUISystem()
	{

	}

	GUISystem::~GUISystem()
	{
	}

	void GUISystem::RegisterElement(GUI::Element* Element)
	{
		Elements.Append(Element);
	}

	void GUISystem::UnregisterElement(GUI::Element* Element)
	{
		Elements.Remove(Element);
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
		NxFr::String Directory = NxFr::Paths::Configs.ToString();

		if (Name.IsEmpty())
		{
			FullName = "imgui";
#if NEXUS_EDITOR
			FullName += "_editor";
#else
			FullName += "_app";
#endif

			Directory = NxFr::Paths::Saved.ToString();
		}

		FullName += ".ini";
		Directory = NxFr::Path::Combine(Directory.ToView(), "ImGui");

		NxFr::Directory(Directory).Create();
		return NxFr::Path(Directory) + FullName;
	}

	void GUISystem::OnInitialize()
	{
		System::OnInitialize();

		ImGui::Initialize();
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

		for (auto& Element : Elements)
		{
			if (Element->IsManual())
			{
				continue;
			}

			Element->Tick(TimeStep);
		}

		ImGui::Render();
	}
}
