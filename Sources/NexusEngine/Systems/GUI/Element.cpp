#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Element.h"

namespace NxEn
{
	namespace GUI
	{
		Element::Element()
			: Id(0), Name(), NamedId(),
			GuiFlags(ElementFlags::None), ImGuiFlags(0)
		{
		}

		Element::~Element()
		{
		}

		void Element::Show()
		{
			SetEnabled(true);
		}

		void Element::Hide()
		{
			SetEnabled(false);
		}

		void Element::Close()
		{
			if (GetGuiFlag(ElementFlags::HideInsteadOfClose))
			{
				Hide();
				return;
			}

			if (GetGuiFlag(ElementFlags::RecycleInsteadOfClose))
			{
				Application::GetSystem<GUISystem>()->RecycleElement(this);
				return;
			}

			if (GetGuiFlag(ElementFlags::WillClose))
			{
				return;
			}

			Application::GetSystem<GUISystem>()->DestroyElement(this);
			SetGuiFlag(ElementFlags::WillClose, true);
		}

		void Element::SetNameId(NxFr::StringView Name, NxFr::GUID Id)
		{
			if (Id == 0)
			{
				Id = GetObjectType();
			}

			NamedId = Utils::NexusToImGuiId(Name, NxFr::StringUtility::ToString(Id));
			this->Name = Utils::ImGuiToNexusName(NamedId);
			this->Id = Id;
		}

		bool Element::GetGuiFlag(ElementFlags Flag) const
		{
			return NxFr::Enum::CheckFlag(GuiFlags, Flag);
		}

		void Element::SetGuiFlag(ElementFlags Flag, bool Value)
		{
			GuiFlags = NxFr::Enum::SetFlag(GuiFlags, Flag, Value);
		}

		bool Element::GetImGuiFlag(uint64 Flag) const
		{
			return NxFr::Integer::CheckFlag(ImGuiFlags, Flag);
		}

		void Element::SetImGuiFlag(uint64 Flag, bool Value)
		{
			ImGuiFlags = NxFr::Integer::SetFlag(ImGuiFlags, Flag, Value);
		}

		uint64 Element::GetImGuiFlags() const
		{
			return ImGuiFlags;
		}

		void Element::SetImGuiFlags(uint64 Flags)
		{
			ImGuiFlags = Flags;
		}

		void Element::OnInitialize()
		{
			SetGuiFlag(ElementFlags::AutoDraw, true);
			SetNameId(GetObjectType());
		}

		void Element::OnEnable()
		{
			if (!IsAutoDraw())
			{
				return;
			}

			Application::GetSystem<GUISystem>()->DrawElement(this, true);
		}

		void Element::OnDisable()
		{
			Application::GetSystem<GUISystem>()->DrawElement(this, false);
		}

		GUI::Element* Element::Acquire(NxFr::StringId Type)
		{
			return Application::GetSystem<GUISystem>()->AcquireElement(Type);
		}
	}
}
