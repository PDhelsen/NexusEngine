#pragma once

#include "NexusEngine/Systems/Assets/AssetMetadata.h"

namespace NxEn
{
	class AssetsRegistry
	{
	public:
		AssetsRegistry(NxFr::Path Root);
		~AssetsRegistry();

		void Append(NxFr::GUID Id, const AssetMetadata& Metadata);
		void Move(NxFr::GUID Id, NxFr::StringView Path);
		void Remove(NxFr::GUID Id);
		AssetMetadata& Get(NxFr::GUID Id);

		void Serialize(NxFr::GUID Id, YAML::Node& Node);
		void Deserialize(NxFr::GUID Id, YAML::Node& Node);

		YAML::Node GetImportData(NxFr::GUID Id);

		NxFr::Array<NxFr::GUID> Find(NxFr::StringView Filter) const;
		NxFr::GUID PathToId(NxFr::StringView Path) const;
		NxFr::String IdToPath(NxFr::GUID Id) const;
		NxFr::String IdToFile(NxFr::GUID Id) const;
		NxFr::String IdToContent(NxFr::GUID Id) const;

		bool IsValid(NxFr::GUID Id) const { return Assets.ContainsKey(Id); };
		bool HasFile(NxFr::GUID Id) const { return Assets[Id].GetPath().IsValid(); }
		uint64 GetCount() const { return Assets.GetCount(); }

	private:
		NxFr::String FilePath(NxFr::StringView Path) const;
		NxFr::String ContentPath(NxFr::StringView Path) const;

	private:
		NxFr::Path Root;
		NxFr::Dictionary<NxFr::GUID, AssetMetadata> Assets;
		NxFr::Dictionary<NxFr::String, NxFr::GUID> Paths;
	};
}
