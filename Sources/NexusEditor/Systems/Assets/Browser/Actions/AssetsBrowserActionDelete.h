#pragma once

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"

namespace NxEd
{
	class AssetsBrowserActionDelete : public AssetsBrowserAction
	{
	public:
		NEXUS_EDITOR_API AssetsBrowserActionDelete();
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionDelete();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<AssetsBrowserItem*>& Items) override;

	private:
		void Delete(AssetsBrowserPanel* Browser, const NxFr::Array<NxFr::String>& Items) const;
		void Delete(AssetsBrowserPanel* Browser, NxFr::String Item) const;
	};
}
