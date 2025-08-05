#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(ProjectPanel)

	ProjectPanel::ProjectPanel()
		: ProjectInfos(nullptr)
	{
	}

	ProjectPanel::~ProjectPanel()
	{
	}

	void ProjectPanel::SetProject(const Project* Infos)
	{
		ProjectInfos = Infos;
	}

	const Project* ProjectPanel::GetProject()
	{
		return ProjectInfos;
	}

	void ProjectPanel::OnInitialize()
	{
		Panel::OnInitialize();

		Title = "Project";
		GuiFlags = ImGuiWindowFlags_NoCollapse;
	}

	void ProjectPanel::OnGui(float TimeStep)
	{
		ImGui::Text("Mode: %s", Enum::ProjectModeToString(ProjectInfos->GetTarget()));
		ImGui::Separator();
		ImGui::Text("Name: %s", ProjectInfos->GetName().C());
		ImGui::Text("Root: %s", ProjectInfos->GetRootPath().C());
		ImGui::Text("Path: %s", ProjectInfos->GetPath().C());
		ImGui::Separator();
		ImGui::Text("Executable: %s", ProjectInfos->GetExecutablePath().C());
		ImGui::Text("Dll: %s", ProjectInfos->GetDllPath().C());
	}
}
