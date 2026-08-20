#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	namespace Edit
	{
		Context::Context(NxFr::StringId Id)
			: Id(Id), Clipboard(), Selection(), Selected(0), IsCutting(false)
		{
		}

		Context::~Context()
		{
		}

		void Context::Cut()
		{
			Clipboard.Clear();
			Clipboard.AppendRange(GetSelection(true));
			IsCutting = true;
		}

		void Context::Copy()
		{
			Clipboard.Clear();
			Clipboard.AppendRange(GetSelection(true));
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

			OnSelectionChanged(InstanceId, true);
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

			OnSelectionChanged(InstanceId, false);
		}

		bool Context::IsSelected(NxFr::GUID InstanceId) const
		{
			return Selection.TryGet(InstanceId);
		}

		NxFr::GUID Context::GetSelected() const
		{
			return Selected;
		}

		NxFr::Array<NxFr::GUID> Context::GetSelection(bool Filtered) const
		{
			return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Selection);
		}

		uint64 Context::GetSelectionCount(bool Filtered) const
		{
			return Selection.GetCount();
		}
	}
}
