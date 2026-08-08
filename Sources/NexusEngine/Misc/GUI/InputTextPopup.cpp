#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEn
{
	void InputTextPopup::RegisterCallback(const NxFr::Delegate<void(NxFr::StringView)>& Callback)
	{
		this->Callback = Callback;
	}

	void InputTextPopup::UnregisterCallback()
	{
		this->Callback = nullptr;
	}

	void InputTextPopup::OnInitialize()
	{
		Popup::OnInitialize();
		Input.Reserve(128);

		SetNameId("Input Text");
		SetMessage("Enter text:");

		AddButton("Cancel");
	}

	void InputTextPopup::OnShutdown()
	{
		UnregisterCallback();
		Input.Clear();

		Popup::OnShutdown();
	}

	void InputTextPopup::OnDraw()
	{
		GUI::Transform Visual = GUI::Transform(-NxFr::Vector2f::One, -NxFr::Vector2f::One, 0);
		if (GUI::Draw::Input(Input, GUI::Utils::ImGuiIdSeparator(), Visual))
		{
			Callback(Input);
			Close();
		}
	}
}
