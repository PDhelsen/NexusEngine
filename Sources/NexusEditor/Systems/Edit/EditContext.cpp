#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	namespace Edit
	{
		Context::Context(NxFr::StringId Id)
			: Id(Id), Clipboard(), Selection(), Selected(0), IsCutting(false)
		{
			OnDestroyed += [this](NxFr::GUID InstanceId)
			{
				Clipboard.TryRemove(InstanceId);
				Selection.TryRemove(InstanceId);
				if (Selected == InstanceId) Selected = 0;
			};
		}

		Context::~Context()
		{
			OnCreated.Clear();
			OnDestroyed.Clear();
			OnSelection.Clear();
		}

		void Context::Clear()
		{
			Clipboard.Clear();
			Selection.Clear();
			Selected = 0;
			IsCutting = false;
		}

		void Context::Cut()
		{
			Clipboard.Clear();
			Clipboard.AppendRange(GetSelection());
			IsCutting = true;
		}

		void Context::Copy()
		{
			Clipboard.Clear();
			Clipboard.AppendRange(GetSelection());
			IsCutting = false;
		}

		void Context::Paste()
		{
			if (IsCutting)
			{
				Clipboard.Clear();
				IsCutting = false;
			}
		}

		void Context::Select(NxFr::GUID InstanceId)
		{
			if (IsSelected(InstanceId))
			{
				return;
			}

			Selection.Append(InstanceId);
			Selected = InstanceId;

			OnSelection.Invoke(InstanceId, true);
		}

		void Context::Unselect(NxFr::GUID InstanceId)
		{
			if (!IsSelected(InstanceId))
			{
				return;
			}

			Selection.Remove(InstanceId);
			if (Selected == InstanceId)
			{
				Selected = 0;
			}

			OnSelection.Invoke(InstanceId, false);
		}

		bool Context::IsSelected(NxFr::GUID InstanceId) const
		{
			return Selection.TryGet(InstanceId);
		}

		NxFr::GUID Context::GetSelected() const
		{
			return Selected;
		}

		NxFr::Array<NxFr::GUID> Context::GetSelection() const
		{
			return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Selection);
		}

		uint64 Context::GetSelectionCount() const
		{
			return Selection.GetCount();
		}
	}
}
