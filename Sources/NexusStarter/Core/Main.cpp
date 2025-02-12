#include "NexusEngine/Core/NexusEngineEntryPoint.h"
#if NEXUS_EDITOR
	#include "NexusEditor/Core/NexusEditorEntryPoint.h"
#else
	#include "NexusApp/Core/NexusAppEntryPoint.h"
#endif

int main(int argc, char* argv[])
{
	NxEn::Initialize(argc, argv);

#if NEXUS_EDITOR
	NxEd::Main();
#else
	NxAp::Main();
#endif

	return NxEn::Shutdown();
}
