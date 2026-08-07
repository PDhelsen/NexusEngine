#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

namespace NxEn
{
	static const NxEn::GUI::Menu::Item* MenuItemProject = GUI::Menu::Create("File/Project", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Execute("GUI.Panel ProjectPanel");
	}));

	static ProjectPanel* PanelProject = GUI::Panel::Create<ProjectPanel>();

	ProjectPanel::ProjectPanel()
		: Infos(nullptr)
	{
	}

	ProjectPanel::~ProjectPanel()
	{
	}

	void ProjectPanel::OnInitialize()
	{
		Panel::OnInitialize();

		SetNameId("Project");
	}

	void ProjectPanel::OnEnable()
	{
		Panel::OnEnable();

		Infos = &Application::GetInstance()->GetProject();
	}

	void ProjectPanel::OnDraw()
	{
		GUI::Transform Visual;
		GUI::Drawer<NxFr::String>::Property(NxFr::StringUtility::ToString(Infos->GetMode()), "Mode", Visual);
		ImGui::Separator();
		GUI::Drawer<NxFr::String>::Property(Infos->GetName(), "Name", Visual);
		GUI::Drawer<NxFr::String>::Property(Infos->GetRootPath(), "Root", Visual);
		GUI::Drawer<NxFr::String>::Property(Infos->GetProjectPath(), "Path", Visual);
		ImGui::Separator();
		GUI::Drawer<NxFr::String>::Property(Infos->GetExecutablePath(), "Executable", Visual);
		GUI::Drawer<NxFr::String>::Property(Infos->GetDllPath(), "Dll", Visual);
	}
}
