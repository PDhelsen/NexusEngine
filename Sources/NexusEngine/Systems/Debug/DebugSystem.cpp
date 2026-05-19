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
#if NX_DEBUG
	bool AutoFlush = true;
#else
	bool AutoFlush = false;
#endif

	static SettingSeq<bool>* SettingVerbosity = SettingSeq<bool>::Create("Settings", "LoggerVerbosity", { true, true, true, true });
	static SettingMap<bool>* SettingChannels = SettingMap<bool>::Create("Settings", "LoggerChannel", { {"Default", true}, { "Verbose", true }});

	const static Command CmdDebugLoggerChannel = Command::Create("Debug.Logger.Channel"_Sid, "Enable/Disable logger channel", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Channel, NxFr::StringView Enabled)
	{
		Application::GetSystem<DebugSystem>()->GetLogger()->SetChannel(NxFr::StringId(Channel), Enabled == "true");
	}));
	const static Command CmdDebugLoggerAllChannels = Command::Create("Debug.Logger.AllChannels"_Sid, "Enable/Disable all logger channel", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		Application::GetSystem<DebugSystem>()->GetLogger()->SetAllChannels(Enabled == "true");
	}));
	const static Command CmdDebugProfiler = Command::Create("Debug.Profiler"_Sid, "Enable/Disable profiler", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		NxFr::Instruments* Instrumentor = Application::GetSystem<DebugSystem>()->GetInstrumentor();
		if (Enabled == "true") Instrumentor->StartRecording(); else Instrumentor->StopRecording();
	}));
	const static Command CmdDebugStats = Command::Create("Debug.Stats"_Sid, "Enable/Disable stats", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		if (Enabled == "true") Stats->StartRecording(); else Stats->StopRecording();
	}));

	NX_OBJECT_IMPLEMENTATION(DebugSystem)

	DebugSystem::DebugSystem()
		: Logger(NxFr::Globals::Debug::Logs), Stats(nullptr), Instrumentor(nullptr), Memory(nullptr), Time(0.0)
	{
	}

	DebugSystem::~DebugSystem()
	{
	}

	void DebugSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Application::GetSystem<SettingsSystem>()->GetOnChange() += { this, &DebugSystem::ApplySettings };

		NxFr::String Folder = NxFr::Path::Combine(NxFr::Globals::Paths::Saved, NxFr::Globals::Args->Get("DebugFolder", "debug"));
		NX_ASSERT(!Folder.IsEmpty(), System, "Folder can't be empty");
		NxFr::Directory(Folder).Create();

		Logger->SetAutoFlush(AutoFlush);
		Logger->SetOutput(NxFr::LoggerOutput::All, NxFr::Path::Combine(Folder, "logs.txt"));
		Instrumentor = new NxFr::ChromeTracing(NxFr::Path::Combine(Folder, "instruments.json"), false);
		Stats = new NxFr::Stats(NxFr::Path::Combine(Folder, "stats.csv"));
		Memory = new NxFr::MemoryTracker();

		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::FpsId, Decimal, Set);
		NX_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::TimerMainId, Decimal, Set);

		NxFr::Globals::Debug::Statistiques = Stats;
		NxFr::Globals::Debug::Instrumentor = Instrumentor;
		NxFr::Globals::Debug::Memory = Memory;
	}

	void DebugSystem::OnShutdown()
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		if (Instrumentor->IsRecording())
		{
			Instrumentor->StopRecording();
		}

		if (Stats->IsRecording())
		{
			Stats->StopRecording();
		}

		Logger->SetAutoFlush(false);

		NxFr::Globals::Debug::Statistiques = nullptr;
		NxFr::Globals::Debug::Instrumentor = nullptr;
		NxFr::Globals::Debug::Memory = nullptr;

		delete Instrumentor;
		delete Stats;
		delete Memory;

		System::OnShutdown();
	}

	void DebugSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		RecordStats();
		TickProfiler();
	}

	void DebugSystem::AutoStart()
	{
		Stats->Initialize();

		if (NxFr::Globals::Args->Has("Profile"))
		{
			NX_LOG(Info, System, "Debug Tools will start automatically");
			Instrumentor->StartRecording();
			Stats->StartRecording();
		}
	}

	void DebugSystem::RecordStats()
	{
		double Now = NxFr::Globals::PlatformTarget->GetProcessorTimer();
		float DeltaTime = (float)(Now - Time);

		NX_STAT_DECIMAL(NxFr::StatsHeader::FpsId, 1.0f / DeltaTime);
		NX_STAT_DECIMAL(NxFr::StatsHeader::TimerMainId, DeltaTime);

		Time = Now;
	}

	void DebugSystem::TickProfiler()
	{
		Stats->Flush();
		Logger->Flush();
		Instrumentor->Flush();
	}

	void DebugSystem::ApplySettings()
	{
		auto& Verbosity = SettingVerbosity->GetValue();
		for (uint64 Index = 0; Index < Verbosity.GetCount(); ++Index)
		{
			Logger->SetVerbosity((NxFr::LoggerVerbosity)(1 << Index), Verbosity[Index]);
		}

		auto& Channels = SettingChannels->GetValue();
		for (auto It = Channels.Begin(); It != Channels.End(); ++It)
		{
			NxFr::StringId Id = NxFr::StringId(It.Get().Key);
			if (Logger->HasChannel(Id))
			{
				Logger->SetChannel(Id, It.Get().Value);
			}
			else
			{
				Logger->AddChannel(Id, It.Get().Value);
			}
		}
	}
}
