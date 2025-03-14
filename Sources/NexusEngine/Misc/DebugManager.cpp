#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/DebugManager.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	static DebugManager* Instance = nullptr;

	DebugManager* DebugManager::GetInstance()
	{
		return Instance;
	}

	void DebugManager::Initialize()
	{
		NEXUS_ASSERT(Instance == nullptr, Default, "DebugManager was already created");
		Instance = new DebugManager();

		bool Profile = NxFr::Arguments::HasFlag("Profile", false);

		NxFr::Path DebugPath = NxFr::Paths::Saved + NxFr::Arguments::GetValue("DebugFolder", "debug");
		NxFr::Directory(DebugPath).Create();

		Instance->Logger = NxFr::Globals::Logs = new NxFr::Logger(false, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::All, DebugPath + "logs.txt");
		Instance->Logger->AddChannel(NxFr::LoggerChannel::Default, true);
		Instance->Logger->AddChannel(NxFr::LoggerChannel::Verbose, false);

		Instance->Stats = NxFr::Globals::Statistiques = new NxFr::Stats(DebugPath + "stats.csv");
		Instance->Stats->Initialize();
		Instance->Stats->Lock();
		if (Profile)
		{
			Instance->Stats->StartRecording();
		}

		Instance->Instrumentor = NxFr::Globals::Instrumentor = NxFr::Instruments::Create(DebugPath + "instruments.json", false);
		if (Profile)
		{
			Instance->Instrumentor->StartRecording();
		}
	}

	void DebugManager::Shutdown()
	{
		Instance->Logger->Flush();
		delete Instance->Logger;
		NxFr::Globals::Logs = nullptr;

		if (Instance->Stats->IsRecording())
		{
			Instance->Stats->StopRecording();
		}
		Instance->Stats->Unlock();
		Instance->Stats->Flush();
		delete Instance->Stats;
		NxFr::Globals::Statistiques = nullptr;

		if (Instance->Instrumentor->IsRecording())
		{
			Instance->Instrumentor->StopRecording();
		}
		NxFr::Instruments::Destroy(Instance->Instrumentor);
		NxFr::Globals::Instrumentor = nullptr;

		Instance = nullptr;
	}

	void DebugManager::Flush()
	{
		Stats->Unlock();
		Stats->Flush();
		Stats->Lock();

		Logger->Flush();
	}
}
