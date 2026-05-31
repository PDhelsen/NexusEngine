#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Memory/HandleManager.h"

#include "NexusEngine/Systems/Settings/SettingTemplate.h"

namespace NxEn
{
	static SettingVar<uint64>* SettingHandlesPerManager = SettingVar<uint64>::Create("Settings", "MemoryHandlesPerManager", 1024);

	HandleManager::HandleManager()
		: Managers(4, nullptr)
	{
	}

	HandleManager::~HandleManager()
	{
		ClearHandleManager();
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

		NxFr::HandleManager* Manager = new NxFr::HandleManager(SettingHandlesPerManager->GetValue());
		Managers.Append(Manager);
		return Manager;
	}

	void HandleManager::ClearHandleManager()
	{
		NxFr::Allocator::Scope Context(nullptr);

		for (NxFr::HandleManager* Manager : Managers)
		{
			delete Manager;
		}

		Managers.Clear();
	}
}
