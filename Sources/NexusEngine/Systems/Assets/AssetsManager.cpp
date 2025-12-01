#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsManager.h"

namespace NxEn
{
	AssetsManager::AssetsManager()
		: Assets(), Loading()
	{
	}

	AssetsManager::~AssetsManager()
	{
	}

	void AssetsManager::Append(NxFr::GUID Id, const AssetHandle& Handle)
	{
		Assets.Append(Id, Handle);
	}

	void AssetsManager::Remove(NxFr::GUID Id)
	{
		NEXUS_ASSERT(!Loading.Contains(Id), System, "Not allowed to remove an asset that is still loading (%d)", Id);

		Assets.Remove(Id);
	}

	AssetHandle& AssetsManager::Get(NxFr::GUID Id)
	{
		return Assets[Id];
	}

	void AssetsManager::Load(NxFr::GUID Id, YAML::Node& Node)
	{
		NEXUS_ASSERT(!Loading.Contains(Id), System, "Circular loading dependecy detected (%d)", Id);

		Loading.Append(Id);

		Assets[Id].GetInstance()->Load(Node);

		Loading.Remove(Id);
	}

	void AssetsManager::Unload(NxFr::GUID Id)
	{
		NEXUS_ASSERT(!Loading.Contains(Id), System, "Circular unloading dependecy detected (%d)", Id);

		Loading.Append(Id);

		Assets[Id].GetInstance()->Unload();

		Loading.Remove(Id);
	}

	void AssetsManager::Save(NxFr::GUID Id, YAML::Node& Node)
	{
		Assets[Id].GetInstance()->Save(Node);
	}

	void NxEn::AssetsManager::Acquire(NxFr::GUID Id)
	{
		Assets[Id].Acquire();
	}

	void NxEn::AssetsManager::Release(NxFr::GUID Id)
	{
		Assets[Id].Release();
	}

	bool AssetsManager::IsUsed(NxFr::GUID Id) const
	{
		return Assets[Id].IsUsed();
	}

	NxFr::List<NxFr::GUID> AssetsManager::GetUnused() const
	{
		NxFr::List<NxFr::GUID> Ids;

		for (auto& [Id, I] : Assets)
		{
			if (I.GetCount() == 0)
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
			if (I.GetInstance()->IsDirty())
			{
				Ids.Append(Id);
			}
		}

		return Ids;
	}
}
