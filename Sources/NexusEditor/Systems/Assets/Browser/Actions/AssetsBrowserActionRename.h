#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionRename : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionRename();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionRename();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;

	private:
		void Rename(AssetsBrowserPanel* Browser, const NxFr::Array<AssetsBrowserItem*>& Items, NxFr::StringView Input) const;
		void Rename(AssetsBrowserPanel* Browser, AssetsBrowserItem* Item, NxFr::StringView Input) const;
	};
}
