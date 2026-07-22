#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Prefab.h"

namespace NxEn
{
	NX_ASSET(Prefab)

	void Prefab::OnDraw()
	{
		Asset::OnDraw();

		GUI::Drawer<NxFr::StringView>::Property(Root ? Root->GetName() : "", "Root");
	}

	void Prefab::OnClone(const Object& Other)
	{
		const Prefab& Instance = static_cast<const Prefab&>(Other);

		Application::GetSystem<WorldSystem>()->PackPrefab(this, Instance.Root);
	}

	void Prefab::OnSave(NxFr::StringView Path) const
	{
		YAML::Node Data = Application::GetSystem<WorldSystem>()->SerializeGameObject(Root);
		NxFr::Yaml::SerializeAndSave(Data, Path);
	}

	void Prefab::OnLoad(NxFr::StringView Path)
	{
		YAML::Node Data = NxFr::Yaml::LoadAndDeserialize(Path);
		SetRoot(Application::GetSystem<WorldSystem>()->DeserializeGameObject(Data));
	}

	void Prefab::OnUnload()
	{
		if (!Root)
		{
			return;
		}

		Application::GetSystem<WorldSystem>()->UnloadGameObject(Root);

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			Assets->Release(Dependency);
		}

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

	void Prefab::SetRoot(NxFr::Handle<GameObject> Instance)
	{
		Unload();

		Root = Instance;
		SetDirty();

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			Assets->Acquire(Dependency);
		}
	}
}
