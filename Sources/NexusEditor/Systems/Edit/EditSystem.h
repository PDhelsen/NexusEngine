#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	class NX_EDITOR_API EditSystem : public NxEn::System
	{
	public:
		NX_OBJECT(EditSystem)

		void RegisterContext(Edit::Context* Ctx);
		Edit::Context* UnregisterContext(NxFr::StringId Id);
		void SetContext(NxFr::StringId Id);
		Edit::Context* GetContext(NxFr::StringId Id = 0) const;
		template<typename T>
		T* GetContext(NxFr::StringId Id = 0) const { return (T*)GetContext(Id); }

		NxFr::Array<NxFr::GUID> GetAll(NxFr::StringId ContextId = 0);
		uint64 GetCount(NxFr::StringId ContextId = 0);
		void Clear(NxFr::StringId ContextId = 0);

		void Create(NxFr::StringId ContextId = 0);
		void Rename(NxFr::StringId ContextId = 0);
		void Move(NxFr::StringId ContextId = 0);
		void Duplicate(NxFr::StringId ContextId = 0);
		void Delete(NxFr::StringId ContextId = 0);
		void Cut(NxFr::StringId ContextId = 0);
		void Copy(NxFr::StringId ContextId = 0);
		void Paste(NxFr::StringId ContextId = 0);

		void Select(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		void Select(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		void Select(NxFr::StringId ContextId = 0);
		void Unselect(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		void Unselect(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		void Unselect(NxFr::StringId ContextId = 0);
		bool IsSelected(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0) const;
		NxFr::GUID GetSelected(NxFr::StringId ContextId = 0) const;
		NxFr::Array<NxFr::GUID> GetSelection(NxFr::StringId ContextId = 0) const;
		uint64 SelectionCount(NxFr::StringId ContextId = 0) const;

		void SetSelected(NxFr::GUID InstanceId, bool State, NxFr::StringId ContextId = 0);
		void SetSelected(NxFr::Collection<NxFr::GUID> InstancesId, bool State, NxFr::StringId ContextId = 0);
		void SetSelected(bool State, NxFr::StringId ContextId = 0);
		void SetSelection(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		void SetSelection(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		void SetSelection(NxFr::StringId ContextId = 0);
		void InvertSelection(NxFr::GUID InstanceId, NxFr::StringId ContextId = 0);
		void InvertSelection(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId = 0);
		void InvertSelection(NxFr::StringId ContextId = 0);

	private:
		NxFr::Dictionary<NxFr::StringId, Edit::Context*> Contexts;
		NxFr::StringId Context;
	};
}

