#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Memory/HandleManager.h"

namespace NxEn
{
	HandleManager::HandleManager(uint64 HandlesPerManager)
		: Managers(4, nullptr), HandlesPerManager(HandlesPerManager)
	{
	}

	HandleManager::~HandleManager()
	{
	}

	NxFr::Handle<void*> HandleManager::GetHandle(void* Pointer) const
	{
		for (NxFr::HandleManager* Manager : Managers)
		{
			NxFr::Handle<void*> Handle = Manager->FindHandle(Pointer);
			if (Handle.IsValid())
			{
				return Handle;
			}
		}

		return NxFr::Handle<void*>();
	}

	NxFr::HandleManager* HandleManager::GetHandlesManager(NxFr::Handle<void*> Handle) const
	{
		for (NxFr::HandleManager* Manager : Managers)
		{
			if (Manager->BelongToManager(Handle))
			{
				return Manager;
			}
		}

		return nullptr;
	}

	NxFr::HandleManager* HandleManager::GetHandlesManager()
	{
		NxFr::HandleManager* Result = FindHandleManager();
		if (Result == nullptr)
		{
			Result = CreateHandleManager();
		}

		return Result;
	}

	NxFr::HandleManager* HandleManager::FindHandleManager()
	{
		for (NxFr::HandleManager* Manager : Managers)
		{
			if (Manager->GetCount() < Manager->GetCapacity())
			{
				return Manager;
			}
		}

		return nullptr;
	}

	NxFr::HandleManager* HandleManager::CreateHandleManager()
	{
		NxFr::Allocator::Scope Context(nullptr);

		NxFr::HandleManager* Manager = new NxFr::HandleManager(HandlesPerManager);
		Managers.Append(Manager);
		return Manager;
	}

	void HandleManager::ClearHandleManager(bool Force)
	{
		NxFr::Allocator::Scope Context(nullptr);
		NxFr::List<NxFr::HandleManager*> ToDelete(Managers.GetCount());

		for (NxFr::HandleManager* Manager : Managers)
		{
			if (Force || Manager->IsEmpty())
			{
				ToDelete.Append(Manager);
				delete Manager;
			}
		}

		for (NxFr::HandleManager* Manager : ToDelete)
		{
			Managers.Remove(Manager);
		}
	}
}
