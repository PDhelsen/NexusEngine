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
		template<uint8 C = 0, typename... Args>
		static Command Create(NxFr::StringId Id, NxFr::StringView Tooltip, NxFr::Delegate<void(Args...)> Callback, bool AutoRegister = true)
		{
			NEXUS_ASSERT_STATIC(C <= 5, "Command support up to 5 arguments");
			return Command(Id, Tooltip, [=](NxFr::StringView Args)
			{
				NxFr::List<NxFr::StringView> Arguments = CommandsSystem::ParseArguments(Args);
				if constexpr (C == 0)
					Callback.Invoke();
				else if constexpr (C == 1)
					Callback.Invoke(Arguments[0]);
				else if constexpr (C == 2)
					Callback.Invoke(Arguments[0], Arguments[1]);
				else if constexpr (C == 3)
					Callback.Invoke(Arguments[0], Arguments[1], Arguments[2]);
				else if constexpr (C == 4)
					Callback.Invoke(Arguments[0], Arguments[1], Arguments[2], Arguments[3]);
				else if constexpr (C == 5)
					Callback.Invoke(Arguments[0], Arguments[1], Arguments[2], Arguments[3], Arguments[4]);
				else
					NEXUS_LOG(Error, Default, "Commands arguments not supported");
			}, AutoRegister);
		}

		NEXUS_ENGINE_API Command(NxFr::StringId Id, NxFr::StringView Tooltip, const NxFr::Delegate<void(NxFr::StringView)>& Callback, bool AutoRegister = true);
		NEXUS_ENGINE_API ~Command();

		NEXUS_ENGINE_API void Register();
		NEXUS_ENGINE_API void Unregister();
		NEXUS_ENGINE_API void Invoke(NxFr::StringView Args) const;

		NEXUS_ENGINE_API bool IsRegistered() const { return Registered; }
		NEXUS_ENGINE_API NxFr::StringId GetId() const { return Id; }
		NEXUS_ENGINE_API NxFr::StringView GetTooltip() const { return Tooltip; }

	private:
		bool Registered;
		NxFr::StringId Id;
		NxFr::String Tooltip;
		NxFr::Delegate<void(NxFr::StringView)> Callback;
	};
}
