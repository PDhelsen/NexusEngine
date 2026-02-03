#include "NexusEditor/Systems/Object/Stage/StageManager.h"
#include "NexusEditor/Systems/Editor/EditorSystem.h"

namespace NxEd
{
	const static NxEn::GUI::Menu::Item MenuItemStage = NxEn::GUI::Menu::Item::Create("Object/Stages/Main", NxFr::Delegate<void()>([]()
	{
		NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		EditorSystem* Editor = NxEn::Application::GetSystem<EditorSystem>();

		Editor->GetStageManager().ShowStage(Worlds->GetWorld());
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
		Stage* Instance = new Stage(Target);
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

		return nullptr;
	}
}
