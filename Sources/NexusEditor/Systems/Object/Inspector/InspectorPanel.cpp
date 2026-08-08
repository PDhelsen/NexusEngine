#include "NexusEditor/Systems/Object/Inspector/InspectorPanel.h"

namespace NxEd
{
	InspectorPanel::InspectorPanel()
		: Target(nullptr), Mode(InspectorMode::None), Lock(false)
	{
	}

	InspectorPanel::~InspectorPanel()
	{
	}

	void InspectorPanel::Show(NxEn::Object* Instance, bool Force)
	{
		if (!Instance || (Lock && !Force))
		{
			return;
		}

		Panel::Show();

		Target.Object = Instance;
		Mode = InspectorMode::Object;
	}

	void InspectorPanel::Show(NxFr::Handle<NxEn::Object> Instance, bool Force)
	{
		if (!Instance || (Lock && !Force))
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

		SetNameId("Inspector");
		SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);

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
		Menu.Show();
	}

	void InspectorPanel::OnDisable()
	{
		Menu.Hide();
		Panel::OnDisable();

		Target.Object = nullptr;
		Mode = InspectorMode::None;
	}

	void InspectorPanel::OnDraw()
	{
		Menu.Draw();

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

		Instance->Draw();
	}
}
