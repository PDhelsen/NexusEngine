#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEd
{
	class AssetsBrowserActionCreate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionCreate)

		NX_EDITOR_API AssetsBrowserActionCreate() : TreeAction("Create", 1, false, false) { }
		NX_EDITOR_API virtual ~AssetsBrowserActionCreate() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionRename : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionRename)

		NX_EDITOR_API AssetsBrowserActionRename() : TreeAction("Rename", 2, false, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionRename() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionDuplicate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionDuplicate)

		NX_EDITOR_API AssetsBrowserActionDuplicate() : TreeAction("Duplicate", 3, false, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionDuplicate() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionMove : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionMove)

		NX_EDITOR_API AssetsBrowserActionMove() : TreeAction("Move", 4, false, true) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionMove() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionDelete : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionDelete)

		NX_EDITOR_API AssetsBrowserActionDelete() : TreeAction("Delete", 5, false, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionDelete() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionImport : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionImport)

		NX_EDITOR_API AssetsBrowserActionImport() : TreeAction("Import", 6, true, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionImport() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionReimport : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionReimport)

		NX_EDITOR_API AssetsBrowserActionReimport() : TreeAction("Reimport", 7, true, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionReimport() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionLoad : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionLoad)

		NX_EDITOR_API AssetsBrowserActionLoad() : TreeAction("Load", 8, true, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionLoad() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionReload : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionReload)

		NX_EDITOR_API AssetsBrowserActionReload() : TreeAction("Reload", 9, true, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionReload() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionInstantiate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionReload)

		NX_EDITOR_API AssetsBrowserActionInstantiate() : TreeAction("Instantiate", 10, false, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionInstantiate() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionView : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionView)

		NX_EDITOR_API AssetsBrowserActionView() : TreeAction("View", 11, false, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionView() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class AssetsBrowserActionReferences : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, AssetsBrowserActionReferences)

		NX_EDITOR_API AssetsBrowserActionReferences() : TreeAction("References", 12, false, false) {}
		NX_EDITOR_API virtual ~AssetsBrowserActionReferences() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};
}

