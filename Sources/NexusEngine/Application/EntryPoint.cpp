#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/EntryPoint.h"

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

			NxFr::String Root = NxFr::Arguments::GetValue("Project", "") + "builds/artifacts/";
			NxFr::String Target = "NexusProject-" + NxFr::Arguments::GetValue("Target", "App") + ".dll";
			NxFr::String Dll = NxFr::Path::Normalize(Root + Target);

			auto CreateApplication = Platform->GetFunctionFromDll<Application*>(Dll, "CreateApplication");
			auto DestroyApplication = Platform->GetFunctionFromDll<void, Application*>(Dll, "DestroyApplication");

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
