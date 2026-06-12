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

	AssetMetadata::AssetMetadata(NxFr::GUID Id, NxFr::StringId Type, NxFr::StringView Path, NxFr::StringView Extension)
		: Id(Id), Type(Type), Name(NxFr::Path::GetName(Path)), Path(Path), Extension(Extension), Data(3)
	{

	}

	AssetMetadata::~AssetMetadata()
	{
	}

	YAML::Node AssetMetadata::Serialize() const
	{
		YAML::Node Node;
		Node["Id"] = Id;
		Node["Type"] = Type;
		Node["Name"] = Name;
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
		Name = Node["Name"].as<NxFr::String>();
		Path = Node["Path"].as<NxFr::String>();
		Extension = Node["Extension"].as<NxFr::String>();
		Dependencies = Node["Dependencies"].as<NxFr::Array<NxFr::GUID>>();
		Data = Node["Data"].as<NxFr::Dictionary<NxFr::String, NxFr::String>>();
	}
}
