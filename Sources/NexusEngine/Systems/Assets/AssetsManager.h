#pragma once

#include "NexusEngine/Systems/Assets/AssetMetadata.h"

namespace NxEn
{
	class AssetsManager
	{
	public:
		AssetsManager();
		~AssetsManager();

		void Append(NxFr::GUID Id, const AssetHandle& Handle);
		void Remove(NxFr::GUID Id);
		AssetHandle& Get(NxFr::GUID Id);

		YAML::Node Save(NxFr::GUID Id, NxFr::StringView ContentFsPath);
		void Load(NxFr::GUID Id, const YAML::Node& Node, NxFr::StringView ContentFsPath);
		void Unload(NxFr::GUID Id);

		void Acquire(NxFr::GUID Id);
		void Release(NxFr::GUID Id);
		bool IsUsed(NxFr::GUID Id) const;

		NxFr::List<NxFr::GUID> GetUnused() const;
		NxFr::List<NxFr::GUID> GetDirty() const;

		bool IsValid(NxFr::GUID Id) const { return Assets.TryGet(Id); };
		uint64 GetCount() const { return Assets.GetCount(); }

	private:
		NxFr::Dictionary<NxFr::GUID, AssetHandle> Assets;
		NxFr::Set<NxFr::GUID> Loading;
	};
}
