#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowserActionCreate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionCreate)

		AssetsBrowserActionCreate() : TreeAction("Create", 1, false, false) { }
		virtual ~AssetsBrowserActionCreate() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionRename : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionRename)

		AssetsBrowserActionRename() : TreeAction("Rename", 2, false, false) {}
		virtual ~AssetsBrowserActionRename() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionDuplicate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionDuplicate)

		AssetsBrowserActionDuplicate() : TreeAction("Duplicate", 3, false, false) {}
		virtual ~AssetsBrowserActionDuplicate() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionMove : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionMove)

		AssetsBrowserActionMove() : TreeAction("Move", 4, false, true) {}
		virtual ~AssetsBrowserActionMove() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionDelete : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionDelete)

		AssetsBrowserActionDelete() : TreeAction("Delete", 5, false, false) {}
		virtual ~AssetsBrowserActionDelete() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionImport : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionImport)

		AssetsBrowserActionImport() : TreeAction("Import", 6, true, false) {}
		virtual ~AssetsBrowserActionImport() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionReimport : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionReimport)

		AssetsBrowserActionReimport() : TreeAction("Reimport", 7, true, false) {}
		virtual ~AssetsBrowserActionReimport() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionLoad : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionLoad)

		AssetsBrowserActionLoad() : TreeAction("Load", 8, true, false) {}
		virtual ~AssetsBrowserActionLoad() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionReload : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionReload)

		AssetsBrowserActionReload() : TreeAction("Reload", 9, true, false) {}
		virtual ~AssetsBrowserActionReload() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionInstantiate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionReload)

		AssetsBrowserActionInstantiate() : TreeAction("Instantiate", 10, false, false) {}
		virtual ~AssetsBrowserActionInstantiate() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API AssetsBrowserActionView : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(AssetsBrowserActionView)

		AssetsBrowserActionView() : TreeAction("View", 11, false, false) {}
		virtual ~AssetsBrowserActionView() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};
}

