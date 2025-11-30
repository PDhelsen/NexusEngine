#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/Assets/AssetMetadata.h"

namespace NxEn
{
	class AssetsRegistry
	{
	public:
		struct Info
		{
			Info(AssetMetadata Metadata);
			Info(Asset* Instance, NxFr::StringView Path = "");

			NxFr::StringId Type;
			NxFr::Path Path;
		};

		AssetsRegistry(NxFr::Path Root);
		~AssetsRegistry();

		void Append(NxFr::GUID Id, const Info& Instance);
		void Move(NxFr::GUID Id, NxFr::StringView Path);
		void Remove(NxFr::GUID Id);

		void Serialize(NxFr::GUID Id, YAML::Node& Node) const;
		void Deserialize(NxFr::GUID Id, YAML::Node& Node) const;

		NxFr::List<NxFr::GUID> Find(NxFr::StringView Filter) const;
		NxFr::String IdToPath(NxFr::GUID Id) const;
		NxFr::GUID PathToId(NxFr::StringView Path) const;

		bool IsValid(NxFr::GUID Id) const { return Assets.ContainsKey(Id); };
		uint64 GetCount() const { return Assets.GetCount(); }

	private:
		NxFr::Path FilePath(NxFr::StringView Path) const;
		NxFr::String AssetPath(NxFr::StringView Path, NxFr::GUID Id) const;

	private:
		NxFr::Path Root;
		NxFr::Dictionary<NxFr::GUID, Info> Assets;
		NxFr::Dictionary<NxFr::String, NxFr::GUID> Paths;
	};
}
