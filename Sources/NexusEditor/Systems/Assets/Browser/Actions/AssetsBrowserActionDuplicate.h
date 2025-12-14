#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionDuplicate : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionDuplicate();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionDuplicate();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;

	private:
		void Duplicate(AssetsBrowserPanel* Browser, const NxFr::Array<AssetsBrowserItem*>& Items) const;
		void Duplicate(AssetsBrowserPanel* Browser, AssetsBrowserItem* Item) const;
	};
}
