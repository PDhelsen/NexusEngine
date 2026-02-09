#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserEditContext.h"

namespace NxEd
{
	const static NxEn::Command CmdEditSelectAll = NxEn::Command::Create("Edit.SelectAll"_Sid, "Select all object in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Select();
	}));
	const static NxEn::Command CmdEditUnselectAll = NxEn::Command::Create("Edit.UnselectAll"_Sid, "Unselect all object in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->Unselect();
	}));
	const static NxEn::Command CmdEditInvert = NxEn::Command::Create("Edit.Invert"_Sid, "Invert selection in the current context", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<EditSystem>()->InvertSelection();
	}));

	const static NxEn::GUI::Menu::Item MenuItemEditSelectAll = NxEn::GUI::Menu::Item::Create("Edit/Selection/SelectAll", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.SelectAll");
	}));
	const static NxEn::GUI::Menu::Item MenuItemEditUnselectAll = NxEn::GUI::Menu::Item::Create("Edit/Selection/UnselectAll", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.UnselectAll");
	}));
	const static NxEn::GUI::Menu::Item MenuItemEditInvert = NxEn::GUI::Menu::Item::Create("Edit/Selection/Invert", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("Edit.Invert");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(EditSystem)

	void EditSystem::RegisterContext(NxFr::StringId Id, Edit::Context* Ctx)
	{
		if (Contexts.ContainsKey(Id))
		{
			NEXUS_LOG(Error, System, "Context %s already exist", Id.GetString().C());
		}

		Contexts.Append(Id, Ctx);
	}

	Edit::Context* EditSystem::UnregisterContext(NxFr::StringId Id)
	{
		if (!Contexts.ContainsKey(Id))
		{
			NEXUS_LOG(Error, System, "Context %s doesn't exist", Id.GetString().C());
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

		for (auto& It = InstancesId.Reset(); It != InstancesId.End(); ++It)
		{
			Ctx->Select(*It);
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

		for (auto& It = InstancesId.Reset(); It != InstancesId.End(); ++It)
		{
			Ctx->Unselect(*It);
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

		for (auto& It = InstancesId.Reset(); It != InstancesId.End(); ++It)
		{
			Ctx->Invert(*It);
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
}
