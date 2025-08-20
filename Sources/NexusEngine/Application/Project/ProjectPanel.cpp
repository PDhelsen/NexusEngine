#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

namespace NxEn
{
	static ProjectPanel* Panel = GUI::Panel::Create<ProjectPanel>();

	const static GUI::Menu::Item MenuItemProject = GUI::Menu::Item::Create("File/Project", NxFr::Delegate<void()>([]()
	{
		Application::GetInstance()->GetSystem<CommandsSystem>()->Execute("GUI.Panel,ProjectPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(ProjectPanel)

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

		Title = "Project";
	}

	void ProjectPanel::OnEnable()
	{
		Panel::OnEnable();

		Infos = &Application::GetInstance()->GetProject();
	}

	void ProjectPanel::OnGui(float TimeStep)
	{
		ImGui::Text("Mode: %s", Enum::ProjectModeToString(Infos->GetTarget()));
		ImGui::Separator();
		ImGui::Text("Name: %s", Infos->GetName().C());
		ImGui::Text("Root: %s", Infos->GetRootPath().C());
		ImGui::Text("Path: %s", Infos->GetPath().C());
		ImGui::Separator();
		ImGui::Text("Executable: %s", Infos->GetExecutablePath().C());
		ImGui::Text("Dll: %s", Infos->GetDllPath().C());
	}
}
