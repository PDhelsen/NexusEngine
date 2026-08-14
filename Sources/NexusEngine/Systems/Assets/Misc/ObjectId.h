#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Misc/Object/ObjectId.h"
#include "NexusEngine/Application/Application.h"
#include "NexusEngine/Systems/Assets/AssetsSystem.h"

namespace NxEn
{
	namespace ObjectId
	{
		template<typename T>
		struct ObjectIdResolver<T, typename NxFr::EnableIf<NxFr::InheritFrom<typename NxFr::DecayPointer<T>::Type, Asset>::Value>::Type>
		{
			static ObjectInstance<T> Resolve(NxFr::GUID Id)
			{
				if (Id == Object::NullId)
				{
					return nullptr;
				}

				return Application::GetSystem<AssetsSystem>()->Load(Id);
			}
		};
	}
}
