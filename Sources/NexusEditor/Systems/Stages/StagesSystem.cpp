#include "NexusEditor/Systems/Stages/StagesSystem.h"

namespace NxEd
{
	static NxEn::Command* CmdStageShowAsset = NxEn::Command::Create("Stage.Show.Asset"_Sid, "Show asset on stage", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxFr::GUID Id = Assets->PathToId(Path);
		NxEn::Asset* Target = Assets->Load(Id);

		StagesSystem* Stages = NxEn::Application::GetSystem<StagesSystem>();
		Stages->CreateStage(Target);
	}));

	static NxEn::Command* CmdStageShowWorld = NxEn::Command::Create("Stage.Show.World"_Sid, "Show world on stage", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Id)
	{
		NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxEn::World* Target = Worlds->GetWorld(NxFr::StringId(Id));

		StagesSystem* Stages = NxEn::Application::GetSystem<StagesSystem>();
		Stages->CreateStage(Target);
	}));

	static NxEn::Command* CmdStageShowGameObject = NxEn::Command::Create("Stage.Show.GameObject"_Sid, "Show gameobject on stage", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Id)
	{
		NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Handle<NxEn::Object> Target = Worlds->GetObject(NxFr::StringUtility::FromString<NxFr::GUID>(Id));
		NxFr::Handle<NxEn::GameObject> Instance = Worlds->Cast<NxEn::GameObject>(Target);

		StagesSystem* Stages = NxEn::Application::GetSystem<StagesSystem>();
		Stages->GetFocusedStage()->Select(Instance);
	}));

	StagePanel* StagesSystem::CreateStage(NxEn::ObjectInstance<NxEn::Object> Instance)
	{
		if (Instance->GetId() == MainStage->GetTarget()->GetId())
		{
			MainStage->Show();
			return MainStage;
		}

		StagePanel** Handle = Stages.TryGet(Instance->GetId());
		if (Handle)
		{
			StagePanel* Panel = *Handle;
			Panel->Show();
			return Panel;
		}

		StagePanel* Panel = new StagePanel();
		Panel->Initialize(Instance);
		Panel->Show();

		Stages.Append(Instance->GetId(), Panel);
		return Panel;
	}

	void StagesSystem::DestroyStage(NxFr::GUID Id)
	{
		if (Id == MainStage->GetTarget()->GetId())
		{
			return;
		}

		StagePanel** Handle = Stages.TryGet(Id);
		if (!Handle)
		{
			return;
		}

		StagePanel* Panel = *Handle;
		Stages.Remove(Id);

		Panel->Hide();
		Panel->Shutdown();
		delete Panel;
	}

	StagePanel* StagesSystem::GetStage(NxFr::GUID Id)
	{
		if (Id == MainStage->GetTarget()->GetId())
		{
			return MainStage;
		}

		StagePanel** Handle = Stages.TryGet(Id);
		return Handle ? *Handle : nullptr;
	}

	StagePanel* StagesSystem::GetFocusedStage()
	{
		for (auto [Id, Instance] : Stages)
		{
			if (Instance->IsFocused())
			{
				return Instance;
			}
		}

		return MainStage;
	}

	StagePanel* StagesSystem::GetMainStage()
	{
		return MainStage;
	}

	void StagesSystem::OnInitialize()
	{
		System::OnInitialize();

		MainStage = NxEn::Application::GetSystem<NxEn::GUISystem>()->GetPanel<StagePanel>();
	}

	void StagesSystem::OnShutdown()
	{
		for (auto [Id, Panel] : Stages)
		{
			DestroyStage(Id);
		}

		MainStage->Hide();
		MainStage->Shutdown();

		System::OnShutdown();
	}

	void StagesSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		if (!MainStage->AreElementsEnabled())
		{
			MainStage->Hide();
		}

		for (auto [Id, Panel] : Stages)
		{
			if (!Panel->AreElementsEnabled())
			{
				DestroyStage(Id);
			}
		}
	}
}
