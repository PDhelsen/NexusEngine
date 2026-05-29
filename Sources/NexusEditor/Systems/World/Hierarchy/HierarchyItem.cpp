#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	HierarchyItem::HierarchyItem(HierarchyManager* Manager, NxFr::Handle<NxEn::GameObject> Target)
		: Manager(Manager), Target(Target), Name(""), ReferenceId(0)
	{
		SetTickable(true);
	}

	HierarchyItem::~HierarchyItem()
	{
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

	HierarchyItem* HierarchyItem::GetParent() const
	{
		return Manager->Convert(Target->GetParent());
	}

	HierarchyItem* HierarchyItem::GetPrevious() const
	{
		return Manager->Convert(Target->GetPrevious());
	}

	HierarchyItem* HierarchyItem::GetNext() const
	{
		return Manager->Convert(Target->GetNext());
	}

	HierarchyItem* HierarchyItem::GetChild() const
	{
		return Manager->Convert(Target->GetChild());
	}
}
