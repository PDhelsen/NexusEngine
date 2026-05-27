#include "NexusEditor/Systems/Object/Stage/StageManager.h"
#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	const static NxEn::GUI::Menu::Item MenuItemStage = NxEn::GUI::Menu::Item::Create("Object/World/Stage", NxFr::Delegate<void()>([]()
	{
		NxEn::World* World = NxEn::Application::GetSystem<NxEn::WorldSystem>()->GetWorld();
		StageManager* Stages = NxEn::Application::GetInstance<NexusEditorApplication>()->GetStageManager();

		Stage* StageView = Stages->GetStage(World);
		if (!StageView)
		{
			StageView = Stages->CreateStage(World);
		}
		Stages->ShowStage(World);
	}));

	const static NxEn::Command CmdStageShowAsset = NxEn::Command::Create("Stage.Show.Asset"_Sid, "Show asset on stage", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxFr::GUID Id = Assets->PathToId(Path);
		NxEn::Asset* Target = Assets->Load(Id);

		StageManager* Stages = NxEn::Application::GetInstance<NexusEditorApplication>()->GetStageManager();
		Stage* Instance = Stages->GetStage(Target);
		if (!Instance)
		{
			Instance = Stages->CreateStage(Target);
		}
		Stages->ShowStage(Target);
	}));

	const static NxEn::Command CmdStageShowGameObject = NxEn::Command::Create("Stage.Show.GameObject"_Sid, "Show gameobject on stage", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Query)
	{
		StageManager* Stages = NxEn::Application::GetInstance<NexusEditorApplication>()->GetStageManager();
		Stage* Instance = Stages->GetFocusedStage();

		NxEn::World* World = Instance->GetWorld();
		if (!World)
		{
			return;
		}

		Instance->GetHierarchy()->Find(Query);

		NxFr::Array<NxFr::Handle<NxEn::GameObject>> Selection = World->Find(Query);
		if (Selection.GetCount())
		{
			Instance->GetInspector()->Show(Selection[0]);
		}
	}));

	StageManager::StageManager()
		: Stages()
	{
		NxEn::World* World = NxEn::Application::GetSystem<NxEn::WorldSystem>()->GetWorld();

		CreateStage(World);
		ShowStage(World);
	}

	StageManager::~StageManager()
	{
		while (Stages.GetCount())
		{
			DestroyStage(Stages.Begin().Get().Key);
		}
	}

	Stage* StageManager::CreateStage(NxEn::Object* Target)
	{
		Stage* Instance = GetStage(Target);
		if (Instance)
		{
			return Instance;
		}

		Instance = new Stage(Target);
		Instance->Main = Target == NxEn::Application::GetSystem<NxEn::WorldSystem>()->GetWorld();
		Instance->Initialize();

		Stages.Append(Target, Instance);
		return Instance;
	}

	void StageManager::ShowStage(NxEn::Object* Target)
	{
		Stage* Instance = GetStage(Target);
		if (!Instance)
		{
			return;
		}

		Instance->Show();
	}

	void StageManager::DestroyStage(NxEn::Object* Target)
	{
		Stage* Instance = GetStage(Target);
		if (!Instance)
		{
			return;
		}

		Stages.Remove(Target);

		Instance->Hide();
		Instance->Shutdown();
		delete Instance;
	}

	void StageManager::DestroyDisableStage()
	{
		NxFr::List<NxEn::Object*> ToRemove = Stages.GetCount();

		for (auto [Target, Instance] : Stages)
		{
			if (!Instance->IsVisible())
			{
				ToRemove.Append(Target);
			}
		}

		for (auto Target : ToRemove)
		{
			DestroyStage(Target);
		}
	}

	Stage* StageManager::GetStage(NxEn::Object* Target) const
	{
		auto Instance = Stages.TryGet(Target);
		return Instance ? *Instance : nullptr;
	}

	Stage* StageManager::GetMainStage() const
	{
		return GetStage(NxEn::Application::GetSystem<NxEn::WorldSystem>()->GetWorld());
	}

	Stage* StageManager::GetFocusedStage() const
	{
		for (auto [Id, Instance] : Stages)
		{
			if (Instance->IsFocused())
			{
				return Instance;
			}
		}

		return GetMainStage();
	}
}
