#include "NexusEditor/Systems/Object/Viewer/ViewerPanel.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerContext.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerFactory.h"

namespace NxEd
{
	void ViewerPanel::Clear()
	{
		if (!Context)
		{
			return;
		}

		delete Context;
		Context = nullptr;
	}

	void ViewerPanel::Show(NxEn::Object* Instance)
	{
		Clear();

		if (!Instance)
		{
			return;
		}

		Panel::Show();

		Context = ViewerFactory::Create(Instance->GetObjectType());
		if (!Context)
		{
			NX_LOG(Warning, System, "No ViewerContext is associated to this object %llu", Instance->GetId());
			return;
		}

		Context->SetupMenu(Menu);
		Context->SetupTarget(Instance);
	}

	void ViewerPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetNameId("Viewer");
		SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);
	}

	void ViewerPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void ViewerPanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.Show();
	}

	void ViewerPanel::OnDisable()
	{
		Clear();

		Menu.Hide();
		Panel::OnDisable();
	}

	void ViewerPanel::OnDraw()
	{
		if (!Context)
		{
			return;
		}

		Menu.Draw();
		Context->Draw();
	}
}
