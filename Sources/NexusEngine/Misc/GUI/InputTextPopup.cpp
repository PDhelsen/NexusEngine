#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEn
{
	InputTextPopup* InputTextPopup::GetInstance()
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		static InputTextPopup Instance;
		Instance.Initialize();

		Instance.UnregisterCallback();
		Instance.Input.Clear();
		Instance.Show();
		return &Instance;
	}

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
		SetTitle("Input Text");
		SetMessage("Enter text:");
		AddButton("Cancel");
	}

	void InputTextPopup::OnShutdown()
	{
		UnregisterCallback();
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
