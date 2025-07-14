#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Commands/Command.h"

namespace NxEn
{
	const CommandInfo CommandInfo::Dummy = CommandInfo { .Id = 0, .Args = "", .Delay = 0 };

	Command::Command(NxFr::StringId Id, NxFr::StringView Tooltip, const NxFr::Delegate<void(NxFr::StringView)>& Callback, bool AutoRegister)
		: Id(Id), Tooltip(Tooltip.ToString()), Callback(Callback), Registered(false)
	{
		if (AutoRegister)
		{
			Register();
		}
	}

	Command::~Command()
	{
		Unregister();
	}

	void Command::Register()
	{
		if (Registered)
		{
			return;
		}

		CommandsSystem::RegisterCommand(this);
		Registered = true;
	}

	void Command::Unregister()
	{
		if (!Registered)
		{
			return;
		}

		CommandsSystem::UnregisterCommand(this);
		Registered = false;
	}

	void Command::Invoke(NxFr::StringView Args) const
	{
		Callback.Invoke(Args);
	}
}
