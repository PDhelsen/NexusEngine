#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class CommandsSystem;

	struct CommandInfo
	{
		static const CommandInfo Dummy;

		NxFr::StringId Id;
		NxFr::String Args;
		float Delay;
	};

	class Command
	{
	public:
		template<typename... Args>
		static Command Create(NxFr::StringId Id, NxFr::StringView Tooltip, NxFr::Delegate<void(Args...)> Callback);

		NEXUS_ENGINE_API Command(NxFr::StringId Id, NxFr::StringView Tooltip, const NxFr::Delegate<void(NxFr::StringView)>& Callback);
		NEXUS_ENGINE_API ~Command();

		NEXUS_ENGINE_API void Invoke(NxFr::StringView Args) const;

		NEXUS_ENGINE_API NxFr::StringId GetId() const { return Id; }
		NEXUS_ENGINE_API NxFr::StringView GetTooltip() const { return Tooltip; }

	private:
		template<typename Func, uint64... Indices>
		static void InvokeWithArguments(Func&& Callback, const NxFr::List<NxFr::StringView>& Args, NxFr::IndexSequence<Indices...>);

	private:
		NxFr::StringId Id;
		NxFr::String Tooltip;
		NxFr::Delegate<void(NxFr::StringView)> Callback;
	};

	template<typename ...Args>
	inline Command Command::Create(NxFr::StringId Id, NxFr::StringView Tooltip, NxFr::Delegate<void(Args...)> Callback)
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		Command Instance = Command(Id, Tooltip, [=](NxFr::StringView ArgsLine)
		{
			NxFr::List<NxFr::StringView> Arguments = CommandsSystem::ParseArguments(ArgsLine);
			InvokeWithArguments(Callback, Arguments, NxFr::MakeIndexSequence<sizeof...(Args)>{});
		});
		CommandsSystem::RegisterCommand(&Instance);
		return Instance;
	}

	template<typename Func, uint64 ...Indices>
	inline void Command::InvokeWithArguments(Func&& Callback, const NxFr::List<NxFr::StringView>& Args, NxFr::IndexSequence<Indices...>)
	{
		Callback.Invoke(Args[Indices]...);
	}
}
