#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class AssetsBrowserItem : public NxEn::Object
	{
		friend class AssetsBrowserPanel;

	public:
		enum class Type
		{
			Directory,
			File,
			Asset
		};

		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserItem)

		NEXUS_EDITOR_API AssetsBrowserItem(NxFr::GUID Id, NxFr::StringView FilePath);
		NEXUS_EDITOR_API ~AssetsBrowserItem();

		NEXUS_EDITOR_API Type GetType() const { return ItemType; }
		NEXUS_EDITOR_API NxFr::GUID GetId() const override { return Id; };
		NEXUS_EDITOR_API NxFr::StringView GetPath() const { return Path; }
		NEXUS_EDITOR_API NxFr::StringView GetPathWithoutExtension() const { return PathWithoutExtension; }
		NEXUS_EDITOR_API NxFr::StringView GetDirectory() const { return Directory; };
		NEXUS_EDITOR_API NxFr::StringView GetName() const override { return Name; };
		NEXUS_EDITOR_API NxFr::StringView GetExtension() const { return Extension; };
		NEXUS_EDITOR_API bool IsOpen() const { return Expanded; }
		NEXUS_EDITOR_API bool IsSelected() const { return Selected; }

		NEXUS_EDITOR_API bool operator==(const AssetsBrowserItem& Other) const;
		NEXUS_EDITOR_API bool operator!=(const AssetsBrowserItem& Other) const;
		NEXUS_EDITOR_API bool operator<(const AssetsBrowserItem& Other) const;
		NEXUS_EDITOR_API bool operator<=(const AssetsBrowserItem& Other) const;
		NEXUS_EDITOR_API bool operator>(const AssetsBrowserItem& Other) const;
		NEXUS_EDITOR_API bool operator>=(const AssetsBrowserItem& Other) const;

	private:
		NEXUS_EDITOR_API void GeneratePathInfo();
		NEXUS_EDITOR_API void GenerateImGuiText();

	private:
		NxFr::GUID Id;
		Type ItemType;

		NxFr::String Path;
		NxFr::StringView PathWithoutExtension;
		NxFr::StringView Directory;
		NxFr::StringView Name;
		NxFr::StringView Extension;
		uint8 Depth;

		NxFr::String ImGuiText;

		int64 Parent;
		int64 Next;
		bool Expanded;
		bool Selected;
	};
}
