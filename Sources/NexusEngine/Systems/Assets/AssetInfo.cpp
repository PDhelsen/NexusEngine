#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetInfo.h"
#include "NexusEngine/Systems/Assets/Asset.h"

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
		if (Count > 0)
		{
			Count--;
		}
	}

	AssetMetadata::AssetMetadata()
		: Id(0), Type(0), Path(""), Extension(""), Data(3)
	{
	}

	AssetMetadata::AssetMetadata(Asset* Instance, NxFr::StringView Path, NxFr::StringView Extension)
		: Id(Instance->GetId()), Type(Instance->GetObjectType()), Path(Path), Extension(Extension), Data(3)
	{
		if (this->Path.IsEmpty())
		{
			this->Extension = "";
		}
	}

	AssetMetadata::~AssetMetadata()
	{
	}

	YAML::Node AssetMetadata::Serialize()
	{
		YAML::Node Node;
		Node["Id"] = Id;
		Node["Type"] = Type;
		Node["Path"] = Path;
		Node["Extension"] = Extension;
		Node["Dependencies"] = Dependencies;
		Node["Data"] = Data;
		return Node;
	}

	void AssetMetadata::Deserialize(const YAML::Node& Node)
	{
		Id = Node["Id"].as<NxFr::GUID>();
		Type = Node["Type"].as<NxFr::StringId>();
		Path = Node["Path"].as<NxFr::String>();
		Extension = Node["Extension"].as<NxFr::String>();
		Dependencies = Node["Dependencies"].as<NxFr::Array<NxFr::GUID>>();
		Data = Node["Data"].as<NxFr::Dictionary<NxFr::String, NxFr::String>>();
	}
}
