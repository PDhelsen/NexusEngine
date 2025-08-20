#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxEn
{
	static ConsolePanel* Panel = GUI::Panel::Create<ConsolePanel>();

	NEXUS_OBJECT_IMPLEMENTATION(ConsolePanel)

	ConsolePanel::ConsolePanel()
		: Commands(nullptr), Logger(nullptr), Cmd(256)
	{
	}

	ConsolePanel::~ConsolePanel()
	{
	}

	void ConsolePanel::OnInitialize()
	{
		Panel::OnInitialize();

		Title = "Console";
	}

	void ConsolePanel::OnEnable()
	{
		Panel::OnEnable();

		Application* App = Application::GetInstance();
		Commands = App->GetSystem<CommandsSystem>();
		Logger = App->GetSystem<DebugSystem>()->GetLogger();
	}

	void ConsolePanel::OnGui(float TimeStep)
	{
		static float ButtonWidthExec = 150.0f;

		ImGui::Separator();

		ImGui::TextUnformatted("Command:");
		ImGui::SameLine();

		{
			GUI::Scope::HorizontalFill Fill(ButtonWidthExec);
			if (ImGui::InputText("##ConsoleCmd", Cmd.C_Buffer(), Cmd.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				ExecuteCommand();
			}
			ImGui::SameLine();
		}

		if (ImGui::Button("Execute##ConsoleCmdExecute", { ButtonWidthExec, ImGui::GetFrameHeight() }))
		{
			ExecuteCommand();
		}
	}

	void ConsolePanel::ExecuteCommand()
	{
		Cmd.Validate();
		Commands->Run(Cmd);

		Cmd.Clear();
	}
}
