#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(InputTextPopup)

	InputTextPopup* InputTextPopup::GetInstance()
	{
		NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));
		static InputTextPopup* Instance = Object::Create<InputTextPopup>();

		Instance->UnregisterCallback();
		Instance->Input.Clear();
		Instance->Show();
		return Instance;
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

	void InputTextPopup::OnEnable()
	{
		Popup::OnEnable();
		Style.Reset();
	}

	void InputTextPopup::OnGui(float TimeStep)
	{
		if (GUI::Draw::Input(Input, "##", &Style))
		{
			Callback(Input);
			Hide();
		}
	}
}
