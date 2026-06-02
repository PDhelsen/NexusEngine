#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Commands/Command.h"

namespace NxEn
{
	Command::Command(NxFr::StringId Id, NxFr::StringView Tooltip, const NxFr::Delegate<void(const NxFr::List<NxFr::StringView>&)>& Callback)
		: Id(Id), Tooltip(Tooltip), Callback(Callback)
	{

	}

	Command::~Command()
	{
	}

	void Command::Invoke(const NxFr::List<NxFr::StringView>& Arguments) const
	{
		Callback.Invoke(Arguments);
	}
}
