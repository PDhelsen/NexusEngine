#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/EntryPoint.h"
#include "NexusEngine/Application/Project.h"

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
				NEXUS_LOG(Warning, Default, "Error Code was already set to %d", ErrorCode);
				return;
			}

			ErrorCode = Code;
		}

		int GetErrorCode()
		{
			return ErrorCode;
		}

		// -------------------------------------------------------------------------------------------------------------------------------
		// Main
		// -------------------------------------------------------------------------------------------------------------------------------

		int Main(int argc, char* argv[])
		{
			NxFr::AllocatorContext Context(nullptr);
			NxFr::Platform* Platform = NxFr::Platform::GetInstance();
			NxFr::Arguments::Parse(argc, argv);

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

			return ErrorCode;
		}
	}
}
