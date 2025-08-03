#include "NexusEditor/Systems/Editor/EditorWindow.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(EditorWindow)

	EditorWindow::EditorWindow()
		: Element(false), GuiFlags(0), Menu(true), Style()
	{
	}

	EditorWindow::~EditorWindow()
	{
	}

	void EditorWindow::OnInitialize()
	{
		Element::OnInitialize();

		GuiFlags =
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings;

		Menu.Initialize();

		Style.AppendVar(ImGuiStyleVar_WindowRounding, 0.0f);
		Style.AppendVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		Style.AppendVarXY(ImGuiStyleVar_WindowPadding, NxFr::Vector2f(0.0f));
	}

	void EditorWindow::OnShutdown()
	{
		Menu.Shutdown();

		Element::OnShutdown();
	}

	void EditorWindow::OnTick(float TimeStep)
	{
		ImGuiViewport* Viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(Viewport->Pos);
		ImGui::SetNextWindowSize(Viewport->Size);

		Style.Push();
		if (ImGui::Begin("Editor", nullptr, GuiFlags))
		{
			Style.Pop();

			OnGui(TimeStep);
			ImGui::End();
		}
	}

	void EditorWindow::OnGui(float TimeStep)
	{
		ImGui::DockSpace(ImGui::GetID("Editor"));
		Menu.Tick(TimeStep);
	}

}
