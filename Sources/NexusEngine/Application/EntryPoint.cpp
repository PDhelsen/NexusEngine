#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/EntryPoint.h"

namespace NxEn
{
	namespace EntryPoint
	{
		int8 ErrorCode = 0;
		bool Restart = false;

		Application* CreateApplication(NxFr::StringView DllName)
		{
			return NxFr::Platform::GetInstance()->GetFunctionFromDll<Application*>(DllName, "CreateApplication").Invoke();
		}

		void DestroyApplication(NxFr::StringView DllName, Application* Target)
		{
			NxFr::Platform::GetInstance()->GetFunctionFromDll<void, Application*>(DllName, "DestroyApplication").Invoke(Target);
		}

		int GetErrorCode()
		{
			return ErrorCode;
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

		bool ShouldRestart()
		{
			return Restart;
		}

		void ScheduleRestart()
		{
			Restart = true;
		}

		void ResetRestart()
		{
			Restart = false;
		}
	}
}
