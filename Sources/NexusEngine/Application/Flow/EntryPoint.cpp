#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Flow/EntryPoint.h"

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
			NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			NxFr::Globals::Debug::Logs = new NxFr::Logger(NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::Console | NxFr::LoggerOutput::IDE | NxFr::LoggerOutput::Callback, "", true);
			NxFr::Globals::Debug::Logs->AddChannel(NxFr::LoggerChannel::Default, true);
			NxFr::Globals::Debug::Logs->AddChannel(NxFr::LoggerChannel::Verbose, true);
		}

		static void DestroyLogger()
		{
			NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			NxFr::Globals::Debug::Logs->Flush();
			delete NxFr::Globals::Debug::Logs;
		}

		// -------------------------------------------------------------------------------------------------------------------------------
		// Main
		// -------------------------------------------------------------------------------------------------------------------------------

		int Main(int argc, char* argv[])
		{
			NxFr::Allocator::Scope Context(nullptr);
			NxFr::Globals::CreateArgs(argc, argv);
			NxFr::Globals::CreatePlatform();
			CreateLogger();

			Project ProjectInfo = CreateProject();
			auto CreateApplication = NxFr::Globals::PlatformTarget->GetFunctionFromDll<Application*, const Project&>(ProjectInfo.GetDllPath(), "CreateApplication");
			auto DestroyApplication = NxFr::Globals::PlatformTarget->GetFunctionFromDll<void, Application*>(ProjectInfo.GetDllPath(), "DestroyApplication");

			do
			{
				Restart = false;

				Application* Instance = CreateApplication.Invoke(ProjectInfo);
				Instance->Run();
				DestroyApplication.Invoke(Instance);

			} while (Restart);

			NxFr::Globals::PlatformTarget->ClearDll();
			DestroyLogger();

			return ErrorCode;
		}
	}
}
