#pragma once

#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	namespace Iterator
	{
		template<typename T>
		struct IteratorWorld : public NxFr::Iterator::Iterator<T, IteratorWorld<T>>
		{
		public:
			IteratorWorld(T* Pointer, uint64 Index = 0)
				: Instance(Pointer + Index)
			{
			}

			bool Equals(const IteratorWorld<T>& Other) const
			{
				return Instance == Other.Instance;
			}

			void Increment()
			{
				++Instance;
			}

			void Decrement()
			{
				--Instance;
			}

			T& Get()
			{
				return *Instance;
			}

			const T& Get() const
			{
				return *Instance;
			}

			uint64 Id() const
			{
				NX_ASSERT(false, Default, "IteratorWorld doesn't support query the id");
				return -1;
			}

		private:
			T* Instance;
		};
	}
}
