#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	template<typename T>
	struct WorldObjectIterator
	{
	public:
		WorldObjectIterator(T* Pointer)
			: Instance(Pointer)
		{
		}

		WorldObjectIterator(T* Pointer, uint64 Index)
			: Instance(Pointer + Index)
		{
		}

		WorldObjectIterator<T>& operator++()
		{
			Iterate();
			return *this;
		}

		WorldObjectIterator<T> operator++(int32)
		{
			WorldObjectIterator<T> Temp = *this;
			++(*this);
			return Temp;
		}

		WorldObjectIterator<T>& operator--()
		{
			Reverse();
			return *this;
		}

		WorldObjectIterator<T> operator--(int32)
		{
			WorldObjectIterator<T> Temp = *this;
			--(*this);
			return Temp;
		}

		T* operator->()
		{
			return &Get();
		}

		const T* operator->() const
		{
			return &Get();
		}

		T& operator*()
		{
			return Get();
		}

		const T& operator*() const
		{
			return Get();
		}

		bool operator==(const WorldObjectIterator<T>& Other) const
		{
			return Equals(Other);
		}

		bool operator!=(const WorldObjectIterator<T>& Other) const
		{
			return !Equals(Other);
		}

		bool Equals(const WorldObjectIterator<T>& Other) const
		{
			return Instance == Other.Instance;
		}

		T& Get()
		{
			return *Instance;
		}

		const T& Get() const
		{
			return *Instance;
		}

		void Iterate()
		{
			++Instance;
		}

		void Reverse()
		{
			--Instance;
		}

		WorldObjectIterator<T>& Next()
		{
			Iterate();
			return *this;
		}

		WorldObjectIterator<T>& Previous()
		{
			Reverse();
			return *this;
		}

	private:
		T* Instance;
	};
}
