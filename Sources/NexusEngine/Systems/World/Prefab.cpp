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

		Root = NxEn::Application::GetSystem<WorldSystem>()->PackPrefab(Instance);
		SetDirty();
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
	}

	void Prefab::OnSave(NxFr::StringView Path) const
	{
		YAML::Node Data = NxEn::Application::GetSystem<WorldSystem>()->SerializePrefab(Root);
		NxFr::Yaml::SerializeFile(Data, Path);
	}

	void Prefab::OnLoad(NxFr::StringView Path)
	{
		YAML::Node Data = NxFr::Yaml::DeserializeFile(Path);
		Root = NxEn::Application::GetSystem<WorldSystem>()->DeserializePrefab(Data);
	}

	void Prefab::OnUnload()
	{
		if (!Root)
		{
			return;
		}

		NxEn::Application::GetSystem<WorldSystem>()->UnloadPrefab(Root);
		Root = NxFr::Handle<GameObject>();
	}
}
