#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineGlobals.h"

namespace NxEn
{
	namespace Globals
	{
		static uint8 ErrorCode = 0;

		void SetErrorCode(uint8 Code)
		{
			if (Code != 0)
			{
				NEXUS_LOG(Warning, Default, "Error code was already set to %d", Code);
				return;
			}

			ErrorCode = Code;
		}

		uint8 GetErrorCode()
		{
			return ErrorCode;
		}
	}
}
