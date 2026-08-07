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
		Popup::OnShutdown();
	}

	void InputTextPopup::OnEnable()
	{
		Popup::OnEnable();
		Style.Reset();
	}

	void InputTextPopup::OnDisable()
	{
		Popup::OnDisable();
		UnregisterCallback();
		Input.Clear();
	}

	void InputTextPopup::OnDraw()
	{
		GUI::Utils::SetSize(-NxFr::Vector2f::One);
		if (GUI::Draw::Input(Input, GUI::Utils::ImGuiIdSeparator()))
		{
			Callback(Input);
			Hide();
		}
	}
}
