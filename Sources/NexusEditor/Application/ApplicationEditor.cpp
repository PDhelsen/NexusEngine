#include "ApplicationEditor.h"

#include "NexusFramework/Core/NexusFramework.h"

namespace NxEd
{
	ApplicationEditor::ApplicationEditor()
	{
	}

	ApplicationEditor::~ApplicationEditor()
	{
	}

	void ApplicationEditor::Run()
	{
		NxFr::Platform::GetInstance()->GetFunctionFromDll<void>("NexusSandbox-Editor", "Run").Invoke();
		NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();
	}
}
