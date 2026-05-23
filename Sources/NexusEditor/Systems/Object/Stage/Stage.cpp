#include "NexusEditor/Systems/Object/Stage/Stage.h"

#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Core/NexusEditorApplication.h"
#include "NexusEditor/Systems/Object/Viewer/Contexts/ViewerContext3D.h"

namespace NxEd
{
	static NxFr::Vector2f DockDefaultPos = NxFr::Vector2f(200.0f, 200.0f);
	static NxFr::Vector2f DockDefaultSize = NxFr::Vector2f(1600.0f, 800.0f);

	NX_OBJECT_IMPLEMENTATION(Stage)

	Stage::Stage(NxEn::Object* Target)
		: Target(Target), DockId(0), Layout(false), Main(false), Viewer(nullptr), Inspector(nullptr), Hierarchy(nullptr)
	{
	}

	Stage::~Stage()
	{
		
	}

	bool Stage::IsVisible() const
	{
		return IsEnabled() && (Viewer->IsEnabled() || Inspector->IsEnabled() || Hierarchy->IsEnabled());
	}

	bool Stage::IsFocused() const
	{
		ImGuiContext* Ctx = ImGui::GetCurrentContext();
		if (!Ctx || !Ctx->NavWindow)
			return false;

		NxFr::StringView Focused = Ctx->NavWindow->Name;
		return Focused == Viewer->GetImGuiId() || Focused == Inspector->GetImGuiId() || Focused == Hierarchy->GetImGuiId();
	}

	bool Stage::IsMain() const
	{
		return Main;
	}

	void Stage::OnInitialize()
	{
		Element::OnInitialize();

		NxFr::String IdString = NxFr::StringUtility::ToString(Target->GetId());
		UpdateImGuiId(Target->GetName() + "##" + IdString);

		Viewer = new ViewerPanel();
		Viewer->Initialize();
		Viewer->SetTitle("Viewer##" + IdString);
		Viewer->SetManual(true);

		Inspector = new InspectorPanel();
		Inspector->Initialize();
		Inspector->SetTitle("Inspector##" + IdString);
		Inspector->SetManual(true);

		Hierarchy = NxEn::Application::GetInstance<NexusEditorApplication>()->GetHierarchyManager().CreatePanel();
		Hierarchy->SetTitle("Hierarchy##" + IdString);
		Hierarchy->SetManual(true);
	}

	void Stage::OnShutdown()
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->GetHierarchyManager().DestroyPanel(Hierarchy);

		Inspector->Shutdown();
		delete Inspector;

		Viewer->Shutdown();
		delete Viewer;

		Element::OnShutdown();
	}

	void Stage::OnEnable()
	{
		Element::OnEnable();

		Viewer->Show(Target);
		Inspector->Show(Target);
		if (GetWorld())
		{
			Hierarchy->Show(GetWorld()->GetRootGameObject());
		}

		EditSystem* Edit = NxEn::Application::GetSystem<EditSystem>();
		Edit::Context* Ctx = Edit->GetContext(Hierarchy->GetImGuiId());
		if (Ctx)
		{
			Ctx->GetOnSelectionChanged() += { this, & Stage::OnSelectionChanged };
		}

		Layout = true;
	}

	void Stage::OnDisable()
	{
		EditSystem* Edit = NxEn::Application::GetSystem<EditSystem>();
		Edit::Context* Ctx = Edit->GetContext(Hierarchy->GetImGuiId());
		if (Ctx)
		{
			Ctx->GetOnSelectionChanged() -= { this, & Stage::OnSelectionChanged };
		}

		if (GetWorld())
		{
			Hierarchy->Hide();
		}
		Inspector->Hide();
		Viewer->Hide();

		Element::OnDisable();
	}

	void Stage::OnGui(float TimeStep)
	{
		DrawDocking();
		DockPanels();

		Viewer->Tick(TimeStep);
		Inspector->Tick(TimeStep);
		Hierarchy->Tick(TimeStep);
	}

	void Stage::OnSelectionChanged(NxFr::GUID Id, bool State)
	{
		NxEn::World* World = GetWorld();
		if (!World)
		{
			return;
		}

		if (State)
		{
			Inspector->Show(World->GetGameObject(Id));
		}
		else
		{
			Inspector->Show(Target);
		}
	}

	void Stage::DrawDocking()
	{
		ImGui::SetNextWindowPos(DockDefaultPos, Main ? ImGuiCond_FirstUseEver : ImGuiCond_Once);
		ImGui::SetNextWindowSize(DockDefaultSize, Main ? ImGuiCond_FirstUseEver : ImGuiCond_Once);

		bool IsOpen = IsVisible();
		DockId = ImGui::GetID(GetImGuiId().C());

		ImGui::Begin(GetImGuiId().C(), &IsOpen);
		ImGui::DockSpace(DockId, NxFr::Vector2f::Zero, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDocking);
		ImGui::End();

		if (!IsOpen)
		{
			Hide();
		}
	}

	void Stage::DockPanels()
	{
		if (!Layout)
		{
			return;
		}
		Layout = false;

		ImGuiID HierarchyId, ViewerId, InspectorId;

		ImGui::DockBuilderRemoveNode(DockId);
		ImGui::DockBuilderAddNode(DockId, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(DockId, DockDefaultSize);

		ImGui::DockBuilderSplitNode(DockId, ImGuiDir_Left, 0.5f, &ViewerId, &InspectorId);
		ImGui::DockBuilderDockWindow(Viewer->GetImGuiId().C(), ViewerId);
		ImGui::DockBuilderDockWindow(Inspector->GetImGuiId().C(), InspectorId);

		if (Hierarchy->IsEnabled())
		{
			ImGui::DockBuilderSplitNode(DockId, ImGuiDir_Left, 0.25f, &HierarchyId, &ViewerId);
			ImGui::DockBuilderDockWindow(Hierarchy->GetImGuiId().C(), HierarchyId);
		}

		ImGui::DockBuilderFinish(DockId);
	}
}
