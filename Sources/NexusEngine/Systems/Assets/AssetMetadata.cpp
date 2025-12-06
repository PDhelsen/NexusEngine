#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetMetadata.h"

namespace NxEn
{
	AssetHandle::AssetHandle(Asset* Instance)
		: Instance(Instance), Count(0)
	{
	}

	AssetHandle::~AssetHandle()
	{
	}

	void AssetHandle::Acquire()
	{
		Count++;
	}

	void AssetHandle::Release()
	{
		Count--;
	}

	AssetMetadata::AssetMetadata()
		: Id(0), Type(0), Path(""), Content(""), Data(3)
	{
	}

	AssetMetadata::AssetMetadata(Asset* Instance, NxFr::StringView Path, NxFr::StringView Extension)
		: Id(Instance->GetId()), Type(Instance->GetObjectType()), Path(""), Content(""), Data(3)
	{
		if (!Path.IsEmpty())
		{
			this->Path = Path;
			if (!Extension.IsEmpty())
			{
				this->Content = Path + "." + Extension;
			}
		}
	}

	AssetMetadata::~AssetMetadata()
	{
	}

	void AssetMetadata::Serialize(YAML::Node& Node)
	{
		YAML::Node Metadata = YAML::Node();
		Metadata["Id"] = Id;
		Metadata["Type"] = Type;
		Metadata["Path"] = Path.Data;
		Metadata["Content"] = Content.Data;
		Metadata["Dependencies"] = Dependencies;
		Metadata["Data"] = Data;

		Node["Metadata"] = Metadata;
	}

	void AssetMetadata::Deserialize(YAML::Node& Node)
	{
		YAML::Node Metadata = Node["Metadata"];

		Id = Metadata["Id"].as<NxFr::GUID>();
		Type = Metadata["Type"].as<NxFr::StringId>();
		Path = Metadata["Path"].as<NxFr::String>();
		Content = Metadata["Content"].as<NxFr::String>();
		Dependencies = Metadata["Dependencies"].as<NxFr::Array<NxFr::GUID>>();
		Data = Metadata["Data"].as<NxFr::Dictionary<NxFr::String, NxFr::String>>();
	}
}
