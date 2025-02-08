#if NEXUS_EDITOR
	#include "NexusEditor/Core/NexusEditorEntryPoint.h"
#else
	#include "NexusApp/Core/NexusAppEntryPoint.h"
#endif

int main(int argc, char* argv[])
{
#if NEXUS_EDITOR
	return NxEd::Main(argc, argv);
#else
	return NxAp::Main(argc, argv);
#endif
}
