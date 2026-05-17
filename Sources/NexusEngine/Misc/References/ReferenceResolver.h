#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Application.h"
#include "NexusEngine/Systems/Assets/AssetsSystem.h"
#include "NexusEngine/Systems/World/WorldSystem.h"

namespace NxEn
{
	template<typename T, typename = void>
	struct ReferenceResolver
	{
		static T Resolve(NxFr::GUID Id, World* WorldInstance)
		{
			NEXUS_ASSERT(false, Default, "Unsupported resolver");
			return T();
		}
	};

	template<typename T>
	struct ReferenceResolver<T, NxFr::EnableIf<NxFr::InheritFrom<typename NxFr::DecayPointer<T>::Type, Asset>::Value>>
	{
		static T Resolve(NxFr::GUID Id, World* WorldInstance)
		{
			return static_cast<T>(Application::GetSystem<AssetsSystem>()->Load(Id));
		}
	};

	template<typename T>
	struct ReferenceResolver<T, NxFr::EnableIf<NxFr::IsSameType<typename NxFr::DecayPointer<T>::Type, GameObject>::Value>>
	{
		static T Resolve(NxFr::GUID Id, World* WorldInstance)
		{
			NEXUS_ASSERT(WorldInstance, Default, "World required to resolve reference");
			return static_cast<T>(WorldInstance->GetGameObject(WorldObjectReferences::Resolve(Id)));
		}
	};

	template<typename T>
	struct ReferenceResolver<T, NxFr::EnableIf<NxFr::InheritFrom<typename NxFr::DecayPointer<T>::Type, Behaviour>::Value>>
	{
		static T Resolve(NxFr::GUID Id, World* WorldInstance)
		{
			NEXUS_ASSERT(WorldInstance, Default, "World required to resolve reference");
			return static_cast<T>(WorldInstance->GetBehaviour(WorldObjectReferences::Resolve(Id)));
		}
	};

	template<typename T>
	struct ReferenceResolver<T, NxFr::EnableIf<NxFr::InheritFrom<typename NxFr::DecayPointer<T>::Type, Component>::Value>>
	{
		static T Resolve(NxFr::GUID Id, World* WorldInstance)
		{
			NEXUS_ASSERT(WorldInstance, Default, "World required to resolve reference");
			return static_cast<T>(WorldInstance->GetComponent(WorldObjectReferences::Resolve(Id)));
		}
	};
}
