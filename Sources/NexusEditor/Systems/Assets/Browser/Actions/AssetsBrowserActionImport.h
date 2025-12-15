#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionImport : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionImport();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionImport();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;
	};
}

