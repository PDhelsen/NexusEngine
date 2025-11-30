#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	struct AssetMetadata
	{
		AssetMetadata();
		AssetMetadata(Asset* Instance, NxFr::StringView Path);
		~AssetMetadata();

		NxFr::GUID Id;
		NxFr::StringId Type;
		NxFr::String Path;
	};	
}

namespace YAML
{
	template<>
	struct convert<NxEn::AssetMetadata>
	{
		static Node encode(const NxEn::AssetMetadata& rhs)
		{
			Node node;
			node["Id"] = rhs.Id;
			node["Type"] = rhs.Type;
			node["Path"] = rhs.Path;
			return node;
		}

		static bool decode(const Node& node, NxEn::AssetMetadata& rhs)
		{
			rhs.Id = node["Id"].as<NxFr::GUID>();
			rhs.Type = node["Type"].as<NxFr::StringId>();
			rhs.Path = node["Path"].as<NxFr::String>();
			return true;
		}
	};
}
