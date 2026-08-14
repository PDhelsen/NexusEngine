#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	template<typename T>
	struct ObjectInstance
	{
	public:
		enum class Mode
		{
			Pointer, Handle
		};

		ObjectInstance()
			: Pointer(nullptr), Instance(Mode::Pointer)
		{
		}

		ObjectInstance(std::nullptr_t)
			: Pointer(nullptr), Instance(Mode::Pointer)
		{
		}

		template<typename U>
		ObjectInstance(U* Pointer)
			: Pointer(Pointer), Instance(Mode::Pointer)
		{
		}

		template<typename U>
		ObjectInstance(NxFr::Handle<U> Handle)
			: Handle(Handle), Instance(Mode::Handle)
		{
		}

		~ObjectInstance()
		{
		}

		inline operator bool() const { return Lookup(); }
		template<typename U> inline bool operator==(U Other) const { return Lookup() == Other; }
		template<typename U> inline bool operator!=(U Other) const { return Lookup() != Other; }

		inline T* operator->() { return Lookup(); }
		inline const T* operator->() const { return Lookup(); }

		inline bool IsValid() const { return Lookup(); }
		inline T* Get() { return Lookup(); }
		inline const T* Get() const { return Lookup(); }

		inline T* AsPointer() { return IsPointer() ? Pointer : nullptr; }
		inline const T* AsPointer() const { return IsPointer() ? Pointer : nullptr; }
		inline NxFr::Handle<T> AsHandle() { return IsHandle() ? Handle : NxFr::Handle<T>(); }
		inline const NxFr::Handle<T> AsHandle() const { return IsHandle() ? Handle : NxFr::Handle<T>(); }

		inline Mode GetMode() const { return Instance; }
		inline bool IsPointer() const { return Instance == Mode::Pointer; }
		inline bool IsHandle() const { return Instance == Mode::Handle; }

	private:
		T* Lookup() const
		{
			switch (Instance)
			{
			case Mode::Pointer: return Pointer;
			case Mode::Handle: return const_cast<T*>(Handle.GetRedirectedPointer());
			}

			return nullptr;
		}

		union
		{
			T* Pointer;
			NxFr::Handle<T> Handle;
		};

		Mode Instance;
	};
}
