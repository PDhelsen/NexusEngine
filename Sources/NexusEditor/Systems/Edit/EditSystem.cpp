#include "NexusEditor/Systems/Edit/EditSystem.h"

namespace NxEd
{
	static const NxEn::GUI::Menu::Item* MenuItemEditCreate = NxEn::GUI::Menu::Create("Edit/Clipboard/Create", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Create");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditRename = NxEn::GUI::Menu::Create("Edit/Clipboard/Rename", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Rename");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditMove = NxEn::GUI::Menu::Create("Edit/Clipboard/Move", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Move");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditDuplicate = NxEn::GUI::Menu::Create("Edit/Clipboard/Duplicate", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Duplicate");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditDelete = NxEn::GUI::Menu::Create("Edit/Clipboard/Delete", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Delete");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditCut = NxEn::GUI::Menu::Create("Edit/Clipboard/Cut", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Cut");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditCopy = NxEn::GUI::Menu::Create("Edit/Clipboard/Copy", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Copy");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditPaste = NxEn::GUI::Menu::Create("Edit/Clipboard/Paste", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Paste");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditSelectAll = NxEn::GUI::Menu::Create("Edit/Selection/SelectAll", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.SelectAll");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditUnselectAll = NxEn::GUI::Menu::Create("Edit/Selection/UnselectAll", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.UnselectAll");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditInvert = NxEn::GUI::Menu::Create("Edit/Selection/InvertSelection", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.InvertSelection");
	}));

	static NxEn::Command* CmdEditCreate = NxEn::Command::Create("Edit.Create"_Sid, "Create object in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Create();
	}));
	static NxEn::Command* CmdEditRename = NxEn::Command::Create("Edit.Rename"_Sid, "Rename selected objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Rename();
	}));
	static NxEn::Command* CmdEditMove = NxEn::Command::Create("Edit.Move"_Sid, "Move selected objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Move();
	}));
	static NxEn::Command* CmdEditDuplicate = NxEn::Command::Create("Edit.Duplicate"_Sid, "Duplicate selected objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Duplicate();
	}));
	static NxEn::Command* CmdEditDelete = NxEn::Command::Create("Edit.Delete"_Sid, "Delete selected objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Delete();
	}));
	static NxEn::Command* CmdEditCut = NxEn::Command::Create("Edit.Cut"_Sid, "Rename selected objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Cut();
	}));
	static NxEn::Command* CmdEditCopy = NxEn::Command::Create("Edit.Copy"_Sid, "Duplicate selected objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Copy();
	}));
	static NxEn::Command* CmdEditPaste = NxEn::Command::Create("Edit.Paste"_Sid, "Delete selected objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Paste();
	}));
	static NxEn::Command* CmdEditSelectAll = NxEn::Command::Create("Edit.SelectAll"_Sid, "Select all objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Select();
	}));
	static NxEn::Command* CmdEditUnselectAll = NxEn::Command::Create("Edit.UnselectAll"_Sid, "Unselect all objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Unselect();
	}));
	static NxEn::Command* CmdEditInvertSelection = NxEn::Command::Create("Edit.InvertSelection"_Sid, "Invert selection in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->InvertSelection();
	}));

	void EditSystem::RegisterContext(Edit::Context* Ctx)
	{
		if (Contexts.TryGet(Ctx->GetId()))
		{
			NX_LOG(Error, System, "Context %s already exist", Ctx->GetId().GetString().C());
			return;
		}

		Contexts.Append(Ctx->GetId(), Ctx);
	}

	Edit::Context* EditSystem::UnregisterContext(NxFr::StringId Id)
	{
		if (!Contexts.TryGet(Id))
		{
			NX_LOG(Error, System, "Context %s doesn't exist", Id.GetString().C());
			return nullptr;
		}

		Edit::Context* Ctx = Contexts[Id];
		Contexts.Remove(Id);
		return Ctx;
	}

	void EditSystem::SetContext(NxFr::StringId Id)
	{
		Context = Id;
	}

	Edit::Context* EditSystem::GetContext(NxFr::StringId Id) const
	{
		Id = Id.IsValid() ? Id : Context;
		auto Instance = Contexts.TryGet(Id);
		return Instance ? *Instance : nullptr;
	}

	NxFr::Array<NxFr::GUID> EditSystem::GetAll(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return NxFr::Array<NxFr::GUID>();
		}

		return Ctx->GetAll();
	}

	uint64 EditSystem::GetCount(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return 0;
		}

		return Ctx->GetCount();
	}

	void EditSystem::Clear(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Clear();
	}

	void EditSystem::Create(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Create();
	}

	void EditSystem::Rename(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Rename();
	}

	void EditSystem::Move(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Move();
	}

	void EditSystem::Duplicate(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Duplicate();
	}

	void EditSystem::Delete(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Delete();
	}
	void EditSystem::Cut(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Cut();
	}

	void EditSystem::Copy(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Copy();
	}

	void EditSystem::Paste(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Paste();
	}

	void EditSystem::Select(NxFr::GUID InstanceId, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Select(InstanceId);
	}

	void EditSystem::Select(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		for (auto& It : InstancesId)
		{
			Ctx->Select(It);
		}
	}

	void EditSystem::Select(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		NxFr::Array<NxFr::GUID> InstancesId = Ctx->GetAll();
		for (auto It = InstancesId.Begin(); It != InstancesId.End(); ++It)
		{
			Ctx->Select(*It);
		}
	}

	void EditSystem::Unselect(NxFr::GUID InstanceId, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Unselect(InstanceId);
	}

	void EditSystem::Unselect(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		for (auto& It : InstancesId)
		{
			Ctx->Unselect(It);
		}
	}

	void EditSystem::Unselect(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		NxFr::Array<NxFr::GUID> InstancesId = Ctx->GetAll();
		for (auto It = InstancesId.Begin(); It != InstancesId.End(); ++It)
		{
			Ctx->Unselect(*It);
		}
	}

	bool EditSystem::IsSelected(NxFr::GUID InstanceId, NxFr::StringId ContextId) const
	{
		const Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return false;
		}

		return Ctx->IsSelected(InstanceId);
	}

	NxFr::GUID EditSystem::GetSelected(NxFr::StringId ContextId) const
	{
		const Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return 0;
		}

		return Ctx->GetSelected();
	}

	NxFr::Array<NxFr::GUID> EditSystem::GetSelection(NxFr::StringId ContextId) const
	{
		const Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return NxFr::Array<NxFr::GUID>();
		}

		return Ctx->GetSelection();
	}

	uint64 EditSystem::SelectionCount(NxFr::StringId ContextId) const
	{
		const Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return 0;
		}

		return Ctx->GetSelectionCount();
	}

	void EditSystem::SetSelected(NxFr::GUID InstanceId, bool State, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		if (State)
		{
			Ctx->Select(InstanceId);
		}
		else
		{
			Ctx->Unselect(InstanceId);
		}
	}

	void EditSystem::SetSelected(NxFr::Collection<NxFr::GUID> InstancesId, bool State, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		for (auto& It : InstancesId)
		{
			if (State)
			{
				Ctx->Select(It);
			}
			else
			{
				Ctx->Unselect(It);
			}
		}
	}

	void EditSystem::SetSelected(bool State, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		NxFr::Array<NxFr::GUID> InstancesId = Ctx->GetAll();
		for (auto It = InstancesId.Begin(); It != InstancesId.End(); ++It)
		{
			if (State)
			{
				Ctx->Select(*It);
			}
			else
			{
				Ctx->Unselect(*It);
			}
		}
	}

	void EditSystem::SetSelection(NxFr::GUID InstanceId, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		NxFr::Array<NxFr::GUID> SelectionIds = Ctx->GetSelection();
		for (auto It = SelectionIds.Begin(); It != SelectionIds.End(); ++It)
		{
			Ctx->Unselect(*It);
		}

		Ctx->Select(InstanceId);
	}

	void EditSystem::SetSelection(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		NxFr::Array<NxFr::GUID> SelectionIds = Ctx->GetSelection();
		for (auto It = SelectionIds.Begin(); It != SelectionIds.End(); ++It)
		{
			Ctx->Unselect(*It);
		}

		for (auto& It : InstancesId)
		{
			Ctx->Select(It);
		}
	}

	void EditSystem::SetSelection(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		NxFr::Array<NxFr::GUID> InstancesId = Ctx->GetAll();
		for (auto It = InstancesId.Begin(); It != InstancesId.End(); ++It)
		{
			Ctx->Select(*It);
		}
	}

	void EditSystem::InvertSelection(NxFr::GUID InstanceId, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		if (!Ctx->IsSelected(InstanceId))
		{
			Ctx->Select(InstanceId);
		}
		else
		{
			Ctx->Unselect(InstanceId);
		}
	}

	void EditSystem::InvertSelection(NxFr::Collection<NxFr::GUID> InstancesId, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		for (auto& It : InstancesId)
		{
			if (!Ctx->IsSelected(It))
			{
				Ctx->Select(It);
			}
			else
			{
				Ctx->Unselect(It);
			}
		}
	}

	void EditSystem::InvertSelection(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		NxFr::Array<NxFr::GUID> InstancesId = Ctx->GetAll();
		for (auto It = InstancesId.Begin(); It != InstancesId.End(); ++It)
		{
			if (!Ctx->IsSelected(*It))
			{
				Ctx->Select(*It);
			}
			else
			{
				Ctx->Unselect(*It);
			}
		}
	}
}
