#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class NX_ENGINE_API HandleManager
	{
		friend class MemorySystem;

	public:
		HandleManager(uint64 HandlesPerManager);
		HandleManager(const HandleManager& Other) = delete;
		HandleManager(HandleManager&& Other) noexcept = delete;
		~HandleManager();

		HandleManager& operator=(const HandleManager& Other) = delete;
		HandleManager& operator=(HandleManager&& Other) noexcept = delete;

		template<typename T>
		NxFr::Handle<T> AcquireHandle(T* Pointer)
		{
			return GetHandlesManager()->AcquireHandle(Pointer);
		}
		template<typename T>
		void UpdateHandle(NxFr::Handle<T> Handle, T* Pointer)
		{
			GetHandlesManager(Handle)->UpdateHandle(Handle, Pointer);
		}
		template<typename T>
		void* ReleaseHandle(NxFr::Handle<T> Handle)
		{
			return GetHandlesManager(Handle)->ReleaseHandle(Handle);
		}
		template<typename T>
		NxFr::Handle<T> FindHandle(T* Pointer)
		{
			return GetHandle(Pointer);
		}

	private:
		NxFr::Handle<void*> GetHandle(void* Pointer) const;
		NxFr::HandleManager* GetHandlesManager(NxFr::Handle<void*> Handle) const;
		NxFr::HandleManager* GetHandlesManager();
		NxFr::HandleManager* FindHandleManager();
		NxFr::HandleManager* CreateHandleManager();
		void ClearHandleManager(bool Force);

	private:
		NxFr::Set<NxFr::HandleManager*> Managers;
		uint64 HandlesPerManager;
	};
}

