#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionLoad : public AssetsBrowserAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionLoad)

		NEXUS_EDITOR_API AssetsBrowserActionLoad();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionLoad();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;
	};
}

