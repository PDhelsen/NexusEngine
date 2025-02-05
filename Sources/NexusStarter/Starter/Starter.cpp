#include "NexusStarter/Starter/Starter.h"

namespace NxSt
{
	void Start(NxFr::StringView Dll)
	{
		NxFr::Platform::GetInstance()->ExecuteFromDll(Dll, "Main");
	}
}
