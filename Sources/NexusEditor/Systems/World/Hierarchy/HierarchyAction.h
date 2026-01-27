#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Components/Tree/TreeAction.h"

namespace NxEd
{
	class HierarchyActionCreate : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionCreate)

		NEXUS_EDITOR_API HierarchyActionCreate() : TreeAction("Create", 1, false, false) {}
		NEXUS_EDITOR_API virtual ~HierarchyActionCreate() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionRename : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionRename)

		NEXUS_EDITOR_API HierarchyActionRename() : TreeAction("Rename", 2, false, false) {}
		NEXUS_EDITOR_API virtual ~HierarchyActionRename() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionDuplicate : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionDuplicate)

		NEXUS_EDITOR_API HierarchyActionDuplicate() : TreeAction("Duplicate", 3, false, false) {}
		NEXUS_EDITOR_API virtual ~HierarchyActionDuplicate() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionMove : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionMove)

		NEXUS_EDITOR_API HierarchyActionMove() : TreeAction("Move", 4, false, true) {}
		NEXUS_EDITOR_API virtual ~HierarchyActionMove() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionDelete : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionDelete)

		NEXUS_EDITOR_API HierarchyActionDelete() : TreeAction("Delete", 5, false, false) {}
		NEXUS_EDITOR_API virtual ~HierarchyActionDelete() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionPrefabCreate : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionPrefabCreate)

		NEXUS_EDITOR_API HierarchyActionPrefabCreate() : TreeAction("Prefab - Create", 6, false, false) {}
		NEXUS_EDITOR_API virtual ~HierarchyActionPrefabCreate() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionPrefabSave : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionPrefabSave)

		NEXUS_EDITOR_API HierarchyActionPrefabSave() : TreeAction("Prefab - Save", 7, false, false) {}
		NEXUS_EDITOR_API virtual ~HierarchyActionPrefabSave() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionPrefabUnpack : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionPrefabUnpack)

		NEXUS_EDITOR_API HierarchyActionPrefabUnpack() : TreeAction("Prefab - Unpack", 8, false, false) {}
		NEXUS_EDITOR_API virtual ~HierarchyActionPrefabUnpack() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};

	class HierarchyActionInspect : public NxEn::TreeAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionInspect)

		NEXUS_EDITOR_API HierarchyActionInspect() : TreeAction("Inspect", 9, false, false) {}
		NEXUS_EDITOR_API virtual ~HierarchyActionInspect() = default;

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxEn::TreeItem*>& Items) override;
	};
}
