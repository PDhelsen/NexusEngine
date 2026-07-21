#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Prefab.h"

namespace NxEn
{
	NX_ASSET(Prefab)

	NxFr::Handle<GameObject> Prefab::GetRoot() const
	{
		return Root;
	}

	void Prefab::SetRoot(NxFr::Handle<GameObject> Instance)
	{
		Unload();

		Root = NxEn::Application::GetSystem<WorldSystem>()->PackPrefab(Instance, GetId());
		SetDirty();

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			Assets->Acquire(Dependency);
		}
	}

	void Prefab::OnDraw()
	{
		Asset::OnDraw();

		GUI::Drawer<NxFr::StringView>::Property(Root ? Root->GetName() : "", "Root");
	}

	void Prefab::OnClone(const Object& Other)
	{
		const Prefab& Instance = static_cast<const Prefab&>(Other);

		Unload();

		Root = NxEn::Application::GetSystem<WorldSystem>()->InstantiateGameObject(Instance.Root);
		SetDirty();

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			Assets->Acquire(Dependency);
		}
	}

	void Prefab::OnSave(NxFr::StringView Path) const
	{
		YAML::Node Data = NxEn::Application::GetSystem<WorldSystem>()->SerializePrefab(Root);
		NxFr::Yaml::SerializeAndSave(Data, Path);
	}

	void Prefab::OnLoad(NxFr::StringView Path)
	{
		YAML::Node Data = NxFr::Yaml::LoadAndDeserialize(Path);
		Root = NxEn::Application::GetSystem<WorldSystem>()->DeserializePrefab(Data);

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			Assets->Acquire(Dependency);
		}
	}

	void Prefab::OnUnload()
	{
		if (!Root)
		{
			return;
		}

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			Assets->Release(Dependency);
		}

		NxEn::Application::GetSystem<WorldSystem>()->UnloadPrefab(Root);
		Root = NxFr::Handle<GameObject>();
	}

	void Prefab::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) const
	{
		if (!Root)
		{
			return;
		}

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = Root->Object::GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			if (Assets->IsTracked(Dependency) && Dependency != GetId())
			{
				Ids.Append(Dependency);
			}
		}
	}
}
