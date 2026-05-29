#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	class NX_EDITOR_API EditSystem : public NxEn::System
	{
	public:
		NX_OBJECT(EditSystem)

		void RegisterContext(NxFr::StringId Id, Edit::Context* Ctx);
		Edit::Context* UnregisterContext(NxFr::StringId Id);
		Edit::Context* GetContext(NxFr::StringId Id = 0) const;

		NxFr::Array<NxFr::GUID> GetAll(NxFr::StringId ContextId = 0);
		uint64 GetCount(NxFr::StringId ContextId = 0);

		void Select(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		void Select(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		void Select(NxFr::StringId ContextId = 0);
		void Unselect(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		void Unselect(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		void Unselect(NxFr::StringId ContextId = 0);
		void InvertSelection(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		void InvertSelection(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		void InvertSelection(NxFr::StringId ContextId = 0);
		bool IsSelected(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0) const;
		NxFr::GUID GetSelected(NxFr::StringId ContextId = 0) const;
		NxFr::Array<NxFr::GUID> GetSelection(NxFr::StringId ContextId = 0) const;
		uint64 SelectionCount(NxFr::StringId ContextId = 0) const;

		void Rename(NxFr::StringId ContextId = 0);
		void Duplicate(NxFr::StringId ContextId = 0);
		void Delete(NxFr::StringId ContextId = 0);
		void Cut(NxFr::StringId ContextId = 0);
		void Copy(NxFr::StringId ContextId = 0);
		void Paste(NxFr::StringId ContextId = 0);

	private:
		NxFr::Dictionary<NxFr::StringId, Edit::Context*> Contexts;
	};
}

