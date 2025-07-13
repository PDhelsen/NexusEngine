#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Commands/Command.h"

namespace NxEn
{
	const static Command TestFirst = Command("TestFirst"_Sid, []() { NEXUS_LOG(Info, Default, "First"); });
	const static Command TestSecond = Command("TestSecond"_Sid, []() { NEXUS_LOG(Info, Default, "Second"); });

	Command::Command(NxFr::StringId Id, NxFr::Delegate<void()> Callback)
		: Id(Id), Callback(Callback)
	{
		CommandsSystem::RegisterCommand(this);
	}

	Command::~Command()
	{
		CommandsSystem::UnregisterCommand(this);
	}

	void Command::Invoke()
	{
		Callback.Invoke();
	}
}
