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
			NEXUS_EDITOR_API static Context* GetCurrent();
			NEXUS_EDITOR_API static void SetCurrent(Context* Instance);

			NEXUS_EDITOR_API Context(NxFr::StringId Id);
			NEXUS_EDITOR_API ~Context();

			NEXUS_EDITOR_API NxFr::Event<NxFr::GUID, bool>& GetOnSelectionChanged() { return OnSelectionChanged; }

			NEXUS_EDITOR_API NxFr::StringId GetId() const { return Id; }

		protected:
			NEXUS_EDITOR_API virtual NxFr::Array<NxFr::GUID> GetAll() = 0;
			NEXUS_EDITOR_API virtual uint64 GetCount() = 0;

			NEXUS_EDITOR_API virtual void Rename() = 0;
			NEXUS_EDITOR_API virtual void Duplicate() = 0;
			NEXUS_EDITOR_API virtual void Delete() = 0;
			NEXUS_EDITOR_API virtual void Cut() = 0;
			NEXUS_EDITOR_API virtual void Copy() = 0;
			NEXUS_EDITOR_API virtual void Paste() = 0;

		private:
			NEXUS_EDITOR_API void Select(NxFr::GUID InstanceId);
			NEXUS_EDITOR_API void Unselect(NxFr::GUID InstanceId);
			NEXUS_EDITOR_API void Invert(NxFr::GUID InstanceId);
			NEXUS_EDITOR_API bool IsSelected(NxFr::GUID InstanceId) const;
			NEXUS_EDITOR_API NxFr::GUID GetSelected() const;
			NEXUS_EDITOR_API NxFr::Array<NxFr::GUID> GetSelection() const;
			NEXUS_EDITOR_API uint64 GetSelectionCount() const;

		protected:
			NxFr::Event<NxFr::GUID, bool> OnSelectionChanged;

			NxFr::StringId Id;

			NxFr::GUID Selected;
			NxFr::Set<NxFr::GUID> Selection;
			NxFr::Set<NxFr::GUID> Clipboard;
		};
	}
}
