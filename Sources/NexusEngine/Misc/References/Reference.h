#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Misc/References/ReferenceGetter.h"
#include "NexusEngine/Misc/References/ReferenceResolver.h"
#include "NexusEngine/Misc/References/ReferenceDrawer.h"

namespace NxEn
{
	template<typename T>
	struct Reference
	{
	public:
		using P = typename ReferenceGetter<T>::P;

		Reference(NxFr::GUID Id) : Id(Id), Instance(nullptr) {}
		Reference(T Other) : Id(Other->GetId()), Instance(Other) {}

		inline explicit operator bool() const { return IsValid(); }
		inline explicit operator P () { return GetInstance(); }
		inline explicit operator const P() const { return GetInstance(); }
		inline P operator->() { return GetInstance(); }
		inline const P operator->() const { return GetInstance(); }

		inline bool operator==(const Reference<T>& Other) const { return GetId() == Other.GetId(); }
		inline bool operator!=(const Reference<T>& Other) const { return GetId() != Other.GetId(); }

		inline bool IsValid() const
		{
			return Id != 0;
		}

		inline bool IsResolved() const
		{
			return Instance != nullptr;
		}

		inline NxFr::GUID GetId() const
		{
			return Id;
		}

		inline NxFr::GUID GetInstanceId() const
		{
			return IsResolved() ? GetInstance()->GetId() : 0;
		}

		inline P GetInstance() const
		{
			return IsResolved() ? ReferenceGetter<T>::Get(Instance) : nullptr;
		}

		inline void Resolve(World* WorldInstance = nullptr)
		{
			Instance = ReferenceResolver<T>::Resolve(Id, WorldInstance);
		}

	private:
		NxFr::GUID Id;
		T Instance;
	};
}
