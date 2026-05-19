#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	class EditSystem : public NxEn::System
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, EditSystem)

		NX_EDITOR_API void RegisterContext(NxFr::StringId Id, Edit::Context* Ctx);
		NX_EDITOR_API Edit::Context* UnregisterContext(NxFr::StringId Id);
		NX_EDITOR_API Edit::Context* GetContext(NxFr::StringId Id = 0) const;

		NX_EDITOR_API NxFr::Array<NxFr::GUID> GetAll(NxFr::StringId ContextId = 0);
		NX_EDITOR_API uint64 GetCount(NxFr::StringId ContextId = 0);

		NX_EDITOR_API void Select(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Select(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Select(NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Unselect(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Unselect(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Unselect(NxFr::StringId ContextId = 0);
		NX_EDITOR_API void InvertSelection(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		NX_EDITOR_API void InvertSelection(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		NX_EDITOR_API void InvertSelection(NxFr::StringId ContextId = 0);
		NX_EDITOR_API bool IsSelected(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0) const;
		NX_EDITOR_API NxFr::GUID GetSelected(NxFr::StringId ContextId = 0) const;
		NX_EDITOR_API NxFr::Array<NxFr::GUID> GetSelection(NxFr::StringId ContextId = 0) const;
		NX_EDITOR_API uint64 SelectionCount(NxFr::StringId ContextId = 0) const;

		NX_EDITOR_API void Rename(NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Duplicate(NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Delete(NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Cut(NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Copy(NxFr::StringId ContextId = 0);
		NX_EDITOR_API void Paste(NxFr::StringId ContextId = 0);

	private:
		NxFr::Dictionary<NxFr::StringId, Edit::Context*> Contexts;
	};
}

