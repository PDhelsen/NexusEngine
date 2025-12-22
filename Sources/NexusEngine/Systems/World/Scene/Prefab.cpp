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
		Root->ReferenceId = 0;

		Instance->ReferenceId = GetId();

		SetDirty();
	}

	void Prefab::OnSave(YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::Delegate<void(YAML::Emitter&, NxFr::Handle<GameObject>)> Save = [&](YAML::Emitter& Data, NxFr::Handle<GameObject> Instance)
		{
			bool IsPrefab = Instance->GetReferenceId();

			Data << YAML::BeginMap;

			Data << YAML::Key << "Id" << YAML::Value << Instance->GetId();
			Data << YAML::Key << "Reference" << YAML::Value << Instance->GetReferenceId();

			if (!IsPrefab)
			{
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
			}

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
			NxFr::Handle<GameObject> Instance;
			NxFr::GUID Id = Data["Id"].as<NxFr::GUID>();
			NxFr::GUID Reference = Data["Reference"].as<NxFr::GUID>();

			if (Reference)
			{
				Prefab* PrefabInstance = Application::GetSystem<AssetsSystem>()->Acquire<Prefab>(Reference);
				Instance = Factory.Instantiate(*PrefabInstance, Parent);
			}
			else
			{
				Instance = Factory.CreateGameObject(Data["Name"].as<NxFr::String>(), Id, Parent);
				YAML::Node Children = Data["Children"];

				if (Children.size() > 0)
				{
					Instance->Child = Load(Children[0], Instance);
					NxFr::Handle<GameObject> ChildInstance = Instance->GetChild();

					for (uint64 Index = 1; Index < Children.size(); ++Index)
					{
						ChildInstance->Next = Load(Children[Index], Instance);
						ChildInstance = ChildInstance->GetNext();
					}
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

			if (Instance->GetReferenceId())
			{
				Application::GetSystem<AssetsSystem>()->Release(Instance->GetReferenceId());
				return;
			}

			NxFr::Handle<GameObject> Child = Instance->GetChild();
			while (Child)
			{
				Unload(Child);
				Child = Child->GetNext();
			}
		};

		Unload(Root);
		Factory.DestroyGameObject(Root);
		Root = NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::GUID> Prefab::GetDependencies()
	{
		NxFr::Set<NxFr::GUID> Dependencies;
		GetDependencies(Root, Dependencies);
		return NxFr::ContainersUtils::ToArray<NxFr::GUID>(Dependencies);
	}

	void Prefab::GetDependencies(NxFr::Handle<GameObject> Instance, NxFr::Set<NxFr::GUID>& Result) const
	{
		if (!Instance)
		{
			return;
		}

		if (Instance->GetReferenceId())
		{
			Result.Append(Instance->GetReferenceId());
			return;
		}

		NxFr::Handle<GameObject> Child = Instance->GetChild();
		while (Child)
		{
			GetDependencies(Child, Result);
			Child = Child->GetNext();
		}
	}
}
