#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(HierarchyItem)

	HierarchyItem::HierarchyItem(NxFr::Handle<NxEn::GameObject> GameObject, const NxFr::Delegate<TreeItem*(NxFr::Handle<NxEn::GameObject>)>& Convert)
		: TreeItem(), Name(), Reference(), GameObject(GameObject), Convert(Convert)
	{
		SetTickable(true);
	}

	HierarchyItem::~HierarchyItem()
	{
	}

	void HierarchyItem::OnTick(float TimeStep)
	{
		GenerateImGuiText();
	}

	void HierarchyItem::GenerateImGuiText()
	{
		if (Name == GameObject->GetName() && Reference == GameObject->GetReferenceId())
		{
			return;
		}

		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxEn::AssetMetadata* Metadata = System->IsTracked(GameObject->GetReferenceId()) ? &System->GetMetadata(GameObject->GetReferenceId()) : nullptr;

		NxFr::StringView Prefix =
			GameObject == GameObject->GetWorld()->GetRootGameObject() ? "W" :
			Metadata && Metadata->GetType() == NxEn::Scene::GetClassType() ? "S" :
			Metadata && Metadata->GetType() == NxEn::Prefab::GetClassType() ? "P" :
			"G";

		ImGuiText = Prefix + " " + GameObject->GetName() + "##" + NxFr::StringUtility::ToString(GameObject->GetId());

		Name = NxFr::StringUtility::Split(ImGuiText, "##");
		Reference = GameObject->GetReferenceId();
	}

	int8 HierarchyItem::Compare(const TreeItem& Other) const
	{
		return GameObject->GetOrderIndex() < static_cast<const HierarchyItem&>(Other).GameObject->GetOrderIndex();
	}
}
