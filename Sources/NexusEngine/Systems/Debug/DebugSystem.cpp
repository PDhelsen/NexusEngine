#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/DebugSystem.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

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
#if NEXUS_DEBUG
	bool FlushOnLog = true;
#else
	bool FlushOnLog = false;
#endif

	static SettingSeq* SettingVerbosity = SettingSeq::Create("Settings", "LoggerVerbosity", Settings::Type::Bool);
	static SettingMap* SettingChannels = SettingMap::Create("Settings", "LoggerChannel", Settings::Type::Bool);

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

	NEXUS_OBJECT_IMPLEMENTATION(DebugSystem)

	DebugSystem::DebugSystem()
		: Logger(NxFr::Globals::Logs), Stats(nullptr), Instrumentor(nullptr), Time(0.0)
	{
	}

	DebugSystem::~DebugSystem()
	{
	}

	void DebugSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Application::GetSystem<SettingsSystem>()->GetOnChange() += { this, &DebugSystem::ApplySettings };

		NxFr::Path Folder = NxFr::Paths::Saved + NxFr::Arguments::Get("DebugFolder", "debug");
		NEXUS_ASSERT(!Folder.Data.IsEmpty(), System, "Folder can't be empty");
		NxFr::Directory(Folder).Create();

		Logger->SetFlushOnLog(FlushOnLog);
		Logger->SetOutput(NxFr::LoggerOutput::File, true, Folder + "logs.txt");
		Instrumentor = NxFr::Instruments::Create(Folder + "instruments.json", false);
		Stats = new NxFr::Stats(Folder + "stats.csv");

		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::FpsId, Decimal, Set);
		NEXUS_STAT_HEADER_INSTANCE(Stats, NxFr::StatsHeader::TimerMainId, Decimal, Set);

		NxFr::Globals::Statistiques = Stats;
		NxFr::Globals::Instrumentor = Instrumentor;
	}

	void DebugSystem::OnShutdown()
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		if (Instrumentor->IsRecording())
		{
			Instrumentor->StopRecording();
		}

		Stats->Unlock();
		if (Stats->IsRecording())
		{
			Stats->StopRecording();
		}

		Logger->SetFlushOnLog(false);

		NxFr::Globals::Statistiques = nullptr;
		NxFr::Globals::Instrumentor = nullptr;

		NxFr::Instruments::Destroy(Instrumentor);
		delete Stats;

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
		Stats->Lock();

		if (NxFr::Arguments::Has("Profile"))
		{
			NEXUS_LOG(Info, System, "Debug Tools will start automatically");
			Instrumentor->StartRecording();
			Stats->StartRecording();
		}
	}

	void DebugSystem::RecordStats()
	{
		double Now = NxFr::Platform::GetInstance()->GetProcessorTimer();
		float DeltaTime = (float)(Now - Time);

		NEXUS_STAT_DECIMAL(NxFr::StatsHeader::FpsId, 1.0f / DeltaTime);
		NEXUS_STAT_DECIMAL(NxFr::StatsHeader::TimerMainId, DeltaTime);

		Time = Now;
	}

	void DebugSystem::TickProfiler()
	{
		Stats->Unlock();
		Stats->Flush();
		Stats->Lock();

		Logger->Flush();
	}

	void DebugSystem::ApplySettings()
	{
		for (uint64 Index = 0; Index < SettingVerbosity->GetCount(); ++Index)
		{
			Logger->SetVerbosity((NxFr::LoggerVerbosity)(1 << Index), SettingVerbosity->As<bool>(Index));
		}

		auto ChannelsCollection = SettingChannels->GetCollection();
		for (auto& It = ChannelsCollection.Reset(); It != ChannelsCollection.End(); ++It)
		{
			auto& Kv = It.Get();
			Logger->SetChannel(NxFr::StringId(Kv.Key), Kv.Value.As<bool>());
		}
	}
}
