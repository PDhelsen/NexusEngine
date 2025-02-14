#include "NexusEngine/Core/NexusEngineEntryPoint.h"
#if NEXUS_EDITOR
	#include "NexusEditor/Core/NexusEditorEntryPoint.h"
#else
	#include "NexusApp/Core/NexusAppEntryPoint.h"
#endif

#if NEXUS_EDITOR
	#define NEXUS_NAMESPACE NxEd
#else
	#define NEXUS_NAMESPACE NxAp
#endif

int main(int argc, char* argv[])
{
	do
	{
		NxEn::EntryPoint::ResetRestart();

		NxEn::EntryPoint::Initialize();
		NEXUS_NAMESPACE::EntryPoint::Run();
		NxEn::EntryPoint::Shutdown();

	} while (NxEn::EntryPoint::ShouldRestart());

	return NxEn::EntryPoint::GetErrorCode();
}
