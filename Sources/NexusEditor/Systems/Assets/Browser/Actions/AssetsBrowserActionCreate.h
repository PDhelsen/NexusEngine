#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionCreate : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionCreate();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionCreate();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;

	private:
		void Create(AssetsBrowserPanel* Browser, const NxFr::Array<AssetsBrowserItem*>& Items, NxFr::StringView Name, NxFr::StringId Type) const;
		void Create(AssetsBrowserPanel* Browser, AssetsBrowserItem* Item, NxFr::StringView Name, NxFr::StringId Type) const;
	};
}
