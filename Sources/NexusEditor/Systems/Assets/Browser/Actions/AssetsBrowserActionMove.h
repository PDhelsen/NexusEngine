#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionMove : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionMove();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionMove();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;

	private:
		void Move(AssetsBrowserPanel* Browser, const NxFr::Array<AssetsBrowserItem*>& Items, NxFr::StringView Input) const;
		void Move(AssetsBrowserPanel* Browser, AssetsBrowserItem* Item, NxFr::StringView Input) const;
	};
}
