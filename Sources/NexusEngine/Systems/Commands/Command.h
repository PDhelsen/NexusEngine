#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class CommandsSystem;

	struct NX_ENGINE_API CommandInfo
	{
		static const CommandInfo Dummy;

		NxFr::StringId Id;
		NxFr::String Args;
		float Delay;
	};

	class NX_ENGINE_API Command
	{
	public:
		template<typename... Args>
		static Command Create(NxFr::StringId Id, NxFr::StringView Tooltip, NxFr::Delegate<void(Args...)> Callback);

		Command(NxFr::StringId Id, NxFr::StringView Tooltip, const NxFr::Delegate<void(NxFr::StringView)>& Callback);
		~Command();

		void Invoke(NxFr::StringView Args) const;

		NxFr::StringId GetId() const { return Id; }
		NxFr::StringView GetTooltip() const { return Tooltip; }

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
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

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
