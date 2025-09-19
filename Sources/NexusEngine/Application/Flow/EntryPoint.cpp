#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/EntryPoint.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"
#include "NexusEngine/Application/Project/Project.h"

namespace NxEn
{
	namespace EntryPoint
	{
		// -------------------------------------------------------------------------------------------------------------------------------
		// Variables
		// -------------------------------------------------------------------------------------------------------------------------------

		bool Restart = false;
		int8 ErrorCode = 0;

		void ScheduleRestart()
		{
			Restart = true;
		}

		void SetErrorCode(int8 Code)
		{
			if (ErrorCode != 0)
			{
				NEXUS_LOG(Warning, Application, "Error Code was already set to %d", ErrorCode);
				return;
			}

			ErrorCode = Code;
		}

		int GetErrorCode()
		{
			return ErrorCode;
		}

		// -------------------------------------------------------------------------------------------------------------------------------
		// Steps
		// -------------------------------------------------------------------------------------------------------------------------------

		static void CreateLogger()
		{
			NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			NxFr::Globals::Logs = new NxFr::Logger(true, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::Console | NxFr::LoggerOutput::IDE | NxFr::LoggerOutput::Callback);
			NxFr::Globals::Logs->AddChannel(NxFr::LoggerChannel::Default, true);
			NxFr::Globals::Logs->AddChannel(NxFr::LoggerChannel::Verbose, true);
		}

		static void DestroyLogger()
		{
			NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			NxFr::Globals::Logs->Flush();
			delete NxFr::Globals::Logs;
		}

		// -------------------------------------------------------------------------------------------------------------------------------
		// Main
		// -------------------------------------------------------------------------------------------------------------------------------

		int Main(int argc, char* argv[])
		{
			NxFr::AllocatorContext Context(nullptr);
			NxFr::Platform* Platform = NxFr::Platform::GetInstance();
			NxFr::Arguments::Parse(argc, argv);
			CreateLogger();

			Project ProjectInfo = CreateProject();
			auto CreateApplication = Platform->GetFunctionFromDll<Application*, const Project&>(ProjectInfo.GetDllPath(), "CreateApplication");
			auto DestroyApplication = Platform->GetFunctionFromDll<void, Application*>(ProjectInfo.GetDllPath(), "DestroyApplication");

			do
			{
				Restart = false;

				Application* Instance = CreateApplication.Invoke(ProjectInfo);
				Instance->Run();
				DestroyApplication.Invoke(Instance);

			} while (Restart);

			Platform->ClearDll();
			DestroyLogger();

			return ErrorCode;
		}
	}
}
