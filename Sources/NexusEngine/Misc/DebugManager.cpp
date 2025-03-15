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

		NxFr::Globals::Logs = Instance->Logger;
		NxFr::Globals::Statistiques = Instance->Stats;
		NxFr::Globals::Instrumentor = Instance->Instrumentor;
	}

	void DebugManager::Shutdown()
	{
		NxFr::Globals::Logs = nullptr;
		NxFr::Globals::Statistiques = nullptr;
		NxFr::Globals::Instrumentor = nullptr;

		Instance = nullptr;
	}

	DebugManager::DebugManager()
	{
		bool Profile = NxFr::Arguments::HasFlag("Profile", false);

		NxFr::Path DebugPath = NxFr::Paths::Saved + NxFr::Arguments::GetValue("DebugFolder", "debug");
		NxFr::Directory(DebugPath).Create();

		Logger = new NxFr::Logger(false, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::All, DebugPath + "logs.txt");
		Logger->AddChannel(NxFr::LoggerChannel::Default, true);
		Logger->AddChannel(NxFr::LoggerChannel::Verbose, false);

		Stats = new NxFr::Stats(DebugPath + "stats.csv");
		Stats->Initialize();
		Stats->Lock();
		if (Profile)
		{
			Stats->StartRecording();
		}

		Instrumentor = NxFr::Instruments::Create(DebugPath + "instruments.json", false);
		if (Profile)
		{
			Instrumentor->StartRecording();
		}
	}

	DebugManager::~DebugManager()
	{
		Logger->Flush();
		delete Logger;
		NxFr::Globals::Logs = nullptr;

		if (Stats->IsRecording())
		{
			Stats->StopRecording();
		}
		Stats->Unlock();
		Stats->Flush();
		delete Stats;
		NxFr::Globals::Statistiques = nullptr;

		if (Instrumentor->IsRecording())
		{
			Instrumentor->StopRecording();
		}
		NxFr::Instruments::Destroy(Instrumentor);
		NxFr::Globals::Instrumentor = nullptr;
	}

	void DebugManager::Flush()
	{
		Stats->Unlock();
		Stats->Flush();
		Stats->Lock();

		Logger->Flush();
	}
}
