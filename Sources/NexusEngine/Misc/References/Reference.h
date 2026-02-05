#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

#include "NexusEngine/Misc/References/ReferenceInstance.h"
#include "NexusEngine/Misc/References/ReferenceResolver.h"
#include "NexusEngine/Misc/References/ReferenceDrawer.h"

namespace NxEn
{
	template<typename T>
	union Reference
	{
	public:
		using P = typename NxFr::RemovePointer<T>::Type*;

		Reference() : Id(0) {}
		Reference(NxFr::GUID Id) : Id(Id) {}
		Reference(T Other) : Instance(Other) {}

		inline explicit operator bool() const { return IsValid(); }
		inline explicit operator P () { return GetInstance(); }
		inline explicit operator const P() const { return GetInstance(); }
		inline P operator->() { return GetInstance(); }
		inline const P operator->() const { return GetInstance(); }

		inline bool operator==(const Reference<T>& Other) const { return GetRawId() == Other.GetRawId(); }
		inline bool operator!=(const Reference<T>& Other) const { return GetRawId() != Other.GetRawId(); }

		inline bool IsValid() const
		{
			return Id != 0;
		}

		inline NxFr::GUID GetRawId() const
		{
			return Id;
		}

		inline NxFr::GUID GetInstanceId() const
		{
			return IsValid() ? GetInstance()->GetId() : 0;
		}

		inline P GetInstance() const
		{
			return ReferenceInstance<T>::Get(Instance);
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
