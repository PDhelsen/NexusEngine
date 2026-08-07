#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"

namespace NxEd
{
	static NxEn::Command* CmdEditSelectAll = NxEn::Command::Create("Edit.SelectAll"_Sid, "Select all objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Select();
	}));
	static NxEn::Command* CmdEditUnselectAll = NxEn::Command::Create("Edit.UnselectAll"_Sid, "Unselect all objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Unselect();
	}));
	static NxEn::Command* CmdEditInvert = NxEn::Command::Create("Edit.Invert"_Sid, "Invert selection in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->InvertSelection();
	}));
	static NxEn::Command* CmdEditRename = NxEn::Command::Create("Edit.Rename"_Sid, "Rename selected objects in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Rename();
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

	static const NxEn::GUI::Menu::Item* MenuItemEditSelectAll = NxEn::GUI::Menu::Create("Edit/Selection/SelectAll", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.SelectAll");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditUnselectAll = NxEn::GUI::Menu::Create("Edit/Selection/UnselectAll", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.UnselectAll");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditInvert = NxEn::GUI::Menu::Create("Edit/Selection/Invert", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Invert");
	}));
	static const NxEn::GUI::Menu::Item* MenuItemEditRename = NxEn::GUI::Menu::Create("Edit/Clipboard/Rename", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Rename");
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

	void EditSystem::RegisterContext(NxFr::StringId Id, Edit::Context* Ctx)
	{
		if (Contexts.TryGet(Id))
		{
			NX_LOG(Error, System, "Context %s already exist", Id.GetString().C());
		}

		Contexts.Append(Id, Ctx);
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

	Edit::Context* EditSystem::GetContext(NxFr::StringId Id) const
	{
		Edit::Context* Current = Edit::Context::GetCurrent();
		Id = Id.GetId() == 0 && Current ? Current->Id : Id;
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

	void EditSystem::InvertSelection(NxFr::GUID InstanceId, NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Invert(InstanceId);
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
			Ctx->Invert(It);
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
			Ctx->Invert(*It);
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

	void EditSystem::Rename(NxFr::StringId ContextId)
	{
		Edit::Context* Ctx = GetContext(ContextId);
		if (!Ctx)
		{
			return;
		}

		Ctx->Rename();
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
}
