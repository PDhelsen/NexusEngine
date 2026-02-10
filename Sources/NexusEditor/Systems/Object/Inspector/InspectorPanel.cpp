#include "NexusEditor/Systems/Object/Inspector/InspectorPanel.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(InspectorPanel)

	InspectorPanel::InspectorPanel()
		: Target(nullptr), Mode(InspectorMode::None), Lock(false)
	{
	}

	InspectorPanel::~InspectorPanel()
	{
	}

	void InspectorPanel::Show(NxEn::Object* Instance)
	{
		if (Lock || !Instance)
		{
			return;
		}

		Panel::Show();

		Target.Object = Instance;
		Mode = InspectorMode::Object;
	}

	void InspectorPanel::Show(NxFr::Handle<NxEn::Object> Instance)
	{
		if (Lock || !Instance)
		{
			return;
		}

		Panel::Show();

		Target.Handle = Instance;
		Mode = InspectorMode::Handle;
	}

	void InspectorPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetTitle("Inspector");
		SetGuiFlag(ImGuiWindowFlags_MenuBar);

		Menu.AddMenuToggle("Lock", &Lock);
	}

	void InspectorPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void InspectorPanel::OnEnable()
	{
		Target.Object = nullptr;
		Mode = InspectorMode::None;

		Panel::OnEnable();
		Menu.SetEnabled(true);
	}

	void InspectorPanel::OnDisable()
	{
		Menu.SetEnabled(false);
		Panel::OnDisable();

		Target.Object = nullptr;
		Mode = InspectorMode::None;
	}

	void InspectorPanel::OnGui(float TimeStep)
	{
		Menu.Tick(TimeStep);

		NxEn::Object* Instance = nullptr;
		switch (Mode)
		{
		case NxEd::InspectorPanel::InspectorMode::None: Instance = nullptr; break;
		case NxEd::InspectorPanel::InspectorMode::Object: Instance = Target.Object; break;
		case NxEd::InspectorPanel::InspectorMode::Handle: Instance = Target.Handle.GetRedirectedPointer(); break;
		}

		if (!Instance)
		{
			return;
		}

		Instance->DrawGui(TimeStep);
	}
}
