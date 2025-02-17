#include "NexusEngine/Application/EntryPoint.h"

#if NEXUS_EDITOR
	#define NEXUS_DLL "NexusSandbox-Editor"
#else
	#define NEXUS_DLL "NexusSandbox-App"
#endif

int main(int argc, char* argv[])
{
	do
	{
		NxEn::EntryPoint::ResetRestart();

		auto Application = NxEn::EntryPoint::CreateApplication(NEXUS_DLL);
		Application->Run();
		NxEn::EntryPoint::DestroyApplication(NEXUS_DLL, Application);

	} while (NxEn::EntryPoint::ShouldRestart());

	return NxEn::EntryPoint::GetErrorCode();
}
