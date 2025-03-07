#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/EntryPoint.h"

#if NEXUS_EDITOR
	#define NEXUS_PROJECT_DLL "NexusProject-Editor"
#else
	#define NEXUS_PROJECT_DLL "NexusProject-App"
#endif

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

			auto CreateApplication = Platform->GetFunctionFromDll<Application*>(NEXUS_PROJECT_DLL, "CreateApplication");
			auto DestroyApplication = Platform->GetFunctionFromDll<void, Application*>(NEXUS_PROJECT_DLL, "DestroyApplication");

			do
			{
				Restart = false;

				Application* Instance = CreateApplication.Invoke();
				Instance->Run();
				DestroyApplication.Invoke(Instance);

			} while (Restart);

			Platform->ClearDll();

			return ErrorCode;
		}
	}
}
