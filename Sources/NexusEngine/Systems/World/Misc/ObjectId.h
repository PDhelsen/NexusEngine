#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Misc/ObjectId/ObjectId.h"
#include "NexusEngine/Application/Application.h"
#include "NexusEngine/Systems/World/WorldSystem.h"

namespace NxEn
{
	namespace ObjectId
	{
		template<typename T>
		struct ObjectIdResolver<T, typename NxFr::EnableIf<
			NxFr::IsSameType<typename NxFr::DecayPointer<T>::Type, GameObject>::Value ||
			NxFr::InheritFrom<typename NxFr::DecayPointer<T>::Type, Behaviour>::Value ||
			NxFr::InheritFrom<typename NxFr::DecayPointer<T>::Type, Component>::Value
		>::Type>
		{
			static T Resolve(NxFr::GUID Id)
			{
				return Application::GetSystem<WorldSystem>()->GetObject(Id);
			}
		};
	}
}
