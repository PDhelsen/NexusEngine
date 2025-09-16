#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/DebugSystem.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
#if NEXUS_DEBUG
	bool FlushOnLog = true;
#else
	bool FlushOnLog = false;
#endif

	static SettingSeq* SettingVerbosity = SettingSeq::Create(Settings::PageSettings, "LoggerVerbosity", Settings::Type::Bool);
	static SettingMap* SettingChannels = SettingMap::Create(Settings::PageSettings, "LoggerChannel", Settings::Type::Bool);

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
		: Logger(nullptr), Stats(nullptr), Instrumentor(nullptr)
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Logger = new NxFr::Logger(FlushOnLog, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::Console | NxFr::LoggerOutput::IDE | NxFr::LoggerOutput::Callback);
		Logger->AddChannel(NxFr::LoggerChannel::Default, true);
		Logger->AddChannel(NxFr::LoggerChannel::Verbose, true);

		NxFr::Globals::Logs = Logger;
	}

	DebugSystem::~DebugSystem()
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		NxFr::Globals::Logs = nullptr;

		Logger->Flush();
		delete Logger;
	}

	void DebugSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		NxFr::Path Folder = NxFr::Paths::Saved + NxFr::Arguments::GetValue("DebugFolder", "debug");
		NEXUS_ASSERT(!Folder.Data.IsEmpty(), System, "Folder can't be empty");
		NxFr::Directory(Folder).Create();

		Logger->SetOutput(NxFr::LoggerOutput::File, true, Folder + "logs.txt");
		Instrumentor = NxFr::Instruments::Create(Folder + "instruments.json", false);
		Stats = new NxFr::Stats(Folder + "stats.csv");

		ApplySettings();

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

		NxFr::Globals::Statistiques = nullptr;
		NxFr::Globals::Instrumentor = nullptr;

		NxFr::Instruments::Destroy(Instrumentor);
		delete Stats;

		System::OnShutdown();
	}

	void DebugSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		Stats->Unlock();
		Stats->Flush();
		Stats->Lock();

		Logger->Flush();
	}

	void DebugSystem::AutoStart()
	{
		Stats->Lock();

		if (NxFr::Arguments::HasFlag("Profile"))
		{
			NEXUS_LOG(Info, System, "Debug Tools will start automatically");
			Instrumentor->StartRecording();
			Stats->StartRecording();
		}
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
