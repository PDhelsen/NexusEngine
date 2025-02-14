#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineEntryPoint.h"

#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

namespace NxEn
{
	namespace EntryPoint
	{
		int8 ErrorCode = 0;
		bool Restart = false;

		void NxEn::EntryPoint::Initialize()
		{
			NxFr::Initialize();
		}

		void NxEn::EntryPoint::Shutdown()
		{
			NxFr::Shutdown();
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
