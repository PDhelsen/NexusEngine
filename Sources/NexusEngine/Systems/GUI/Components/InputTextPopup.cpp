#include "NexusEngine/Core/NexusEnginePch.h"
#include "InputTextPopup.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(InputTextPopup)

	InputTextPopup* InputTextPopup::GetInstance()
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));
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
		SetTitle("Input Text");
	}

	void InputTextPopup::OnGui(float TimeStep)
	{
		if (ImGui::InputText("##", Input.Characters(), Input.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			Input.Validate();
			Callback(Input);

			Hide();
		}
	}
}
