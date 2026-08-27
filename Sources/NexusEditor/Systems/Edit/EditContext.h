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
			Context(NxFr::StringId Id);
			~Context();

			NxFr::StringId GetId() const { return Id; }

			NxFr::Event<NxFr::GUID>& GetOnCreated() { return OnCreated; }
			NxFr::Event<NxFr::GUID>& GetOnDestroyed() { return OnDestroyed; }
			NxFr::Event<NxFr::GUID, bool>& GetOnSelection() { return OnSelection; }

		protected:
			virtual NxFr::Array<NxFr::GUID> GetAll() = 0;
			virtual uint64 GetCount() = 0;
			virtual void Clear();

			virtual void Create() = 0;
			virtual void Rename() = 0;
			virtual void Move() = 0;
			virtual void Duplicate() = 0;
			virtual void Delete() = 0;
			virtual void Cut();
			virtual void Copy();
			virtual void Paste();

			virtual void Select(NxFr::GUID InstanceId);
			virtual void Unselect(NxFr::GUID InstanceId);
			virtual bool IsSelected(NxFr::GUID InstanceId) const;
			virtual NxFr::GUID GetSelected() const;
			virtual NxFr::Array<NxFr::GUID> GetSelection() const;
			virtual uint64 GetSelectionCount() const;

		protected:
			NxFr::StringId Id;

			NxFr::Event<NxFr::GUID> OnCreated;
			NxFr::Event<NxFr::GUID> OnDestroyed;
			NxFr::Event<NxFr::GUID, bool> OnSelection;

			NxFr::Set<NxFr::GUID> Clipboard;
			NxFr::Set<NxFr::GUID> Selection;
			NxFr::GUID Selected;
			bool IsCutting;
		};
	}
}
