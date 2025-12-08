#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace AssetSerializer
	{
		const NxFr::String NodeMetadata = "Metadata";
		const NxFr::String NodeData = "Data";

		void Serialize(NxFr::StringView Path, const YAML::Node& Metadata, const YAML::Node& Data)
		{
			YAML::Node Root;
			Root[NodeMetadata] = Metadata;
			Root[NodeData] = Data;
			NxFr::Yaml::SerializeFile(Root, Path);
		}

		void Deserialize(NxFr::StringView Path, YAML::Node& Metadata, YAML::Node& Data)
		{
			YAML::Node Root = NxFr::Yaml::DeserializeFile(Path);
			Metadata = Root[NodeMetadata];
			Data = Root[Data];
		}

		YAML::Node DeserializeMetadata(NxFr::StringView Path)
		{
			YAML::Node Metadata, Data;
			Deserialize(Path, Metadata, Data);
			return Metadata;
		}

		YAML::Node DeserializeData(NxFr::StringView Path)
		{
			YAML::Node Metadata, Data;
			Deserialize(Path, Metadata, Data);
			return Data;
		}
	}
}
