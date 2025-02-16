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

		NxEn::Application* Application = NEXUS_NAMESPACE::EntryPoint::CreateApplication();
		Application->Run();
		delete Application;

	} while (NxEn::EntryPoint::ShouldRestart());

	return NxEn::EntryPoint::GetErrorCode();
}
