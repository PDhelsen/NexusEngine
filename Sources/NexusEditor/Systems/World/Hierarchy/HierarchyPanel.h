#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyPanel : public NxEn::GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyPanel)

		NEXUS_EDITOR_API void Refresh();
		NEXUS_EDITOR_API void SelectWorld(NxFr::StringId Id);
		NEXUS_EDITOR_API void SelectGameObject(NxFr::Handle<NxEn::GameObject> Target);

		template<typename T>
		void AppendAction();
		template<typename T>
		void RemoveAction();

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		void DrawHeader(float TimeStep);
		void DrawItem(NxFr::Handle<NxEn::GameObject> Instance);
		void DrawContext(NxFr::Handle<NxEn::GameObject> Instance);
		void SelectItem(NxFr::Handle<NxEn::GameObject> Instance);
		void OpenContext(NxFr::Handle<NxEn::GameObject> Instance);

		void ClearItems();
		void FetchItems();
		void AddItem(NxFr::Handle<NxEn::GameObject> Instance);
		void RemoveItem(NxFr::Handle<NxEn::GameObject> Instance);
		void OnGameObjectChanged(NxFr::StringId EventId, NxFr::StringId WorldId, NxFr::GUID GameObjectId);

		void Find();
		void Select(NxFr::Handle<NxEn::GameObject> Instance, bool Additive = false, bool List = false);
		void Show(NxFr::Handle<NxEn::GameObject> Instance);
		bool IsVisible(NxFr::Handle<NxEn::GameObject> Instance);

		void ProcessAction();
		NxFr::Array<NxFr::Handle<NxEn::GameObject>> GatherActionItems(HierarchyAction* Action);
		void GatherChildren(NxFr::Handle<NxEn::GameObject> Instance, NxFr::Set<NxFr::Handle<NxEn::GameObject>>& Result);
		void SortActions();

		NxEn::World* GetWorld() { return Worlds->GetWorld(WorldsIds[WorldIndex]); }

	private:
		NxEn::GUI::Style Style;
		NxEn::GUI::Menu Menu;
		NxFr::Array<NxFr::StringView> Labels;

		NxEn::WorldSystem* Worlds;
		NxEn::InputSystem* Inputs;

		NxFr::Array<NxFr::StringId> WorldsIds;
		uint64 WorldIndex;

		NxFr::Dictionary<NxFr::Handle<NxEn::GameObject>, HierarchyItem> Items;

		NxFr::Set<NxFr::Handle<NxEn::GameObject>> Selection;
		NxFr::Handle<NxEn::GameObject> Selected;

		NxFr::Set<NxFr::Handle<NxEn::GameObject>> Filtered;
		NxFr::String Filter;

		NxFr::List<HierarchyAction*> Actions;
		HierarchyAction* ActionRequested;
	};

	template<typename T>
	inline void HierarchyPanel::AppendAction()
	{
		Actions.Append(new T());
		SortActions();
	}

	template<typename T>
	inline void HierarchyPanel::RemoveAction()
	{
		uint64 Index = 0;
		for (; Index < Actions.GetCount(); ++Index)
		{
			if (Actions[Index]->GetObjectType() == T::GetClassType())
			{
				break;
			}
		}

		if (Index < Actions.GetCount())
		{
			delete Actions[Index];
			Actions.Remove(Index);
			SortActions();
		}
	}
}
