#include "NexusEditor/Systems/World/Scene/ScenesManager.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEngine/Systems/Settings/SettingTemplate.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	const static NxEn::GUI::Menu::Item MenuItemSceneRefresh = NxEn::GUI::Menu::Item::Create("Object/Scene/Refresh", &ScenesManager::Refresh);
	const static NxEn::GUI::Menu::Item MenuItemSceneCreate = NxEn::GUI::Menu::Item::Create("Object/Scene/Create", &ScenesManager::Create);

	static NxEn::SettingVar<bool>* SettingUnloadBeforeLoading = NxEn::SettingVar<bool>::Create("Editor", "Scene - Unload before loading", true);

	namespace ScenesManager
	{
		NxFr::Dictionary<NxFr::GUID, bool> Scenes;

		void Toggle_Gui(NxFr::GUID Id)
		{
			if (Scenes[Id])
			{
				Load(Id);
			}
			else
			{
				Unload(Id);
			}
		}

		void Toggle(NxFr::GUID Id)
		{
			if (!Scenes[Id])
			{
				Load(Id);
			}
			else
			{
				Unload(Id);
			}
		}

		void Load(NxFr::GUID Id)
		{
			NxEn::WorldSystem* WorldSystem = NxEn::Application::GetSystem<NxEn::WorldSystem>();
			NxFr::GUID WorldId = NxEn::GUISystem::GetPanel<HierarchyPanel>()->GetWorldId();

			if (SettingUnloadBeforeLoading->GetValue())
			{
				NxFr::Array<NxFr::GUID> Ids = WorldSystem->GetScenes(WorldId);
				for (auto I : Ids)
				{
					Unload(I);
				}

			}

			WorldSystem->LoadScene(Id, WorldId);
			Scenes[Id] = true;
		}

		void Unload(NxFr::GUID Id)
		{
			NxEn::WorldSystem* WorldSystem = NxEn::Application::GetSystem<NxEn::WorldSystem>();

			WorldSystem->UnloadScene(Id);
			Scenes[Id] = false;
		}

		void Refresh()
		{
			NxEn::AssetsSystem* AssetSystem = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
			NxEn::WorldSystem* WorldSystem = NxEn::Application::GetSystem<NxEn::WorldSystem>();

			NxEn::GUI::Menu* MainMenu = NxEn::GUISystem::GetMenu();
			MainMenu->Remove("Object/Scene/Scenes/");

			MainMenu->Remove("Object/Scene/UnloadBeforeLoading");
			MainMenu->AddMenuToggle("Object/Scene/UnloadBeforeLoading", &SettingUnloadBeforeLoading->GetValue());

			NxFr::Array<NxFr::GUID> Ids = AssetSystem->Find("t:Scene");
			Scenes = NxFr::Dictionary<NxFr::GUID, bool>(Ids.GetCount());

			for (uint64 Index = 0; Index < Ids.GetCount(); ++Index)
			{
				NxFr::GUID Id = Ids[Index];
				Scenes.Append(Id, WorldSystem->IsSceneLoaded(Id) != 0);

				NxFr::String Path = AssetSystem->IdToPath(Id);
				MainMenu->AddMenuToggle("Object/Scene/Scenes/" + NxFr::Path::Split(Path).Last(), &Scenes[Id], [=]() { Toggle_Gui(Id); });
			}
		}

		void Create()
		{
			NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
			Popup->RegisterCallback([=](NxFr::StringView Input)
			{
				NxFr::GUID WorldId = NxEn::GUISystem::GetPanel<HierarchyPanel>()->GetWorldId();
				NxEn::WorldSystem* WorldSystem = NxEn::Application::GetSystem<NxEn::WorldSystem>();
				WorldSystem->CreateScene(Input, WorldId);
			});
		}

		bool GetShouldUnloadBeforeLoading()
		{
			return SettingUnloadBeforeLoading->GetValue();
		}

		void SetShouldUnloadBeforeLoading(bool Value)
		{
			SettingUnloadBeforeLoading->GetValue() = Value;
		}
	}
}
