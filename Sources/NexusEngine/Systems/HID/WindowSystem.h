#pragma once

#include "NexusEngine/Systems/System.h"

namespace NxEn
{
	class WindowSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, WindowSystem)

		NxFr::Event<> OnClose;
		NxFr::Event<bool> OnFocus;
		NxFr::Event<NxFr::Vector2i> OnMove;
		NxFr::Event<NxFr::Vector2i> OnResize;

		NEXUS_ENGINE_API WindowSystem();

		NEXUS_ENGINE_API void Close();
		NEXUS_ENGINE_API void Minimize();
		NEXUS_ENGINE_API void Maximize();
		NEXUS_ENGINE_API void Restore();
		NEXUS_ENGINE_API void Show();
		NEXUS_ENGINE_API void Hide();
		NEXUS_ENGINE_API void Focus();

		NEXUS_ENGINE_API void SetPosition(NxFr::Vector2i Position);
		NEXUS_ENGINE_API void SetResolution(NxFr::Vector2i Size);
		NEXUS_ENGINE_API void SetTitle(NxFr::StringView Title);
		NEXUS_ENGINE_API void SetIcon(void* Icon);
		NEXUS_ENGINE_API void SetVSync(bool VSync);

		NxFr::Vector2i GetPosition() { return Position; }
		NxFr::Vector2i GetResolution() { return Resolution; }
		NxFr::StringView GetTitle() { return Title; }
		const void* GetIcon() { return Icon; }
		bool GetVSync() { return VSync; }
		bool IsFocused() { return Focused; }

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_ENGINE_API void OnFocused(bool Focus);
		NEXUS_ENGINE_API void OnMoved(NxFr::Vector2i Position);
		NEXUS_ENGINE_API void OnResized(NxFr::Vector2i Size);

	private:
		NxFr::Vector2i Position;
		NxFr::Vector2i Resolution;
		NxFr::String Title;
		void* Icon;
		bool VSync;
		bool Focused;

		void* Window;
	};
}
