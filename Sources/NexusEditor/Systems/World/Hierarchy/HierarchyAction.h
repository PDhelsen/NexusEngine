#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEd
{
	class NX_EDITOR_API HierarchyActionCreate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(HierarchyActionCreate)

		HierarchyActionCreate() : TreeAction("Create", 1, false, false) {}
		virtual ~HierarchyActionCreate() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API HierarchyActionRename : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(HierarchyActionRename)

		HierarchyActionRename() : TreeAction("Rename", 2, false, false) {}
		virtual ~HierarchyActionRename() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API HierarchyActionDuplicate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(HierarchyActionDuplicate)

		HierarchyActionDuplicate() : TreeAction("Duplicate", 3, false, false) {}
		virtual ~HierarchyActionDuplicate() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API HierarchyActionMove : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(HierarchyActionMove)

		HierarchyActionMove() : TreeAction("Move", 4, false, true) {}
		virtual ~HierarchyActionMove() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API HierarchyActionDelete : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(HierarchyActionDelete)

		HierarchyActionDelete() : TreeAction("Delete", 5, false, false) {}
		virtual ~HierarchyActionDelete() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	/*class NX_EDITOR_API HierarchyActionPrefabCreate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(HierarchyActionPrefabCreate)

		HierarchyActionPrefabCreate() : TreeAction("Prefab - Create", 6, false, false) {}
		virtual ~HierarchyActionPrefabCreate() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API HierarchyActionPrefabSave : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(HierarchyActionPrefabSave)

		HierarchyActionPrefabSave() : TreeAction("Prefab - Save", 7, false, false) {}
		virtual ~HierarchyActionPrefabSave() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class NX_EDITOR_API HierarchyActionPrefabUnpack : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(HierarchyActionPrefabUnpack)

		HierarchyActionPrefabUnpack() : TreeAction("Prefab - Unpack", 8, false, false) {}
		virtual ~HierarchyActionPrefabUnpack() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};*/

	class NX_EDITOR_API HierarchyActionInspect : public NxEn::TreeAction
	{
	public:
		NX_OBJECT(HierarchyActionInspect)

		HierarchyActionInspect() : TreeAction("Inspect", 9, false, false) {}
		virtual ~HierarchyActionInspect() = default;

		void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};
}
