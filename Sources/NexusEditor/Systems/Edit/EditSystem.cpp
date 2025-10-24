#include "NexusEditor/Systems/Edit/EditSystem.h"

#include "NexusEngine/Systems/Settings/SettingTemplate.h"

namespace NxEd
{
	static NxEn::SettingVar<uint64>* SettingHistoryCapacity = NxEn::SettingVar<uint64>::Create("Editor", "EditHistoryCapacity", 64);

	const static NxEn::Command CmdEditUndo = NxEn::Command::Create("Edit.Undo"_Sid, "Rename selected obejct", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Undo();
	}));
	const static NxEn::Command CmdEditRedo = NxEn::Command::Create("Edit.Redo"_Sid, "Rename selected obejct", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Redo();
	}));
	const static NxEn::Command CmdEditRename = NxEn::Command::Create("Edit.Rename"_Sid, "Rename selected obejct", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Rename();
	}));
	const static NxEn::Command CmdEditDelete = NxEn::Command::Create("Edit.Delete"_Sid, "Rename selected obejcts", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Delete();
	}));
	const static NxEn::Command CmdEditCut = NxEn::Command::Create("Edit.Cut"_Sid, "Cut selected obejcts", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Cut();
	}));
	const static NxEn::Command CmdEditCopy = NxEn::Command::Create("Edit.Copy"_Sid, "Copy selected obejcts", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Copy();
	}));
	const static NxEn::Command CmdEditPaste = NxEn::Command::Create("Edit.Paste"_Sid, "Paste selected obejcts", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Paste();
	}));
	const static NxEn::Command CmdEditDuplicate = NxEn::Command::Create("Edit.Duplicate"_Sid, "Duplicate selected obejcts", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Duplicate();
	}));
	const static NxEn::Command CmdEditSelectAll = NxEn::Command::Create("Edit.SelectAll"_Sid, "Select all object in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->SelectAll();
	}));
	const static NxEn::Command CmdEditUnselectAll = NxEn::Command::Create("Edit.UnselectAll"_Sid, "Unselect all object in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->UnselectAll();
	}));
	const static NxEn::Command CmdEditInvert = NxEn::Command::Create("Edit.InvertSelection"_Sid, "Invert selection in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->InvertSelection();
	}));

	NEXUS_OBJECT_IMPLEMENTATION(EditSystem)

	EditSystem::EditSystem()
		: Contexts(), Selection(), Clipboard(), HistoryUndo(), HistoryRedo()
	{
		CreateContext(Edit::Context::Dummy);
	}

	EditSystem::~EditSystem()
	{
	}

	Edit::Context& EditSystem::CreateContext(NxFr::StringId Id)
	{
		return Contexts.Append(Id, Edit::Context(Id));
	}

	const Edit::Context& EditSystem::GetContext(NxFr::StringId Id) const
	{
		return Contexts[Id];
	}

	const Edit::Context& EditSystem::GetCurrentContext() const
	{
		NxEn::GUI::Panel* Panel = NxEn::Application::GetSystem<NxEn::GUISystem>()->GetActivePanel();
		if (!Panel)
		{
			return Contexts[Edit::Context::Dummy];
		}

		const Edit::Context* Context = Contexts.TryGet(Panel->GetObjectType());
		if (!Context)
		{
			return Contexts[Edit::Context::Dummy];
		}

		return *Context;
	}

	void EditSystem::Select(NxEn::Object* Target, NxFr::StringId Ctx)
	{
		auto Iterator = FindInfo(Ctx, Target);
		if (Iterator == Selection.End())
		{
			Selection.AppendConstruct(Ctx, Target->GetId(), nullptr);
		}
	}

	void EditSystem::Select(NxFr::Collection<NxEn::Object*> Targets, NxFr::StringId Ctx)
	{
		for (auto& It = Targets.Reset(); It != Targets.End(); ++It)
		{
			Select(*It, Ctx);
		}
	}

	void EditSystem::SelectAll(NxFr::StringId Ctx)
	{
		const Edit::Context& Context = GetCtx(Ctx);
		if (!Context.IsBinded(Context.Select))
		{
			return;
		}

		NxFr::List<NxEn::Object*> Targets = Context.Select.Invoke();
		Select(Targets, Context.Id);
	}

	void EditSystem::Unselect(NxEn::Object* Target, NxFr::StringId Ctx)
	{
		auto Iterator = FindInfo(Ctx, Target);
		if (Iterator != Selection.End())
		{
			Selection.Remove(Iterator.Id());
		}
	}

	void EditSystem::Unselect(NxFr::Collection<NxEn::Object*> Targets, NxFr::StringId Ctx)
	{
		for (auto& It = Targets.Reset(); It != Targets.End(); ++It)
		{
			Unselect(*It, Ctx);
		}
	}

	void EditSystem::UnselectAll(NxFr::StringId Ctx)
	{
		const Edit::Context& Context = GetCtx(Ctx);
		if (!Context.IsBinded(Context.Convert))
		{
			return;
		}

		NxFr::List<NxEn::Object*> Targets(Selection.GetCount());
		for (auto& Info : Selection)
		{
			NxEn::Object* Target = Context.TryGet(Info.Target, Info.Context);
			if (!Target)
			{
				continue;
			}

			Targets.Append(Target);
		}

		Unselect(Targets, Context.Id);
	}

	void EditSystem::ToggleSelection(NxEn::Object* Target, NxFr::StringId Ctx)
	{
		if (!IsSelected(Target, Ctx))
		{
			Select(Target, Ctx);
		}
		else
		{
			Unselect(Target, Ctx);
		}
	}

	void EditSystem::InvertSelection(NxFr::StringId Ctx)
	{
		NxFr::List<NxEn::Object*> Targets = SelectedAll();
		SelectAll(Ctx);
		Unselect(Targets, Ctx);
	}

	bool EditSystem::IsSelected(NxEn::Object* Target, NxFr::StringId Ctx) const
	{
		return FindInfo(Ctx, Target) != Selection.End();
	}

	NxEn::Object* EditSystem::Selected() const
	{
		if (Selection.IsEmpty())
		{
			return nullptr;
		}

		const Edit::Info& Info = Selection.Get(0);
		const Edit::Context& Context = GetContext(Info.Context);
		return Context.TryGet(Info.Target);
	}

	NxFr::List<NxEn::Object*> EditSystem::SelectedAll() const
	{
		NxFr::List<NxEn::Object*> Result(Selection.GetCount());
		for (auto& Info : Selection)
		{
			const Edit::Context& Context = GetContext(Info.Context);
			NxEn::Object* Target = Context.TryGet(Info.Target);
			if (!Target)
			{
				continue;
			}

			Result.Append(Target);
		}

		return Result;
	}

	uint64 EditSystem::SelectionCount() const
	{
		return Selection.GetCount();
	}

	void EditSystem::Rename()
	{
		if (Selection.IsEmpty())
		{
			return;
		}

		const Edit::Info& Info = Selection.Get(0);
		const Edit::Context& Context = GetContext(Info.Context);
		NxEn::Object* Target = Context.TryGet(Info.Target);
		if (!Target)
		{
			return;
		}

		if (!Context.IsBinded(Context.Rename))
		{
			return;
		}

		Context.Rename(Target);
	}

	void EditSystem::Delete()
	{
		for (auto& Info : Selection)
		{
			const Edit::Context& Context = GetContext(Info.Context);
			NxEn::Object* Target = Context.TryGet(Info.Target);
			if (!Target)
			{
				continue;
			}

			if (!Context.IsBinded(Context.Delete))
			{
				continue;
			}

			Context.Delete(Target);
		}

		Selection.Clear();
	}

	void EditSystem::Cut()
	{
		Copy();
		Paste();
		Delete();
	}

	void EditSystem::Copy()
	{
		Clipboard.Clear();
		Clipboard.Reserve(Selection.GetCount());

		for (auto& Info : Selection)
		{
			const Edit::Context& Context = GetContext(Info.Context);
			NxEn::Object* Target = Context.TryGet(Info.Target);
			if (!Target)
			{
				continue;
			}

			NxEn::Object* Data = Target->Clone();
			Clipboard.AppendConstruct(Info.Context, Info.Target, Data);
		}
	}

	void EditSystem::Paste()
	{
		for (auto& Info : Clipboard)
		{
			const Edit::Context& Context = GetContext(Info.Context);
			if (!Info.Data)
			{
				continue;
			}

			if (!Context.IsBinded(Context.Paste))
			{
				continue;
			}

			Context.Paste(Info.Data);
		}

		Clipboard.Clear();
	}

	void EditSystem::Duplicate()
	{
		Copy();
		Paste();
	}

	void EditSystem::Undo()
	{
		if (HistoryUndo.IsEmpty())
		{
			return;
		}

		const Edit::Info& Info = HistoryUndo.Last();
		const Edit::Context& Context = GetContext(Info.Context);
		NxEn::Object* Target = Context.TryGet(Info.Target);
		if (!Target)
		{
			return;
		}

		RecordRedo(Target, Info.Context);
		Target->Clone(Info.Data);

		HistoryUndo.RemoveBack();
	}

	void EditSystem::Redo()
	{
		if (HistoryRedo.IsEmpty())
		{
			return;
		}

		const Edit::Info& Info = HistoryRedo.Last();
		const Edit::Context& Context = GetContext(Info.Context);
		NxEn::Object* Target = Context.TryGet(Info.Target);
		if (!Target)
		{
			return;
		}

		RecordUndo(Target, Info.Context);
		Target->Clone(Info.Data);

		HistoryRedo.RemoveBack();
	}

	void EditSystem::RecordUndo(NxEn::Object* Target, NxFr::StringId Ctx)
	{
		const Edit::Context& Context = GetCtx(Ctx);
		if (HistoryUndo.GetCount() >= SettingHistoryCapacity->GetValue())
		{
			HistoryUndo.RemoveFront();
		}

		HistoryUndo.AppendBackConstruct(Context.Id, Target->GetId(), Target->Clone());
	}

	void EditSystem::RecordRedo(NxEn::Object* Target, NxFr::StringId Ctx)
	{
		const Edit::Context& Context = GetCtx(Ctx);
		if (HistoryRedo.GetCount() >= SettingHistoryCapacity->GetValue())
		{
			HistoryRedo.RemoveFront();
		}

		HistoryRedo.AppendBackConstruct(Context.Id, Target->GetId(), Target->Clone());
	}

	void EditSystem::OnInitialize()
	{
		NxEn::GUI::Menu* Menu = NxEn::GUISystem::GetMenu();
		Menu->AddMenuItem("Edit/Undo", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Undo");
		}, "", 0, [this]() { return !HistoryUndo.IsEmpty(); });
		Menu->AddMenuItem("Edit/Redo", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Redo");
		}, "", 1, [this]() { return !HistoryRedo.IsEmpty(); });
		Menu->AddMenuItem("Edit/Rename", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Rename");
		}, "", 2, [this]() { return !Selection.IsEmpty(); });
		Menu->AddMenuItem("Edit/Delete", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Delete");
		}, "", 3, [this]() { return !Selection.IsEmpty(); });
		Menu->AddMenuItem("Edit/Cut", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Cut");
		}, "", 4, [this]() { return !Selection.IsEmpty(); });
		Menu->AddMenuItem("Edit/Copy", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Copy");
		}, "", 5, [this]() { return !Selection.IsEmpty(); });
		Menu->AddMenuItem("Edit/Paste", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Paste");
		}, "", 6, [this]() { return !Selection.IsEmpty(); });
		Menu->AddMenuItem("Edit/Duplicate", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Duplicate");
		}, "", 7, [this]() { return !Selection.IsEmpty(); });
		Menu->AddMenuItem("Edit/Select All", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.SelectAll");
		}, "", 8);
		Menu->AddMenuItem("Edit/Unselect All", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.UnselectAll");
		}, "", 9, [this]() { return !Selection.IsEmpty(); });
		Menu->AddMenuItem("Edit/Invert Selection", []()
		{
			NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.InvertSelection");
		}, "", 10, [this]() { return !Selection.IsEmpty(); });
	}

	const Edit::Context& EditSystem::GetCtx(NxFr::StringId Ctx) const
	{
		return Ctx.GetId() == 0 ? GetCurrentContext() : GetContext(Ctx);
	}

	NxFr::List<Edit::Info>::I EditSystem::FindInfo(NxFr::StringId Ctx, NxEn::Object* Target) const
	{
		const Edit::Context& Context = GetCtx(Ctx);
		NxFr::GUID Id = Target->GetId();
		Edit::Info Info = Edit::Info(Context.Id, Id, nullptr);
		return Selection.Find(Info);
	}
}
