#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxEn
{
	static NxFr::StringId TextInfo = "Info"_Sid;
	static NxFr::StringId TextWarning = "Warning"_Sid;
	static NxFr::StringId TextError = "Error"_Sid;
	static NxFr::StringId TextFatal = "Fatal"_Sid;

	static ConsolePanel* Panel = GUI::Panel::Create<ConsolePanel>();

	NEXUS_OBJECT_IMPLEMENTATION(ConsolePanel)

	const GUI::Style& ConsolePanel::GetStyle(NxFr::LoggerVerbosity Verbosity)
	{
		NxFr::StringId Id = 0;

		switch (Verbosity)
		{
		case NxFr::LoggerVerbosity::Info: Id = TextInfo; break;
		case NxFr::LoggerVerbosity::Warning: Id = TextWarning; break;
		case NxFr::LoggerVerbosity::Error: Id = TextError; break;
		case NxFr::LoggerVerbosity::Fatal: Id = TextFatal; break;

		case NxFr::LoggerVerbosity::None:
		case NxFr::LoggerVerbosity::All:
		case NxFr::LoggerVerbosity::COUNT:
		default: Id = 0; break;
		}

		return Application::GetSystem<GUISystem>()->GetStyle(Id);
	}

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

		SetGuiFlag(ImGuiWindowFlags_MenuBar);
		SetTitle("Console");
	}

	void ConsolePanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void ConsolePanel::OnEnable()
	{
		Panel::OnEnable();

		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
		Logger->RegisterCallback({ this, &ConsolePanel::AddLogs });

		uint8 VerbosityCount = NxFr::Enum::ToFlagIndex(NxFr::LoggerVerbosity::COUNT);
		FlagsVerbosity.Grow(VerbosityCount);
		for (uint64 Index = 0; Index < VerbosityCount; ++Index)
		{
			uint64 Priority = FlagsVerbosity.GetCount();
			NxFr::LoggerVerbosity Verbosity = (NxFr::LoggerVerbosity)((uint64)1 << Index);
			FlagsVerbosity.Append(Verbosity, Logger->CheckVerbosity(Verbosity));

			NxFr::String Path = NxFr::StringView("Verbosity/") + NxFr::Enum::ToString((NxFr::LoggerVerbosity)Index);
			Menu.AddMenuToggle(Path, &FlagsVerbosity[Verbosity], nullptr, "", Priority);
		}

		NxFr::Array<NxFr::StringId> Channels = Logger->GetChannels();
		Channels.Sort([](const NxFr::StringId& A, const NxFr::StringId& B) { return A.C() <= B.C(); });
		FlagsChannels.Grow(Channels.GetCount());
		for (uint64 Index = 0; Index < Channels.GetCount(); ++Index)
		{
			uint64 Priority = FlagsVerbosity.GetCount() + FlagsChannels.GetCount();
			NxFr::StringId Id = Channels[Index];
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

		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
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
					if (ImGui::InputText("##Search", Search.C_Buffer(), Search.GetCapacity()))
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
			ImGui::BeginChild("Logs", { 0, GUI::Utils::Fill(NxFr::Vector2f(LineHeight), 2).y }, 0, ImGuiWindowFlags_HorizontalScrollbar);

			for (uint64 Index = 0; Index < Logs.GetCount(); ++Index)
			{
				Log& Log = Logs[Index];
				if (Log.Verbosity && Log.Channel && (Search.IsEmpty() || Log.Text.Contains(Search)))
				{
					GUI::Scope::Style Style(Log.Style);
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
				if (ImGui::InputText("##Command", Command.C_Buffer(), Command.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					ExecuteCommand();
				}
				ImGui::SameLine();
			}

			if (ImGui::Button("Execute", { ButtonWidthExec, ImGui::GetFrameHeight() }))
			{
				ExecuteCommand();
			}
		}
	}

	void ConsolePanel::ExecuteCommand()
	{
		Command.Validate();

		Application::GetSystem<CommandsSystem>()->Run(Command);

		Command.Clear();
	}

	void ConsolePanel::AddLogs(NxFr::LoggerVerbosity Verbosity, NxFr::StringId Channel, NxFr::StringView Message)
	{
		Logs.AppendConstruct(Move(Message.ToString()), FlagsVerbosity[Verbosity], FlagsChannels[Channel], GetStyle(Verbosity));
	}

	void ConsolePanel::ClearLogs()
	{
		Logs.Clear();
	}
}
