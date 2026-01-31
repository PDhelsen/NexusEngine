#include "NexusEditor/Systems/Object/Viewer/ViewerPanel.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerContext.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerFactory.h"

namespace NxEd
{
	static ViewerPanel* Panel = NxEn::GUI::Panel::Create<ViewerPanel>();

	const static NxEn::GUI::Menu::Item MenuItemViewer = NxEn::GUI::Menu::Item::Create("Object/Object/Viewer", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel ViewerPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(ViewerPanel)

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
		Panel::Show();

		Clear();

		if (!Instance)
		{
			return;
		}

		Context = ViewerFactory::Create(Instance->GetObjectType());
		if (!Context)
		{
			NEXUS_LOG(Warning, System, "No ViewerContext is associated to this object %d", Instance->GetId());
			return;
		}

		Context->SetupMenu(Menu);
		Context->SetupTarget(Instance);
	}

	void ViewerPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetTitle("Viewer");
		SetGuiFlag(ImGuiWindowFlags_MenuBar);
	}

	void ViewerPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void ViewerPanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.SetEnabled(true);
	}

	void ViewerPanel::OnDisable()
	{
		Clear();

		Menu.SetEnabled(false);
		Panel::OnDisable();
	}

	void ViewerPanel::OnGui(float TimeStep)
	{
		if (!Context)
		{
			return;
		}

		Menu.Tick(TimeStep);
		Context->OnGui(TimeStep);
	}
}
