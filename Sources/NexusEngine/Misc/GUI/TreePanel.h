#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Elements/Panel.h"

namespace NxEn
{
	struct NX_ENGINE_API TreeAction
	{
		bool operator<=(const TreeAction& Other) const { return Priority <= Other.Priority; }

		NxFr::String Name;
		NxFr::Delegate<void()> Action;
		int64 Priority;
	};

	struct NX_ENGINE_API TreeItem
	{
		virtual NxFr::GUID GetId() const = 0;
		virtual NxFr::StringView GetName() const = 0;
		virtual NxFr::StringView GetLabel() const = 0;
		virtual NxFr::StringView GetDescription() const = 0;
		virtual NxFr::StringId GetType() const = 0;

		virtual NxFr::GUID GetParent() const = 0;
		virtual NxFr::GUID GetNext() const = 0;
		virtual NxFr::GUID GetPrevious() const = 0;
		virtual NxFr::GUID GetChild() const = 0;

		virtual bool IsOpen() const = 0;
		virtual void Open(bool State) = 0;

		virtual bool Compare(const TreeItem& Other) const = 0;
	};

	class NX_ENGINE_API TreePanel : public GUI::Panel
	{
	public:
		NX_OBJECT(TreePanel)

		virtual TreeItem* GetItem(NxFr::GUID Id) = 0;

		virtual NxFr::Array<NxFr::GUID> GetSelected(NxFr::GUID* Active = nullptr);
		virtual NxFr::GUID GetRoot();
		virtual void SetRoot(NxFr::GUID Id);

		virtual void Clear();
		virtual void Refresh();
		virtual void Select(NxFr::GUID Id, bool State = true, bool Additive = false, bool List = false, bool ApplyFilter = true);
		virtual bool IsSelected(NxFr::GUID Id, bool* Active = nullptr);
		virtual void Find(NxFr::StringView Query);
		virtual bool IsFiltered(NxFr::GUID Id);
		virtual void SetVisible(NxFr::GUID Id, bool State = true);
		virtual bool IsVisible(NxFr::GUID Id);

		virtual void AppendAction(const TreeAction& Action);
		virtual void RemoveAction(NxFr::StringView Action);

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnDraw() override;

		virtual void OnCreateItem(NxFr::GUID Id);
		virtual void OnDestroyItem(NxFr::GUID Id);
		virtual void OnSelectItem(NxFr::GUID Id, bool State);

		void DrawHeader();
		void DrawItem(NxFr::GUID Id);
		void DrawAction(NxFr::StringView Label);
		void HandleSelection(NxFr::GUID Id, bool Selected);
		void ProcessAction();

		NxFr::GUID GetIterator(NxFr::GUID Id);
		NxFr::List<TreeAction>::I GetAction(NxFr::StringView Name);

		GUI::Menu Menu;

		InputSystem* Inputs;

		NxFr::GUID Root;
		NxFr::Set<NxFr::GUID> Selection;
		NxFr::GUID Selected;
		NxFr::Set<NxFr::GUID> Filtered;
		NxFr::String Filter;
		NxFr::List<TreeAction> Actions;
		NxFr::String Action;
	};
}
