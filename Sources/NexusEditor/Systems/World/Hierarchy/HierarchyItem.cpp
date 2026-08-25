#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

namespace NxEd
{
	HierarchyItem::HierarchyItem()
		: Target(), Opened(false), Label(""), Cache("", 0)
	{
	}

	HierarchyItem::~HierarchyItem()
	{
	}

	bool HierarchyItem::Compare(const TreeItem& Other) const
	{
		return Target->GetOrderIndex() < static_cast<const HierarchyItem&>(Other).Target->GetOrderIndex();
	}

	void HierarchyItem::CacheLabel() const
	{
		if (Target->GetName() == Cache.GetFirst() && Target->GetTemplateId() == Cache.GetSecond())
		{
			return;
		}

		NxEn::AssetMetadata* Metadata = nullptr;
		if (Target->GetTemplateId())
		{
			Metadata = NxEn::Application::GetSystem<NxEn::AssetsSystem>()->GetMetadata(Target->GetTemplateId());
		}

		NxFr::StringView Prefix =
			Target == Target->GetWorld()->GetRoot() ? "W" :
			Metadata && Metadata->GetType() == NxEn::Scene::GetClassType() ? "S" :
			Metadata && Metadata->GetType() == NxEn::Prefab::GetClassType() ? "P" :
			"G";

		Label = NxEn::GUI::Utils::NexusToImGuiId(Prefix + " " + Target->GetName(), Target->GetId());

		Cache.SetFirst(Target->GetName());
		Cache.SetSecond(Target->GetTemplateId());
	}
}
