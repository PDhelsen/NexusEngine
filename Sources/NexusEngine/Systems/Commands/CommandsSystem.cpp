#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Commands/CommandsSystem.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(CommandsSystem)

	static NxFr::Dictionary<NxFr::StringId, Command*>& GetCommands()
	{
		static NxFr::Dictionary<NxFr::StringId, Command*> Commands;
		return Commands;
	}

	Command* CommandsSystem::GetCommand(NxFr::StringId Id)
	{
		return GetCommands()[Id];
	}

	void CommandsSystem::RegisterCommand(Command* Instance)
	{
		GetCommands().Append(Instance->GetId(), Instance);
	}

	void CommandsSystem::UnregisterCommand(Command* Instance)
	{
		GetCommands().Remove(Instance->GetId());
	}

	CommandsSystem::CommandsSystem()
		: Queue(), Current(nullptr)
	{
	}

	CommandsSystem::~CommandsSystem()
	{
	}

	void CommandsSystem::Run(NxFr::StringId Id, float Delay)
	{
		Queue.AppendConstruct(Id, Delay);
	}

	void CommandsSystem::Execute(NxFr::StringId Id)
	{
		Current = GetCommand(Id);
		Current->Invoke();
		Current = nullptr;
	}

	bool CommandsSystem::IsExecutingCommand() const
	{
		return Current != nullptr;
	}

	NxFr::StringId CommandsSystem::GetCurrentCommand() const
	{
		return IsExecutingCommand() ? Current->GetId() : NxFr::StringId(0);
	}

	void CommandsSystem::OnInitialize()
	{
		System::OnInitialize();
	}

	void CommandsSystem::OnShutdown()
	{
		System::OnShutdown();
	}

	void CommandsSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		FlushCommands(TimeStep);
	}

	void CommandsSystem::FlushCommands(float TimeStep)
	{
		while (Queue.GetCount() > 0)
		{
			CommandInfo& Info = Queue.Get();

			Info.Delay -= TimeStep;
			if (Info.Delay > 0.0f)
			{
				break;
			}

			Execute(Info.Id);
			Queue.Remove();
		}
	}
}
