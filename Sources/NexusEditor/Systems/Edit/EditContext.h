#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class EditSystem;

	namespace Edit
	{
		class Context
		{
			friend class EditSystem;

		public:
			NX_EDITOR_API static Context* GetCurrent();
			NX_EDITOR_API static void SetCurrent(Context* Instance);

			NX_EDITOR_API Context(NxFr::StringId Id);
			NX_EDITOR_API ~Context();

			NX_EDITOR_API NxFr::Event<NxFr::GUID, bool>& GetOnSelectionChanged() { return OnSelectionChanged; }

			NX_EDITOR_API NxFr::StringId GetId() const { return Id; }

		protected:
			NX_EDITOR_API virtual NxFr::Array<NxFr::GUID> GetAll() = 0;
			NX_EDITOR_API virtual uint64 GetCount() = 0;

			NX_EDITOR_API virtual void Select(NxFr::GUID InstanceId);
			NX_EDITOR_API virtual void Unselect(NxFr::GUID InstanceId);
			NX_EDITOR_API virtual void Invert(NxFr::GUID InstanceId);
			NX_EDITOR_API virtual bool IsSelected(NxFr::GUID InstanceId) const;
			NX_EDITOR_API virtual NxFr::GUID GetSelected() const;
			NX_EDITOR_API virtual NxFr::Array<NxFr::GUID> GetSelection() const;
			NX_EDITOR_API virtual uint64 GetSelectionCount() const;

			NX_EDITOR_API virtual void Rename() = 0;
			NX_EDITOR_API virtual void Duplicate() = 0;
			NX_EDITOR_API virtual void Delete() = 0;
			NX_EDITOR_API virtual void Cut() = 0;
			NX_EDITOR_API virtual void Copy() = 0;
			NX_EDITOR_API virtual void Paste() = 0;

		protected:
			NxFr::Event<NxFr::GUID, bool> OnSelectionChanged;

			NxFr::StringId Id;

			NxFr::GUID Selected;
			NxFr::Set<NxFr::GUID> Selection;
			NxFr::Set<NxFr::GUID> Clipboard;
		};
	}
}
