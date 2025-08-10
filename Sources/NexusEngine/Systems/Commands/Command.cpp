#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Commands/Command.h"

namespace NxEn
{
	const CommandInfo CommandInfo::Dummy = CommandInfo { .Id = 0, .Args = "", .Delay = 0 };

	Command::Command(NxFr::StringId Id, NxFr::StringView Tooltip, const NxFr::Delegate<void(NxFr::StringView)>& Callback)
		: Id(Id), Tooltip(Tooltip.ToString()), Callback(Callback)
	{

	}

	Command::~Command()
	{
	}

	void Command::Invoke(NxFr::StringView Args) const
	{
		Callback.Invoke(Args);
	}
}
