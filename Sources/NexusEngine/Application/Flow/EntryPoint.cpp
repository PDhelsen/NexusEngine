#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/EntryPoint.h"

#include "NexusEngine/Application/Project/Project.h"

namespace NxEn
{
	namespace EntryPoint
	{
		static int ErrorCode = 0;
		static bool Restart = false;

		static Project CreateProject()
		{
			// Lookup Cmdline args (Named)
			NxFr::StringView PathArg = NxFr::Globals::Args->Get("Project");
			NxFr::String Path = NxFr::Path::Normalize(PathArg);

			// Lookup Cmdline args (Positional)
			if (Path.IsEmpty())
			{
				PathArg = NxFr::Globals::Args->Get(1);
				if (NxFr::Path::GetExtension(PathArg) == "nexus")
				{
					Path = NxFr::Path::Normalize(PathArg);
				}
			}

			// Lookup working directory
			if (Path.IsEmpty())
			{
				NxFr::String WorkingDir = NxFr::Globals::PlatformTarget->GetWorkingDirectory();
				NxFr::List<NxFr::String> Files = NxFr::Directory(WorkingDir).GetFiles();
				for (auto& File : Files)
				{
					if (NxFr::Path::GetExtension(File) == "nexus")
					{
						Path = Move(File);
						break;
					}
				}
			}

			return Project(Path);
		}

		int Main(int argc, char* argv[])
		{
			// Globals - Initialization
			NxFr::Allocator::Scope Memory(nullptr);
			NxFr::Globals::CreateArgs(argc, argv);
			NxFr::Globals::CreatePlatform();
			NxFr::Globals::Debug::Logs = new NxFr::Logger(NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::Console | NxFr::LoggerOutput::IDE, "", true);
			NxFr::Globals::Debug::Logs->AddChannel(NxFr::LoggerChannel::Default, true);

			// Project
			Project ProjectInfo = CreateProject();
			auto CreateApplication = NxFr::Globals::PlatformTarget->GetFunctionFromDll<Application*, const Project&>(ProjectInfo.GetDllPath(), "CreateApplication");
			auto DestroyApplication = NxFr::Globals::PlatformTarget->GetFunctionFromDll<void, Application*>(ProjectInfo.GetDllPath(), "DestroyApplication");

			// Application
			do
			{
				Restart = false;

				Application* Instance = CreateApplication.Invoke(ProjectInfo);
				Instance->Initialize();
				Instance->Run();
				Instance->Shutdown();
				DestroyApplication.Invoke(Instance);

			} while (Restart);

			// Globals - Shutdown
			NxFr::Globals::PlatformTarget->ClearDll();
			NxFr::Globals::Debug::Logs->Flush();
			delete NxFr::Globals::Debug::Logs;

			return ErrorCode;
		}

		void SetErrorCode(int32 Code)
		{
			ErrorCode = Code;
		}

		void ScheduleRestart()
		{
			Restart = true;
		}
	}
}
