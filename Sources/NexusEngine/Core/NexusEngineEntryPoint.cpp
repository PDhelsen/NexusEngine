#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineEntryPoint.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

#include "NexusEngine/Core/NexusEngineGlobals.h"

namespace NxEn
{
	// ---------------------------------------------------------------------------
	// Paths
	// ---------------------------------------------------------------------------

	static void CreateFolders()
	{
		NxFr::Directory(NxFr::Paths::Cooked).Create();
		NxFr::Directory(NxFr::Paths::Saved).Create();
	}

	static NxFr::Path GetDebugPath()
	{
		NxFr::Path DebugPath = NxFr::Paths::Saved + "debug";
		NxFr::Directory(DebugPath).Create();
		return DebugPath;
	}

	// ---------------------------------------------------------------------------
	// Framework
	// ---------------------------------------------------------------------------

	static void CreateLogger(const NxFr::Path& Folder)
	{
		NxFr::Logger* Logs = new NxFr::Logger(true, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::All, Folder + "logs.txt");
		Logs->AddChannel(NxFr::LoggerChannel::Default, true);
		Logs->AddChannel(NxFr::LoggerChannel::Verbose, false);
		NxFr::Globals::Logs = Logs;
	}

	static void CreateStats(const NxFr::Path& Folder)
	{
		NxFr::Stats* Statistiques = new NxFr::Stats(Folder + "stats.csv");
		Statistiques->Initialize();
		Statistiques->StartRecording();
		NxFr::Globals::Statistiques = Statistiques;
	}

	static void CreateIntruments(const NxFr::Path& Folder)
	{
		NxFr::Instruments* Instrumentor = NxFr::Instruments::Create(Folder + "instruments.json", false);
		Instrumentor->StartRecording();
		NxFr::Globals::Instrumentor = Instrumentor;
	}

	static void DestroyLogger()
	{
		NxFr::Logger* Logs = NxFr::Globals::Logs;
		NxFr::Globals::Logs = nullptr;

		delete Logs;
	}

	static void DestroyStats()
	{
		NxFr::Stats* Statistiques = NxFr::Globals::Statistiques;
		NxFr::Globals::Statistiques = nullptr;

		Statistiques->StopRecording();
		delete Statistiques;
	}

	static void DestroyInstruments()
	{
		NxFr::Instruments* Instrumentor = NxFr::Globals::Instrumentor;
		NxFr::Globals::Instrumentor = nullptr;

		Instrumentor->StopRecording();
		NxFr::Instruments::Destroy(Instrumentor);
	}

	// ---------------------------------------------------------------------------
	// Boot
	// ---------------------------------------------------------------------------

	void Initialize(int argc, char* argv[])
	{
		NxFr::AllocatorContext Context(nullptr);

		NxFr::Platform::GetInstance();

		CreateFolders();
		NxFr::Path DebugPath = GetDebugPath();

		CreateLogger(DebugPath);
		CreateStats(DebugPath);
		CreateIntruments(DebugPath);
	}

	int Shutdown()
	{
		NxFr::AllocatorContext Context(nullptr);

		DestroyInstruments();
		DestroyStats();
		DestroyLogger();

		NxFr::Platform::GetInstance()->ClearDll();

		return Globals::GetErrorCode();
	}
}
