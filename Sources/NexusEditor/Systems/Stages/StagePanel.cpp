#include "NexusEditor/Systems/Stages/StagePanel.h"

#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	static StagePanel* Panel = NxEn::GUI::Panel::Create<StagePanel>(false);

	static const NxEn::GUI::Menu::Item* MenuItemStage = NxEn::GUI::Menu::Create("Object/Stage", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel StagePanel");
	}));

	void StagePanel::Initialize(NxEn::ObjectInstance<NxEn::Object> Target)
	{
		this->Target = Target;

		Panel::Initialize();
	}

	void StagePanel::Select(NxFr::Handle<NxEn::GameObject> Instance)
	{
		Show();

		Inspector->Show(Instance);
		if (World)
		{
			Hierarchy->Select(Instance ? Instance->GetId() : 0);
		}
	}

	void StagePanel::OnInitialize()
	{
		Panel::OnInitialize();

		Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		Edit = NxEn::Application::GetSystem<EditSystem>();

		if (Target->GetObjectType() == NxEn::World::GetClassType())
		{
			World = static_cast<NxEn::World*>(Target.Get());
		}
		else if (Target->GetObjectType() == NxEn::Prefab::GetClassType())
		{
			NxEn::Prefab* Instance = static_cast<NxEn::Prefab*>(Target.Get());
			World = Worlds->CreateWorld(Target->GetName());
			Worlds->InstantiateGameObject(Instance->GetRoot(), World->GetRoot(), World->GetId());
		}
		else if (Target->GetObjectType() == NxEn::Scene::GetClassType())
		{
			NxEn::Scene* Instance = static_cast<NxEn::Scene*>(Target.Get());
			World = Worlds->CreateWorld(Target->GetName());
			Worlds->InstantiateScene(Instance, World->GetId());
		}
		else
		{
			World = nullptr;
		}

		SetNameId(Target->GetName(), NxEn::GUI::Utils::GenerateElementId(this, Target->GetName()));

		Dock = new NxEn::GUI::Dock();
		Dock->Initialize();
		Dock->SetGuiFlag(NxEn::GUI::ElementFlags::AutoDraw, false);
		Dock->SetGuiFlag(NxEn::GUI::ElementFlags::HideInsteadOfClose, true);
		Dock->SetNameId(Target->GetName(), NxEn::GUI::Utils::GenerateElementId(Dock, Target->GetName()));

		Viewer = new ViewerPanel();
		Viewer->Initialize();
		Viewer->SetGuiFlag(NxEn::GUI::ElementFlags::AutoDraw, false);
		Viewer->SetNameId("Viewer", NxEn::GUI::Utils::GenerateElementId(Viewer, Target->GetName()));
		
		Inspector = new InspectorPanel();
		Inspector->Initialize();
		Inspector->SetGuiFlag(NxEn::GUI::ElementFlags::AutoDraw, false);
		Inspector->SetNameId("Inspector", NxEn::GUI::Utils::GenerateElementId(Inspector, Target->GetName()));

		if (World)
		{
			HierarchyManager* Manager = NxEn::Application::GetInstance<NexusEditorApplication>()->GetHierarchyManager();
			Hierarchy = new HierarchyPanel();
			Context = new HierarchyEditContext(Manager, Hierarchy, Target->GetId());
			Hierarchy->Initialize(Manager, Context);
			Hierarchy->SetGuiFlag(NxEn::GUI::ElementFlags::AutoDraw, false);
			Hierarchy->SetNameId("Hierarchy", NxEn::GUI::Utils::GenerateElementId(Hierarchy, Target->GetName()));
			Hierarchy->SetRoot(World->GetRoot()->GetId());
			Context->GetOnSelection() += { this, & StagePanel::OnSelection };
		}
	}

	void StagePanel::OnShutdown()
	{
		if (World)
		{
			Hierarchy->Shutdown();
			delete Hierarchy;
			delete Context;
		}

		Inspector->Shutdown();
		delete Inspector;
		
		Viewer->Shutdown();
		delete Viewer;

		Dock->Shutdown();
		delete Dock;

		if (World && World != Target.Get())
		{
			Worlds->DestroyWorld(World->GetId());
			World = nullptr;
		}

		Panel::OnShutdown();
	}

	void StagePanel::OnEnable()
	{
		Panel::OnEnable();

		Dock->Show();
		Viewer->Show(World ? World : Target.Get());
		Inspector->Show(Target);
		if (World)
		{
			Hierarchy->Show();
			Edit->RegisterContext(Context);
		}

		Dock->DockElement(Viewer);
		Dock->DockElement(Inspector, ImGuiDir_Right, 0.25f);
		if (World)
		{
			Dock->DockElement(Hierarchy, ImGuiDir_Left, 0.25f);
		}
	}

	void StagePanel::OnDisable()
	{
		Panel::OnDisable();

		if (World)
		{
			Edit->UnregisterContext(Context->GetId());
			Hierarchy->Hide();
		}
		Inspector->Hide();
		Viewer->Hide();
		Dock->Hide();
	}

	void StagePanel::OnDraw()
	{
		Dock->Draw();
		Viewer->Draw();
		Inspector->Draw();
		if (World)
		{
			Hierarchy->Draw();
		}
	}

	void StagePanel::OnSelection(NxFr::GUID Id, bool State)
	{
		if (!World)
		{
			return;
		}

		NxFr::Handle<NxEn::Object> Target = Worlds->GetObject(Id, World->GetId());
		NxFr::Handle<NxEn::GameObject> Instance = Worlds->Cast<NxEn::GameObject>(Target);
		if (State && Instance)
		{
			Inspector->Show(Instance);
		}
	}

	bool StagePanel::AreElementsEnabled()
	{
		return Dock->IsEnabled() &&
			(Viewer->IsEnabled() ||
			Inspector->IsEnabled() ||
			(World && Hierarchy->IsEnabled())
			);
	}
}
