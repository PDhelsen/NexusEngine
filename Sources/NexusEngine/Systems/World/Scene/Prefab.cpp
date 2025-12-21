#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Scene/Prefab.h"
#include "NexusEngine/Systems/World/ObjectFactory.h"

namespace NxEn
{
	static ObjectFactory Factory("Prefab"_Sid);

	NEXUS_ASSET_IMPLEMENTATION(Prefab)

	NxFr::Handle<GameObject> Prefab::GetRoot() const
	{
		return Root;
	}

	void Prefab::SetRoot(NxFr::Handle<GameObject> Instance)
	{
		Root = Factory.DuplicateGameObject(Instance);
		Instance->ReferenceId = GetId();
		SetDirty();
	}

	void Prefab::OnSave(YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::Delegate<void(YAML::Emitter&, NxFr::Handle<GameObject>)> Save = [&](YAML::Emitter& Data, NxFr::Handle<GameObject> Instance)
		{
			Data << YAML::BeginMap;
			Data << YAML::Key << "Id" << YAML::Value << Instance->GetId();
			Data << YAML::Key << "Name" << YAML::Value << Instance->GetName();
			Data << YAML::Key << "Children" << YAML::Value;
			Data << YAML::BeginSeq;
			NxFr::Handle<GameObject> InstanceChild = Instance->GetChild();
			while (InstanceChild)
			{
				Save(Data, InstanceChild);
				InstanceChild = InstanceChild->GetNext();
			}
			Data << YAML::EndSeq;
			Data << YAML::EndMap;
		};

		YAML::Emitter Data;
		Save(Data, Root);

		NxFr::Yaml::SerializeFile(Data, Content);
		Node["Count"] = 1 + Root->GetChildCount(true);
	}

	void Prefab::OnLoad(const YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::Delegate<NxFr::Handle<GameObject>(YAML::Node, NxFr::Handle<GameObject>)> Load = [&](YAML::Node Data, NxFr::Handle<GameObject> Parent)
		{
			NxFr::Handle<GameObject> Instance = Factory.CreateGameObject(Data["Name"].as<NxFr::String>(), Data["Id"].as<NxFr::GUID>(), Parent);

			YAML::Node ChildrenNode = Data["Children"];
			if (ChildrenNode.size() > 0)
			{
				Instance->Child = Load(ChildrenNode[0], Instance);
				NxFr::Handle<GameObject> ChildInstance = Instance->GetChild();

				for (uint64 Index = 1; Index < ChildrenNode.size(); ++Index)
				{
					ChildInstance->Next = Load(ChildrenNode[Index], Instance);
					ChildInstance = ChildInstance->GetNext();
				}
			}

			return Instance;
		};

		Factory.Reserve(Node["Count"].as<uint64>());
		YAML::Node Data = NxFr::Yaml::DeserializeFile(Content);

		Root = Load(Data, NxFr::Handle<GameObject>());
	}

	void Prefab::OnUnload()
	{
		NxFr::Delegate<void(NxFr::Handle<GameObject>)> Unload = [&](NxFr::Handle<GameObject> Instance)
		{
			if (!Instance)
			{
				return;
			}

			NxFr::Handle<GameObject> InstanceChild = Instance->GetChild();
			while (InstanceChild)
			{
				Unload(InstanceChild);
			}

			Factory.DestroyGameObject(Instance);
		};

		Unload(Root);
	}
}
