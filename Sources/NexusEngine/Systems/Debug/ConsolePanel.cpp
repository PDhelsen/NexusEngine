#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxEn
{
	static ConsolePanel* Panel = GUI::Panel::Create<ConsolePanel>();

	NEXUS_OBJECT_IMPLEMENTATION(ConsolePanel)

	ConsolePanel::ConsolePanel()
		: Menu(), Logs(), FlagsVerbosity(), FlagsChannels(), Command(256), Search(64)
	{
	}

	ConsolePanel::~ConsolePanel()
	{
	}

	void ConsolePanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		GuiFlags |= ImGuiWindowFlags_MenuBar;
		Title = "Console";
	}

	void ConsolePanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void ConsolePanel::OnEnable()
	{
		Panel::OnEnable();

		Application* App = Application::GetInstance();
		NxFr::Logger* Logger = App->GetSystem<DebugSystem>()->GetLogger();
		Logger->RegisterCallback({ this, &ConsolePanel::AddLogs });

		for (uint64 Index = 0; Index < NxFr::Enum::ToFlagIndex(NxFr::LoggerVerbosity::COUNT); ++Index)
		{
			uint64 Priority = FlagsVerbosity.GetCount();
			NxFr::LoggerVerbosity Verbosity = (NxFr::LoggerVerbosity)((uint64)1 << Index);
			FlagsVerbosity.Append(Verbosity, Logger->CheckVerbosity(Verbosity));

			NxFr::String Path = NxFr::StringView("Verbosity/") + NxFr::Enum::ToString((NxFr::LoggerVerbosity)Index);
			Menu.AddMenuToggle(Path, &FlagsVerbosity[Verbosity], nullptr, "", Priority);
		}

		NxFr::Array<NxFr::StringId> Channels = Logger->GetChannels();
		Channels.Sort([](const NxFr::StringId& A, const NxFr::StringId& B) { return A.C() <= B.C(); });
		for (auto& Id : Channels)
		{
			uint64 Priority = FlagsVerbosity.GetCount() + FlagsChannels.GetCount();
			FlagsChannels.Append(Id, Logger->CheckChannel(Id));

			NxFr::String Path = NxFr::StringView("Channels/") + Id.C();
			Menu.AddMenuToggle(Path, &FlagsChannels[Id], nullptr, "", Priority);
		}
	}

	void ConsolePanel::OnDisable()
	{
		FlagsVerbosity.Clear();
		FlagsChannels.Clear();
		Menu.Clear();

		Application* App = Application::GetInstance();
		NxFr::Logger* Logger = App->GetSystem<DebugSystem>()->GetLogger();
		Logger->UnregisterCallback({ this, &ConsolePanel::AddLogs });

		Panel::OnDisable();
	}

	void ConsolePanel::OnGui(float TimeStep)
	{
		static float ButtonWidthSearchLabel = ImGui::CalcTextSize("Search:").x;
		static float ButtonWidthSearchInput = 250.0f;
		static float ButtonWidthClear = 100.0f;
		static float ButtonWidthExec = 150.0f;
		static float LineHeight = ImGui::GetTextLineHeightWithSpacing();

		// Menu
		{
			Menu.Tick(TimeStep);

			if (ImGui::BeginMenuBar())
			{
				ImGui::SetCursorPosX(GUI::Utils::Fill(NxFr::Vector2f(ButtonWidthSearchLabel + ButtonWidthSearchInput + ButtonWidthClear), 3, false, true).x);

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Search:");
				{
					GUI::Scope::Width Width(ButtonWidthSearchInput);
					if (ImGui::InputText("##ConsoleCmd", Search.C_Buffer(), Search.GetCapacity()))
					{
						Search.Validate();
					}
				}

				if (ImGui::Button("Clear Logs", { ButtonWidthClear, ImGui::GetFrameHeight() }))
				{
					ClearLogs();
				}
			}
			ImGui::EndMenuBar();
		}

		// Logs
		{
			ImGui::BeginChild("##Logs", { 0, GUI::Utils::Fill(NxFr::Vector2f(LineHeight), 2).y }, 0, ImGuiWindowFlags_HorizontalScrollbar);

			for (auto& Log : Logs)
			{
				if (Log.Verbosity && Log.Channel && (Search.IsEmpty() || Log.Text.Contains(Search)))
				{
					ImGui::Text(Log.Text.C());
				}
			}

			ImGui::EndChild();
		}

		ImGui::Separator();

		// Command
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Command:");
			ImGui::SameLine();

			{
				GUI::Scope::Width Width(GUI::Utils::Fill(NxFr::Vector2f(ButtonWidthExec), 1, false).x);
				if (ImGui::InputText("##ConsoleCmd", Command.C_Buffer(), Command.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
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
	}

	void ConsolePanel::ExecuteCommand()
	{
		Command.Validate();

		Application::GetInstance()->GetSystem<CommandsSystem>()->Run(Command);

		Command.Clear();
	}

	void ConsolePanel::AddLogs(NxFr::LoggerVerbosity Verbosity, NxFr::StringId Channel, NxFr::StringView Message)
	{
		Logs.AppendConstruct(Move(Message.ToString()), FlagsVerbosity[Verbosity], FlagsChannels[Channel]);
	}

	void ConsolePanel::ClearLogs()
	{
		Logs.Clear();
	}
}
