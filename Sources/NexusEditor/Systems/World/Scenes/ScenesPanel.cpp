#include "NexusEditor/Systems/World/Scenes/ScenesPanel.h"

#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEd
{
	static NxEn::SettingVar<bool>* SettingLoadSingle = NxEn::SettingVar<bool>::Create("Editor", "Scene - Load scene in single mode", true);

	static ScenesPanel* Panel = NxEn::GUI::Panel::Create<ScenesPanel>();

	static const NxEn::GUI::Menu::Item* MenuItemScenes = NxEn::GUI::Menu::Create("Object/World/Scenes", NxFr::Delegate<void()>([]()
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

		SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);
		SetNameId("Scenes");

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
		NxEn::GUI::Transform Visual = NxEn::GUI::Transform(-NxFr::Vector2f::One, NxFr::Vector2f(NxEn::GUI::Styles::WidthButton(), 0.0f), -1.0f);
		Menu.Draw();

		int64 ActionIndex = -1;
		Action ActionType = Action::None;

		float Origin = ImGui::GetCursorPosX();
		float Offset = NxEn::GUI::Utils::Fill(NxFr::Vector2f(Visual.Size.x * 2.0f, 0), 2).x;

		NxFr::GUID WorldId = GetWorldId();
		for (uint64 Index = 0; Index < ScenesInstances.GetCount(); ++Index)
		{
			const SceneInfo& Info = ScenesInstances[Index];

			Visual.Position.x = Origin;
			NxEn::GUI::Draw::Label(Info.Path, Visual);

			ImGui::SameLine();

			Visual.Position.x = Origin + Offset + ImGui::GetStyle().ItemSpacing.x;
			bool IsLoaded = IsSceneLoaded(Info.Id, WorldId);
			if (NxEn::GUI::Draw::Button(NxEn::GUI::Utils::NexusToImGuiId(IsLoaded ? "Unload" : "Load", Info.Id), Visual))
			{
				ActionIndex = Index;
				ActionType = Action::Toggle;
			}

			if (IsLoaded)
			{
				ImGui::SameLine();

				Visual.Position.x = -1.0f;
				if (NxEn::GUI::Draw::Button(NxEn::GUI::Utils::NexusToImGuiId("Save", Info.Id), Visual))
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
		NxEn::InputTextPopup* Popup = NxEn::Application::GetSystem<NxEn::GUISystem>()->TryReuseElement<NxEn::InputTextPopup>();
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
