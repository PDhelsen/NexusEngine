#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsManager.h"

namespace NxEn
{
	AssetsManager::Info::Info(Asset* Instance)
		: Instance(Instance), Count(0)
	{
	}

	AssetsManager::AssetsManager()
		: Assets(), Loading()
	{
	}

	AssetsManager::~AssetsManager()
	{
	}

	void AssetsManager::Append(NxFr::GUID Id, const Info& Instance)
	{
		Assets.Append(Id, Instance);
	}

	void AssetsManager::Remove(NxFr::GUID Id)
	{
		NEXUS_ASSERT(!Loading.Contains(Id), System, "Not allowed to remove an asset that is still loading (%d)", Id);

		Assets.Remove(Id);
	}

	void AssetsManager::Load(NxFr::GUID Id, YAML::Node& Node)
	{
		NEXUS_ASSERT(!Loading.Contains(Id), System, "Circular loading dependecy detected (%d)", Id);

		Loading.Append(Id);

		Assets[Id].Instance->Load(Node);

		Loading.Remove(Id);
	}

	void AssetsManager::Unload(NxFr::GUID Id)
	{
		NEXUS_ASSERT(!Loading.Contains(Id), System, "Circular unloading dependecy detected (%d)", Id);

		Loading.Append(Id);

		Assets[Id].Instance->Unload();

		Loading.Remove(Id);
	}

	void AssetsManager::Save(NxFr::GUID Id, YAML::Node& Node)
	{
		Assets[Id].Instance->Save(Node);
	}

	Asset* AssetsManager::Get(NxFr::GUID Id)
	{
		Info& I = Assets[Id];
		return I.Instance;
	}

	void NxEn::AssetsManager::Acquire(NxFr::GUID Id)
	{
		Info& I = Assets[Id];
		I.Count++;
	}

	void NxEn::AssetsManager::Release(NxFr::GUID Id)
	{
		Info& I = Assets[Id];
		I.Count--;
	}

	bool AssetsManager::IsUsed(NxFr::GUID Id) const
	{
		const Info& I = Assets[Id];
		return I.Count > 0;
	}

	NxFr::List<NxFr::GUID> AssetsManager::GetUnused() const
	{
		NxFr::List<NxFr::GUID> Ids;

		for (auto& [Id, I] : Assets)
		{
			if (I.Count == 0)
			{
				Ids.Append(Id);
			}
		}

		return Ids;
	}

	NxFr::List<NxFr::GUID> AssetsManager::GetDirty() const
	{
		NxFr::List<NxFr::GUID> Ids;

		for (auto& [Id, I] : Assets)
		{
			if (I.Instance->IsDirty())
			{
				Ids.Append(Id);
			}
		}

		return Ids;
	}
}
