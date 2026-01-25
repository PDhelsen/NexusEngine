#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

namespace NxEd
{
	static NxFr::Dictionary<NxFr::GUID, HierarchyItem*> Instances;

	NEXUS_OBJECT_IMPLEMENTATION(HierarchyItem)

	HierarchyItem::HierarchyItem(NxFr::Handle<NxEn::GameObject> Target)
		: Target(Target), Name(""), ReferenceId(0)
	{
		SetTickable(true);

		NEXUS_ASSERT(!Instances.ContainsKey(Target->GetId()), System, "GameObject %d is already tracked", Target->GetId());
		Instances.Append(Target->GetId(), this);
	}

	HierarchyItem::~HierarchyItem()
	{
		Instances.Remove(Target->GetId());
	}

	void HierarchyItem::OnTick(float TimeStep)
	{
		CacheImGuiText();
	}

	int8 HierarchyItem::Compare(const TreeItem& Other) const
	{
		return Target->GetOrderIndex() - static_cast<const HierarchyItem&>(Other).Target->GetOrderIndex();
	}

	void HierarchyItem::CacheImGuiText()
	{
		if (Name == Target->GetName() && ReferenceId == Target->GetReferenceId())
		{
			return;
		}

		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxEn::AssetMetadata* Metadata = System->IsTracked(Target->GetReferenceId()) ? &System->GetMetadata(Target->GetReferenceId()) : nullptr;

		NxFr::StringView Prefix =
			Target == Target->GetWorld()->GetRootGameObject() ? "W" :
			Metadata && Metadata->GetType() == NxEn::Scene::GetClassType() ? "S" :
			Metadata && Metadata->GetType() == NxEn::Prefab::GetClassType() ? "P" :
			"G";

		ImGuiText = Prefix + " " + Target->GetName() + "##" + NxFr::StringUtility::ToString(Target->GetId());

		Name = Target->GetName();
		ReferenceId = Target->GetReferenceId();
	}

	void HierarchyItem::Clear()
	{
		auto Temp = NxFr::ContainersUtils::ToArray(Instances);
		for (auto Instance : Temp)
		{
			delete Instance.Value;
		}
	}

	HierarchyItem* HierarchyItem::Convert(NxFr::Handle<NxEn::GameObject> Target)
	{
		if (!Target)
		{
			return nullptr;
		}

		HierarchyItem** Instance = Instances.TryGet(Target->GetId());
		return Instance ? *Instance : nullptr;
	}
}
