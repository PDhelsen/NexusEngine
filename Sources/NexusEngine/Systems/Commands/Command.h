#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class CommandsSystem;

	class NX_ENGINE_API Command
	{
	public:
		template<typename... Args>
		static Command* Create(NxFr::StringId Id, NxFr::StringView Tooltip, NxFr::Delegate<void(Args...)> Callback)
		{
			NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::Constant);

			return CommandsSystem::GetCommands().Register(Id, new Command(Id, Tooltip, [=](const NxFr::List<NxFr::StringView>& Arguments)
			{
				InvokeWithArguments(Callback, Arguments, NxFr::MakeIndexSequence<sizeof...(Args)>{});
			}));
		}

		Command(NxFr::StringId Id, NxFr::StringView Tooltip, const NxFr::Delegate<void(const NxFr::List<NxFr::StringView>&)>& Callback);
		~Command();

		void Invoke(const NxFr::List<NxFr::StringView>& Arguments) const;

		NxFr::StringId GetId() const { return Id; }
		NxFr::StringView GetTooltip() const { return Tooltip; }

	private:
		template<typename Func, uint64... Indices>
		static void InvokeWithArguments(Func&& Callback, const NxFr::List<NxFr::StringView>& Arguments, NxFr::IndexSequence<Indices...>)
		{
			Callback.Invoke(Arguments[Indices]...);
		}

		NxFr::StringId Id;
		NxFr::String Tooltip;
		NxFr::Delegate<void(const NxFr::List<NxFr::StringView>&)> Callback;
	};
}
