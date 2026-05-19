#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEd
{
	class HierarchyActionCreate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyActionCreate)

		NX_EDITOR_API HierarchyActionCreate() : TreeAction("Create", 1, false, false) {}
		NX_EDITOR_API virtual ~HierarchyActionCreate() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionRename : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyActionRename)

		NX_EDITOR_API HierarchyActionRename() : TreeAction("Rename", 2, false, false) {}
		NX_EDITOR_API virtual ~HierarchyActionRename() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionDuplicate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyActionDuplicate)

		NX_EDITOR_API HierarchyActionDuplicate() : TreeAction("Duplicate", 3, false, false) {}
		NX_EDITOR_API virtual ~HierarchyActionDuplicate() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionMove : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyActionMove)

		NX_EDITOR_API HierarchyActionMove() : TreeAction("Move", 4, false, true) {}
		NX_EDITOR_API virtual ~HierarchyActionMove() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionDelete : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyActionDelete)

		NX_EDITOR_API HierarchyActionDelete() : TreeAction("Delete", 5, false, false) {}
		NX_EDITOR_API virtual ~HierarchyActionDelete() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionPrefabCreate : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyActionPrefabCreate)

		NX_EDITOR_API HierarchyActionPrefabCreate() : TreeAction("Prefab - Create", 6, false, false) {}
		NX_EDITOR_API virtual ~HierarchyActionPrefabCreate() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionPrefabSave : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyActionPrefabSave)

		NX_EDITOR_API HierarchyActionPrefabSave() : TreeAction("Prefab - Save", 7, false, false) {}
		NX_EDITOR_API virtual ~HierarchyActionPrefabSave() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionPrefabUnpack : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyActionPrefabUnpack)

		NX_EDITOR_API HierarchyActionPrefabUnpack() : TreeAction("Prefab - Unpack", 8, false, false) {}
		NX_EDITOR_API virtual ~HierarchyActionPrefabUnpack() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionInspect : public NxEn::TreeAction
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, HierarchyActionInspect)

		NX_EDITOR_API HierarchyActionInspect() : TreeAction("Inspect", 9, false, false) {}
		NX_EDITOR_API virtual ~HierarchyActionInspect() = default;

		NX_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};
}
