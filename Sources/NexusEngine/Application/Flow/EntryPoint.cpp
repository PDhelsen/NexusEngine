#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/EntryPoint.h"

#include "NexusEngine/Application/Project/Project.h"

namespace NxEn
{
	namespace EntryPoint
	{
		bool Restart = false;
		int ErrorCode = 0;

		void ScheduleRestart()
		{
			Restart = true;
		}

		void SetErrorCode(int32 Code)
		{
			if (ErrorCode != 0)
			{
				NX_LOG(Warning, Application, "Error Code was already set to %d", ErrorCode);
				return;
			}

			ErrorCode = Code;
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
				Instance->Run();
				DestroyApplication.Invoke(Instance);

			} while (Restart);

			// Globals - Shutdown
			NxFr::Globals::PlatformTarget->ClearDll();
			NxFr::Globals::Debug::Logs->Flush();
			delete NxFr::Globals::Debug::Logs;

			return ErrorCode;
		}
	}
}
