#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class EditSystem;

	namespace Edit
	{
		class NX_EDITOR_API Context
		{
			friend class EditSystem;

		public:
			static Context* GetCurrent();
			static void SetCurrent(Context* Instance);

			Context(NxFr::StringId Id);
			~Context();

			NxFr::Event<NxFr::GUID, bool>& GetOnSelectionChanged() { return OnSelectionChanged; }

			NxFr::StringId GetId() const { return Id; }

		protected:
			virtual NxFr::Array<NxFr::GUID> GetAll() = 0;
			virtual uint64 GetCount() = 0;

			virtual void Select(NxFr::GUID InstanceId);
			virtual void Unselect(NxFr::GUID InstanceId);
			virtual void Invert(NxFr::GUID InstanceId);
			virtual bool IsSelected(NxFr::GUID InstanceId) const;
			virtual NxFr::GUID GetSelected() const;
			virtual NxFr::Array<NxFr::GUID> GetSelection() const;
			virtual uint64 GetSelectionCount() const;

			virtual void Rename() = 0;
			virtual void Duplicate() = 0;
			virtual void Delete() = 0;
			virtual void Cut() = 0;
			virtual void Copy() = 0;
			virtual void Paste() = 0;

		protected:
			NxFr::Event<NxFr::GUID, bool> OnSelectionChanged;

			NxFr::StringId Id;

			NxFr::GUID Selected;
			NxFr::Set<NxFr::GUID> Selection;
			NxFr::Set<NxFr::GUID> Clipboard;
		};
	}
}
