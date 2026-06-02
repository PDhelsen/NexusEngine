#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/DebugSystem.h"

#include "NexusEngine/Systems/Settings/SettingTemplate.h"

#include "NexusFramework/Debug/Profiler/Instruments/ChromeTracing.h"

namespace NxFr
{
	namespace StatsHeader
	{
		const NxFr::StringId FpsId = "FPS"_Sid;
		const NxFr::StringId TimerMainId = "Timer - Main"_Sid;
	}
}

namespace NxEn
{
	static SettingSeq<bool>* SettingVerbosity = SettingSeq<bool>::Create("Settings", "LoggerVerbosity", { true, true, true, true });
	static SettingMap<bool>* SettingChannels = SettingMap<bool>::Create("Settings", "LoggerChannel", { {"Default", true}, { "Verbose", true }});
	static SettingVar<bool>* SettingAutoFlush = SettingVar<bool>::Create("Settings", "LoggerAutoFlush", false);

	static Command* CmdDebugLoggerChannel = Command::Create("Debug.Logger.Channel"_Sid, "Enable/Disable logger channel", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Channel, NxFr::StringView Enabled)
	{
		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
		Logger->SetChannel(NxFr::StringId(Channel), NxFr::StringUtility::FromString<bool>(Enabled));
	}));
	static Command* CmdDebugLoggerAllChannels = Command::Create("Debug.Logger.AllChannels"_Sid, "Enable/Disable all logger channel", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
		Logger->SetAllChannels(NxFr::StringUtility::FromString<bool>(Enabled));
	}));
	static Command* CmdDebugProfiler = Command::Create("Debug.Profiler"_Sid, "Enable/Disable profiler", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		NxFr::Instruments* Instrumentor = Application::GetSystem<DebugSystem>()->GetInstruments();
		if (NxFr::StringUtility::FromString<bool>(Enabled)) Instrumentor->StartRecording(); else Instrumentor->StopRecording();
	}));
	static Command* CmdDebugStats = Command::Create("Debug.Stats"_Sid, "Enable/Disable stats", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		if (NxFr::StringUtility::FromString<bool>(Enabled)) Stats->StartRecording(); else Stats->StopRecording();
	}));

	DebugSystem::DebugSystem()
		: Time(nullptr), Logs(NxFr::Globals::Debug::Logs), Statistiques(nullptr), Instrumentor(nullptr), Memory(nullptr)
	{
	}

	DebugSystem::~DebugSystem()
	{
	}

	void DebugSystem::OnInitialize()
	{
		System::OnInitialize();
		Time = &Application::GetInstance()->GetTime();
		Application::GetSystem<SettingsSystem>()->GetOnChange() += { this, &DebugSystem::ApplySettings };

		NxFr::String Folder = NxFr::Path::Combine(NxFr::Globals::Paths::Saved, NxFr::Globals::Args->Get("DebugFolder", "debug"));
		NxFr::Directory(Folder).Create();

		Logs->SetOutput(NxFr::LoggerOutput::All, NxFr::Path::Combine(Folder, "logs.txt"));
		Instrumentor = new NxFr::ChromeTracing(NxFr::Path::Combine(Folder, "instruments.json"));
		Statistiques = new NxFr::Stats(NxFr::Path::Combine(Folder, "stats.csv"));
		Memory = new NxFr::MemoryTracker();

		NxFr::Globals::Debug::Instrumentor = Instrumentor;
		NxFr::Globals::Debug::Statistiques = Statistiques;
		NxFr::Globals::Debug::Memory = Memory;

		NX_STAT_HEADER_INSTANCE(Statistiques, NxFr::StatsHeader::FpsId, Decimal, Set);
		NX_STAT_HEADER_INSTANCE(Statistiques, NxFr::StatsHeader::TimerMainId, Decimal, Set);
	}

	void DebugSystem::OnShutdown()
	{
		if (Instrumentor->IsRecording())
		{
			Instrumentor->StopRecording();
		}

		if (Statistiques->IsRecording())
		{
			Statistiques->StopRecording();
		}

		delete Instrumentor;
		delete Statistiques;
		delete Memory;

		NxFr::Globals::Debug::Instrumentor = nullptr;
		NxFr::Globals::Debug::Statistiques = nullptr;
		NxFr::Globals::Debug::Memory = nullptr;

		Application::GetSystem<SettingsSystem>()->GetOnChange() -= { this, & DebugSystem::ApplySettings };
		Time = nullptr;
		System::OnShutdown();
	}

	void DebugSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		Statistiques->Flush();
		Logs->Flush();
		Instrumentor->Flush();

		NX_STAT_DECIMAL(NxFr::StatsHeader::FpsId, 1.0f / Time->GetDeltaTime());
		NX_STAT_DECIMAL(NxFr::StatsHeader::TimerMainId, Time->GetDeltaTime() * NxFr::Time::SecondToMilli);
	}

	void DebugSystem::AutoStart()
	{
		Statistiques->Initialize();

		if (NxFr::Globals::Args->Has("Profile"))
		{
			NX_LOG(Info, System, "Debug Tools will start automatically");
			Instrumentor->StartRecording();
			Statistiques->StartRecording();
		}
	}

	void DebugSystem::ApplySettings()
	{
		auto& Verbosity = SettingVerbosity->GetValue();
		for (uint64 Index = 0; Index < Verbosity.GetCount(); ++Index)
		{
			Logs->SetVerbosity((NxFr::LoggerVerbosity)(1 << Index), Verbosity[Index]);
		}

		auto& Channels = SettingChannels->GetValue();
		for (auto It = Channels.Begin(); It != Channels.End(); ++It)
		{
			NxFr::StringId Id = NxFr::StringId(It.Get().Key);
			if (Logs->HasChannel(Id))
			{
				Logs->SetChannel(Id, It.Get().Value);
			}
			else
			{
				Logs->AddChannel(Id, It.Get().Value);
			}
		}

		Logs->SetAutoFlush(SettingAutoFlush->GetValue());
	}
}
