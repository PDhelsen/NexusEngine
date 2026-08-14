#include "NexusEditor/Systems/Object/Inspector/InspectorPanel.h"

namespace NxEd
{
	InspectorPanel::InspectorPanel()
		: Instance(nullptr), Lock(false)
	{
	}

	InspectorPanel::~InspectorPanel()
	{
	}

	void InspectorPanel::Show(NxEn::ObjectInstance<NxEn::Object> Target, bool Force)
	{
		if (Lock && !Force)
		{
			return;
		}

		Panel::Show();
		Instance = Target;
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
		Panel::OnEnable();
		Menu.Show();
	}

	void InspectorPanel::OnDisable()
	{
		Instance = nullptr;

		Menu.Hide();
		Panel::OnDisable();
	}

	void InspectorPanel::OnDraw()
	{
		Menu.Draw();

		if (!Instance)
		{
			return;
		}

		Instance->Draw();
	}
}
