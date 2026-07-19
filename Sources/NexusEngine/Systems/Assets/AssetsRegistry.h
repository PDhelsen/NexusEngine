#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Assets/AssetInfo.h"

namespace NxEn
{
	class AssetsRegistry
	{
	public:
		AssetsRegistry(NxFr::String Root);
		~AssetsRegistry();

		AssetMetadata& Append(NxFr::StringId Type, NxFr::StringView Path, NxFr::StringView Extension);
		AssetMetadata& Move(NxFr::GUID Id, NxFr::StringView Path);
		AssetMetadata& Copy(NxFr::GUID Id, NxFr::StringView Path);
		void Remove(NxFr::GUID Id);
		AssetMetadata& Get(NxFr::GUID Id);

		void SerializeAndSave(NxFr::GUID Id, const YAML::Node& Assetdata);
		YAML::Node LoadAndDeserialize(NxFr::GUID Id);

		NxFr::Array<NxFr::GUID> Find(NxFr::StringView Query) const;
		NxFr::GUID PathToId(NxFr::StringView Path) const;
		NxFr::StringView IdToPath(NxFr::GUID Id) const;
		NxFr::String IdToAssetFsPath(NxFr::GUID Id) const;
		NxFr::String IdToContentFsPath(NxFr::GUID Id) const;

		bool IsValid(NxFr::GUID Id) const { return Assets.TryGet(Id); };
		bool HasFile(NxFr::GUID Id) const { return Assets[Id].HasFile(); }
		uint64 GetCount() const { return Assets.GetCount(); }

	private:
		YAML::Node Read(NxFr::StringView Path, NxFr::StringView Key);
		void Write(NxFr::StringView Path, const YAML::Node& Metadata, const YAML::Node& Assetdata);
		NxFr::String PathToFsPath(NxFr::StringView Path, NxFr::StringView Extension) const;

		NxFr::String Root;
		NxFr::Dictionary<NxFr::GUID, AssetMetadata> Assets;
		NxFr::Dictionary<NxFr::String, NxFr::GUID> Paths;
	};
}
