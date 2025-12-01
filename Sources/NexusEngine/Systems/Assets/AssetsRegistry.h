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

		NxFr::List<NxFr::GUID> Find(NxFr::StringView Filter) const;
		NxFr::String IdToPath(NxFr::GUID Id) const;
		NxFr::GUID PathToId(NxFr::StringView Path) const;

		bool IsValid(NxFr::GUID Id) const { return Assets.ContainsKey(Id); };
		uint64 GetCount() const { return Assets.GetCount(); }

	private:
		NxFr::Path FilePath(NxFr::StringView Path) const;
		NxFr::Path AssetPath(NxFr::StringView Path, NxFr::GUID Id) const;

	private:
		NxFr::Path Root;
		NxFr::Dictionary<NxFr::GUID, AssetMetadata> Assets;
		NxFr::Dictionary<NxFr::String, NxFr::GUID> Paths;
	};
}
