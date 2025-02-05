#include "NexusStarter/Starter/Starter.h"

namespace NxSt
{
	void Start(NxFr::StringView Dll)
	{
		NxFr::Platform::GetInstance()->GetFunctionFromDll<void>(Dll, "Main").Invoke();
	}
}
