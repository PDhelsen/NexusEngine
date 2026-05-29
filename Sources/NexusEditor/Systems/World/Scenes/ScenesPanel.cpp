#include "NexusEditor/Systems/World/Scenes/ScenesPanel.h"

#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"
#include "NexusEngine/Systems/Settings/SettingTemplate.h"

namespace NxEd
{
	static NxEn::SettingVar<bool>* SettingLoadSingle = NxEn::SettingVar<bool>::Create("Editor", "Scene - Load scene in single mode", true);

	static ScenesPanel* Panel = NxEn::GUI::Panel::Create<ScenesPanel>();

	const static NxEn::GUI::Menu::Item MenuItemScenes = NxEn::GUI::Menu::Item::Create("Object/World/Scenes", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel ScenesPanel");
	}));

	void ScenesPanel::Refresh()
	{
		NxFr::Array<NxFr::GUID> ScenesIds = Assets->Find("t:Scene");
		ScenesInfos = ScenesIds.GetCount();
		for (uint64 Index = 0; Index < ScenesIds.GetCount(); ++Index)
		{
			NxFr::GUID Id = ScenesIds[Index];
			ScenesInfos[Index] = Info{
				.Id = Id,
				.Path = Assets->IdToPath(Id),
				.World = Worlds->IsSceneLoaded(Id)
			};
		}

		WorldsIds = Worlds->GetWorlds();
		WorldsIndex = NxFr::ContainerUtility::Find(WorldsIds, NxEn::WorldSystem::WorldId.GetId()).Id();
		NxFr::Array<NxFr::StringView> WorldsLabel = WorldsIds.GetCount();
		for (uint64 Index = 0; Index < WorldsIds.GetCount(); ++Index)
		{
			WorldsLabel[Index] = Worlds->GetWorld(WorldsIds[Index])->GetName();
		}

		Menu.Remove("Worlds/");
		Menu.AddMenuEnum("Worlds/", &WorldsIndex, WorldsLabel, nullptr, 2);
	}

	void ScenesPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetGuiFlag(ImGuiWindowFlags_MenuBar);
		SetTitle("Scenes");

		Menu.AddMenuItem("Refresh", { this, &ScenesPanel::Refresh }, 0);
		Menu.AddMenuItem("Create", { this, &ScenesPanel::Create }, 1);
		Menu.AddMenuToggle("Load Single", &SettingLoadSingle->GetValue(), nullptr, 3);
	}

	void ScenesPanel::OnEnable()
	{
		Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Worlds->GetOnSceneEvent() += { this, &ScenesPanel::OnScenesChanged };

		Panel::OnEnable();
		Menu.SetEnabled(true);
		Style.Reset();

		Refresh();
	}

	void ScenesPanel::OnDisable()
	{
		Menu.SetEnabled(false);
		Panel::OnDisable();

		Worlds->GetOnSceneEvent() -= { this, &ScenesPanel::OnScenesChanged };
		Worlds = nullptr;
		Assets = nullptr;
	}

	void ScenesPanel::OnDraw()
	{
		static const float Width = 100.0f;

		Style.Position.x = -1.0f;
		Menu.Draw();

		int32 Toggle = -1;
		float Origin = ImGui::GetCursorPosX();
		float Offset = NxEn::GUI::Utils::Fill().x - Width * 2.0f;

		for (uint64 Index = 0; Index < ScenesInfos.GetCount(); ++Index)
		{
			Info& Info = ScenesInfos[Index];

			Style.Position.x = Origin;
			NxEn::GUI::Draw::Label(Info.Path, &Style);
			ImGui::SameLine();

			Style.Position.x = Origin + Offset;
			bool IsLoaded = Info.IsLoaded();
			NxEn::GUI::Draw::Label(IsLoaded ? Info.World.GetString() : "None", &Style);
			ImGui::SameLine();

			Style.Position.x = Origin + Offset + Width;
			Style.SetPosition();
			if (NxEn::GUI::Draw::Button((IsLoaded ? "Unload" : "Load") + NxFr::StringView("##") + NxFr::StringUtility::ToString(Info.Id), NxFr::Vector2f(Width, 0.0f)))
			{
				Toggle = Index;
			}
		}

		if (Toggle >= 0)
		{
			OnToggle(Toggle);
		}
	}

	void ScenesPanel::OnScenesChanged(NxFr::StringId, NxFr::GUID, NxFr::GUID)
	{
		Refresh();
	}

	void ScenesPanel::OnToggle(uint64 Index)
	{
		Info& Info = ScenesInfos[Index];
		if (Info.IsLoaded())
		{
			Unload(Info.Id);
		}
		else
		{
			Load(Info.Id);
		}
	}

	void ScenesPanel::Create()
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			Worlds->CreateScene(Input, GetWorld());
		});
	}

	void ScenesPanel::Load(NxFr::GUID SceneId)
	{
		if (SettingLoadSingle->GetValue())
		{
			Worlds->LoadSceneSingle(SceneId, GetWorld());
		}
		else
		{
			Worlds->LoadScene(SceneId, GetWorld());
		}
	}

	void ScenesPanel::Unload(NxFr::GUID SceneId)
	{
		Worlds->UnloadScene(SceneId);
	}
}
