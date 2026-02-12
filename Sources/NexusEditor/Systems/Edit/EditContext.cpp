#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	namespace Edit
	{
		static Context* Current = nullptr;

		Context* Context::GetCurrent()
		{
			return Current;
		}

		void Context::SetCurrent(Context* Instance)
		{
			Current = Instance;
		}

		Context::Context(NxFr::StringId Id)
			: Id(Id), Selection(), Selected(0)
		{
		}

		Context::~Context()
		{
			OnSelectionChanged.Clear();
		}

		void Context::Select(NxFr::GUID InstanceId)
		{
			if (IsSelected(InstanceId))
			{
				return;
			}

			Selection.Append(InstanceId);
			Selected = InstanceId;

			if (OnSelectionChanged)
			{
				OnSelectionChanged.Invoke(InstanceId, true);
			}
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

			if (OnSelectionChanged)
			{
				OnSelectionChanged.Invoke(InstanceId, false);
			}
		}

		void Context::Invert(NxFr::GUID InstanceId)
		{
			if (!IsSelected(InstanceId))
			{
				Select(InstanceId);
			}
			else
			{
				Unselect(InstanceId);
			}
		}

		bool Context::IsSelected(NxFr::GUID InstanceId) const
		{
			return Selection.Contains(InstanceId);
		}

		NxFr::GUID Context::GetSelected() const
		{
			return Selected;
		}

		NxFr::Array<NxFr::GUID> Context::GetSelection() const
		{
			return NxFr::ContainersUtils::ToArray<NxFr::GUID>(Selection);
		}

		uint64 Context::GetSelectionCount() const
		{
			return Selection.GetCount();
		}

		void Context::Rename()
		{
		}

		void Context::Duplicate()
		{
		}

		void Context::Delete()
		{
		}

		void Context::Cut()
		{
		}

		void Context::Copy()
		{
		}

		void Context::Paste()
		{
		}
	}
}


