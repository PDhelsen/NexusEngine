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
		: Id(0), Type(0), Path(""), Extension(""), Dependencies(), Data(3)
	{
	}

	AssetMetadata::~AssetMetadata()
	{
	}

	NxFr::Yaml::Node AssetMetadata::Serialize() const
	{
		NxFr::Yaml::Node Node;
		Node["Id"] = Id;
		Node["Type"] = Type;
		Node["Name"] = Name;
		Node["Path"] = Path;
		Node["Extension"] = Extension;
		Node["Dependencies"] = Dependencies;
		Node["Data"] = Data;
		return Node;
	}

	void AssetMetadata::Deserialize(const NxFr::Yaml::Node& Node)
	{
		Id = Node["Id"].As<NxFr::GUID>();
		Type = Node["Type"].As<NxFr::StringId>();
		Name = Node["Name"].As<NxFr::String>();
		Path = Node["Path"].As<NxFr::String>();
		Extension = Node["Extension"].As<NxFr::String>();
		Dependencies = Node["Dependencies"].As<NxFr::Array<NxFr::GUID>>();
		Data = Node["Data"].As<NxFr::Dictionary<NxFr::String, NxFr::String>>();
	}
}
