#include "NexusEditor/Systems/Object/Stage/Stage.h"

#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Core/NexusEditorApplication.h"
#include "NexusEditor/Systems/Object/Viewer/Contexts/ViewerContext3D.h"

namespace NxEd
{
	Stage::Stage(NxEn::Object* Target)
		: Target(Target), World(nullptr), Dock(), Main(false), Viewer(nullptr), Inspector(nullptr), Hierarchy(nullptr)
	{
		if (Target->GetObjectType() == NxEn::World::GetClassType())
		{
			World = static_cast<NxEn::World*>(Target);
		}
		else
		{
			NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
			World = Worlds->CreateWorld(Target->GetName());
			if (Target->GetObjectType() == NxEn::Prefab::GetClassType())
			{
				NxEn::Prefab* Instance = static_cast<NxEn::Prefab*>(Target);
				Worlds->InstantiateGameObject(Instance->GetRoot(), World->GetRoot(), World->GetId());
			}
			else if (Target->GetObjectType() == NxEn::Scene::GetClassType())
			{
				NxEn::Scene* Instance = static_cast<NxEn::Scene*>(Target);
				Worlds->InstantiateScene(Instance, World->GetId());
			}
		}
	}

	Stage::~Stage()
	{
		
	}

	bool Stage::IsVisible() const
	{
		return Element::IsEnabled() && (Viewer->IsEnabled() || Inspector->IsEnabled() || Hierarchy->IsEnabled());
	}

	bool Stage::IsFocused() const
	{
		return Element::IsFocused() && (Viewer->IsFocused() || Inspector->IsFocused() || Hierarchy->IsFocused());
	}

	bool Stage::IsMain() const
	{
		return Main;
	}

	void Stage::OnInitialize()
	{
		Element::OnInitialize();

		SetGuiFlag(NxEn::GUI::ElementFlags::HideInsteadOfClose, true);
		SetNameId(Target->GetName(), Target->GetId());

		Dock.Initialize();
		Dock.SetGuiFlag(NxEn::GUI::ElementFlags::AutoDraw, false);
		Dock.SetNameId(Target->GetName(), Target->GetId());

		Viewer = new ViewerPanel();
		Viewer->Initialize();
		Viewer->SetGuiFlag(NxEn::GUI::ElementFlags::AutoDraw, false);
		Viewer->SetNameId("Viewer", Target->GetId());

		Inspector = new InspectorPanel();
		Inspector->Initialize();
		Inspector->SetGuiFlag(NxEn::GUI::ElementFlags::AutoDraw, false);
		Inspector->SetNameId("Inspector", Target->GetId());

		Hierarchy = NxEn::Application::GetInstance<NexusEditorApplication>()->GetHierarchyManager()->CreatePanel(World);
		Hierarchy->SetGuiFlag(NxEn::GUI::ElementFlags::AutoDraw, false);
		Hierarchy->SetNameId("Hierarchy", +Target->GetId());
	}

	void Stage::OnShutdown()
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->GetHierarchyManager()->DestroyPanel(World);

		Inspector->Shutdown();
		delete Inspector;

		Viewer->Shutdown();
		delete Viewer;

		Dock.Shutdown();

		Element::OnShutdown();
	}

	void Stage::OnEnable()
	{
		Element::OnEnable();

		Dock.Show();
		if (Target->GetObjectType() == NxEn::World::GetClassType() || Target->GetObjectType() == NxEn::Scene::GetClassType() || Target->GetObjectType() == NxEn::Prefab::GetClassType())
		{
			Viewer->Show(World);
			Inspector->Show(World->GetRoot());
		}
		else
		{
			Viewer->Show(Target);
			Inspector->Show(Target);
		}
		Hierarchy->Show();

		Dock.DockElement(Viewer);
		Dock.DockElement(Inspector, ImGuiDir_Right, 0.25f);
		Dock.DockElement(Hierarchy, ImGuiDir_Left, 0.25f);

		EditSystem* Edit = NxEn::Application::GetSystem<EditSystem>();
		Edit::Context* Ctx = Edit->GetContext(Target->GetId());
		if (Ctx)
		{
			Ctx->GetOnSelectionChanged() += { this, & Stage::OnSelectionChanged };
		}
	}

	void Stage::OnDisable()
	{
		EditSystem* Edit = NxEn::Application::GetSystem<EditSystem>();
		Edit::Context* Ctx = Edit->GetContext(Target->GetId());
		if (Ctx)
		{
			Ctx->GetOnSelectionChanged() -= { this, & Stage::OnSelectionChanged };
		}

		Hierarchy->Hide();
		Inspector->Hide();
		Viewer->Hide();
		Dock.Hide();

		Element::OnDisable();
	}

	void Stage::OnDraw()
	{
		Dock.Draw();
		Viewer->Draw();
		Inspector->Draw();
		Hierarchy->Draw();
	}

	void Stage::OnSelectionChanged(NxFr::GUID Id, bool State)
	{
		NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxFr::Handle<NxEn::GameObject> Instance = Worlds->GetObject(Id, World->GetId());
		if (State && Instance)
		{
			Inspector->Show(Instance);
		}
	}
}
