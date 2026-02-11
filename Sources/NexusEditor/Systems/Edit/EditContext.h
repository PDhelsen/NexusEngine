#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

#define NEXUS_EDITCONTEXT_DECLARATION(Dll, Label)\
Dll NxFr::StringId GetObjectLabel() const override { return #Label##_Sid; }\
Dll static NxFr::StringId GetClassLabel() { return #Label##_Sid; }

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
			NEXUS_EDITOR_API virtual NxFr::StringId GetObjectLabel() const = 0;

		protected:
			NEXUS_EDITOR_API virtual NxFr::Array<NxFr::GUID> GetAll() = 0;
			NEXUS_EDITOR_API virtual uint64 GetCount() = 0;

			NEXUS_EDITOR_API void Select(NxFr::GUID InstanceId);
			NEXUS_EDITOR_API void Unselect(NxFr::GUID InstanceId);
			NEXUS_EDITOR_API void Invert(NxFr::GUID InstanceId);
			NEXUS_EDITOR_API bool IsSelected(NxFr::GUID InstanceId) const;
			NEXUS_EDITOR_API NxFr::GUID GetSelected() const;
			NEXUS_EDITOR_API NxFr::Array<NxFr::GUID> GetSelection() const;
			NEXUS_EDITOR_API uint64 GetSelectionCount() const;

			NEXUS_EDITOR_API virtual void Rename();
			NEXUS_EDITOR_API virtual void Duplicate();
			NEXUS_EDITOR_API virtual void Delete();

		private:
			NxFr::Event<NxFr::GUID, bool> OnSelectionChanged;

			NxFr::StringId Id;

			NxFr::Set<NxFr::GUID> Selection;
			NxFr::GUID Selected;
		};
	}
}
