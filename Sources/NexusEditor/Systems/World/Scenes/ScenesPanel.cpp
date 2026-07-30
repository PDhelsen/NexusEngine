#include "NexusEditor/Systems/World/Scenes/ScenesPanel.h"

#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEd
{
	static NxEn::SettingVar<bool>* SettingLoadSingle = NxEn::SettingVar<bool>::Create("Editor", "Scene - Load scene in single mode", true);

	static ScenesPanel* Panel = NxEn::GUI::Panel::Create<ScenesPanel>();

	static const NxEn::GUI::Menu::Item* MenuItemScenes = NxEn::GUI::Menu::Item::Create("Object/World/Scenes", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel ScenesPanel");
	}));

	void ScenesPanel::Refresh()
	{
		NxFr::Array<NxFr::GUID> Scenes = Assets->Find("t:Scene");
		ScenesInstances = Scenes.GetCount();
		for (uint64 Index = 0; Index < Scenes.GetCount(); ++Index)
		{
			NxFr::GUID SceneId = Scenes[Index];
			ScenesInstances[Index] = SceneInfo
			{
				.Id = SceneId,
				.Path = Assets->IdToPath(SceneId)
			};
		}

		NxFr::GUID TargetWorldId = NxEn::WorldSystem::MainWorldId;
		if (WorldsInstances.IsValidIndex(WorldIndex))
		{
			TargetWorldId = GetWorldId();
		}

		WorldsInstances = Worlds->GetWorlds();
		auto WorldIterator = NxFr::ContainerUtility::Find(WorldsInstances, TargetWorldId);
		WorldIndex = WorldIterator != WorldsInstances.End() ? WorldIterator.Id() : 0;
		NxFr::Array<NxFr::StringView> WorldsLabel = WorldsInstances.GetCount();
		for (uint64 Index = 0; Index < WorldsInstances.GetCount(); ++Index)
		{
			NxFr::GUID WorldId = WorldsInstances[Index];
			WorldsLabel[Index] = Worlds->GetWorld(WorldId)->GetName();
		}

		Menu.Remove("Worlds/");
		Menu.AddMenuEnum("Worlds/", &WorldIndex, WorldsLabel, nullptr, 2);
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
		Worlds->GetOnWorldChange() += { this, &ScenesPanel::OnScenesChanged };

		Panel::OnEnable();
		Menu.SetEnabled(true);
		Style.Reset();

		Refresh();
	}

	void ScenesPanel::OnDisable()
	{
		Menu.SetEnabled(false);
		Panel::OnDisable();

		Worlds->GetOnWorldChange() -= { this, &ScenesPanel::OnScenesChanged };
		Worlds = nullptr;
		Assets = nullptr;
	}

	void ScenesPanel::OnDraw()
	{
		static const float Width = 100.0f;

		Style.Position.x = -1.0f;
		Menu.Draw();

		int64 ActionIndex = -1;
		Action ActionType = Action::None;

		float Origin = ImGui::GetCursorPosX();
		float Offset = NxEn::GUI::Utils::Fill().x - Width * 2.0f;

		NxFr::GUID WorldId = GetWorldId();
		for (uint64 Index = 0; Index < ScenesInstances.GetCount(); ++Index)
		{
			const SceneInfo& Info = ScenesInstances[Index];

			Style.Position.x = Origin;
			NxEn::GUI::Draw::Label(Info.Path, &Style);

			ImGui::SameLine();

			Style.Position.x = Origin + Offset;
			Style.Width = Width;
			bool IsLoaded = IsSceneLoaded(Info.Id, WorldId);
			if (NxEn::GUI::Draw::Button((IsLoaded ? "Unload" : "Load") + NxFr::StringView("##") + NxFr::StringUtility::ToString(Info.Id), &Style))
			{
				ActionIndex = Index;
				ActionType = Action::Toggle;
			}

			if (IsLoaded)
			{
				ImGui::SameLine();

				Style.Position.x += Width;
				if (NxEn::GUI::Draw::Button("Save" + NxFr::StringView("##") + NxFr::StringUtility::ToString(Info.Id), &Style))
				{
					ActionIndex = Index;
					ActionType = Action::Save;
				}
			}
		}

		if (ActionIndex >= 0)
		{
			switch (ActionType)
			{
			case Action::Toggle: Toggle(ScenesInstances[ActionIndex]); break;
			case Action::Save: Save(ScenesInstances[ActionIndex]); break;
			}
		}
	}

	void ScenesPanel::OnScenesChanged(NxFr::StringId Action, NxFr::GUID SceneId, bool IsScene)
	{
		Refresh();
	}

	void ScenesPanel::Create()
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			Assets->Create<NxEn::Scene>(Input, NxEn::Scene::Extension);
			Refresh();
		});
	}

	void ScenesPanel::Save(const SceneInfo& Info)
	{
		NxEn::Scene* SceneInstance = Assets->GetAsset<NxEn::Scene>(Info.Id);
		Worlds->PackScene(SceneInstance, GetWorldId());
		Assets->Save(SceneInstance->GetId());
	}

	void ScenesPanel::Toggle(const SceneInfo& Info)
	{
		NxFr::GUID WorldId = GetWorldId();
		if (IsSceneLoaded(Info.Id, WorldId))
		{
			Unload(Info);
		}
		else
		{
			Load(Info);
		}
	}

	void ScenesPanel::Load(const SceneInfo& Info)
	{
		NxEn::Scene* SceneInstance = Assets->Acquire<NxEn::Scene>(Info.Id);
		if (SettingLoadSingle->GetValue())
		{
			Worlds->InstantiateSceneSingle(SceneInstance, GetWorldId());
		}
		else
		{
			Worlds->InstantiateScene(SceneInstance, GetWorldId());
		}
	}

	void ScenesPanel::Unload(const SceneInfo& Info)
	{
		NxEn::Scene* SceneInstance = Assets->GetAsset<NxEn::Scene>(Info.Id);
		Worlds->DestroyScene(SceneInstance, GetWorldId());
		Assets->Release(SceneInstance->GetId());
	}
}
