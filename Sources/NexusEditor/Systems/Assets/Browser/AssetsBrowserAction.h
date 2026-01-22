#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEd
{
	class AssetsBrowserActionCreate : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionCreate)

		NEXUS_EDITOR_API AssetsBrowserActionCreate() : TreeAction("Create", 1, false, false) { }
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionCreate() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionRename : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionRename)

		NEXUS_EDITOR_API AssetsBrowserActionRename() : TreeAction("Rename", 2, false, false) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionRename() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionDuplicate : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionDuplicate)

		NEXUS_EDITOR_API AssetsBrowserActionDuplicate() : TreeAction("Duplicate", 3, false, false) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionDuplicate() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionMove : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionMove)

		NEXUS_EDITOR_API AssetsBrowserActionMove() : TreeAction("Move", 4, false, true) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionMove() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionDelete : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionDelete)

		NEXUS_EDITOR_API AssetsBrowserActionDelete() : TreeAction("Delete", 5, false, false) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionDelete() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionImport : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionImport)

		NEXUS_EDITOR_API AssetsBrowserActionImport() : TreeAction("Import", 6, true, false) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionImport() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionReimport : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionReimport)

		NEXUS_EDITOR_API AssetsBrowserActionReimport() : TreeAction("Reimport", 7, true, false) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionReimport() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionLoad : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionLoad)

		NEXUS_EDITOR_API AssetsBrowserActionLoad() : TreeAction("Load", 8, true, false) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionLoad() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionReload : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionReload)

		NEXUS_EDITOR_API AssetsBrowserActionReload() : TreeAction("Reload", 9, true, false) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionReload() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionInstantiate : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionReload)

		NEXUS_EDITOR_API AssetsBrowserActionInstantiate() : TreeAction("Instantiate", 10, false, false) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionInstantiate() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionReferences : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserActionReferences)

		NEXUS_EDITOR_API AssetsBrowserActionReferences() : TreeAction("References", 11, false, false) {}
		NEXUS_EDITOR_API virtual ~AssetsBrowserActionReferences() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};
}

