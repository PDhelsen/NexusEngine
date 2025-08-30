#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	class EditSystem : public NxEn::System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, EditSystem)

		NEXUS_EDITOR_API EditSystem();
		NEXUS_EDITOR_API ~EditSystem();

		NEXUS_EDITOR_API Edit::Context& CreateContext(NxFr::StringId Id);
		NEXUS_EDITOR_API const Edit::Context& GetContext(NxFr::StringId Id) const;
		NEXUS_EDITOR_API const Edit::Context& GetCurrentContext() const;

		NEXUS_EDITOR_API void Select(NxEn::Object* Target, NxFr::StringId Ctx = 0);
		NEXUS_EDITOR_API void Select(NxFr::Collection<NxEn::Object*> Targets, NxFr::StringId Ctx = 0);
		NEXUS_EDITOR_API void SelectAll(NxFr::StringId Ctx = 0);
		NEXUS_EDITOR_API void Unselect(NxEn::Object* Target, NxFr::StringId Ctx = 0);
		NEXUS_EDITOR_API void Unselect(NxFr::Collection<NxEn::Object*> Targets, NxFr::StringId Ctx = 0);
		NEXUS_EDITOR_API void UnselectAll(NxFr::StringId Ctx = 0);
		NEXUS_EDITOR_API void ToggleSelection(NxEn::Object* Target, NxFr::StringId Ctx = 0);
		NEXUS_EDITOR_API void InvertSelection(NxFr::StringId Ctx = 0);
		NEXUS_EDITOR_API bool IsSelected(NxEn::Object* Target, NxFr::StringId Ctx = 0) const;
		NEXUS_EDITOR_API NxEn::Object* Selected() const;
		NEXUS_EDITOR_API NxFr::List<NxEn::Object*> SelectedAll() const;
		NEXUS_EDITOR_API uint64 SelectionCount() const;

		NEXUS_EDITOR_API void Rename();
		NEXUS_EDITOR_API void Delete();
		NEXUS_EDITOR_API void Cut();
		NEXUS_EDITOR_API void Copy();
		NEXUS_EDITOR_API void Paste();
		NEXUS_EDITOR_API void Duplicate();

		NEXUS_EDITOR_API void Undo();
		NEXUS_EDITOR_API void Redo();
		NEXUS_EDITOR_API void RecordUndo(NxEn::Object* Target, NxFr::StringId Ctx = 0);
		NEXUS_EDITOR_API void RecordRedo(NxEn::Object* Target, NxFr::StringId Ctx = 0);

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;

		NEXUS_EDITOR_API const Edit::Context& GetCtx(NxFr::StringId Ctx) const;
		NEXUS_EDITOR_API NxFr::List<Edit::Info>::I FindInfo(NxFr::StringId Ctx, NxEn::Object* Target) const;

	private:
		NxFr::Dictionary<NxFr::StringId, Edit::Context> Contexts;
		NxFr::List<Edit::Info>* Selection;
		NxFr::List<Edit::Info>* Clipboard;
		NxFr::Dequeue<Edit::Info>* HistoryUndo;
		NxFr::Dequeue<Edit::Info>* HistoryRedo;

		NxEn::Allocator* Allocator;
	};
}
