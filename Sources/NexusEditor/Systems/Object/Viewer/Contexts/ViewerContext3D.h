#pragma once

#include "NexusEditor/Systems/Object/Viewer/ViewerContext.h"

namespace NxEd
{
	class ViewerContext3D : public ViewerContext
	{
		enum class ViewContextMode
		{
			Invalid, World, Prefab
		};

	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, ViewerContext3D)

		NX_EDITOR_API ViewerContext3D();
		NX_EDITOR_API virtual ~ViewerContext3D();

		NX_EDITOR_API NxEn::World* GetWorld() const override { return World; }

	protected:
		NX_EDITOR_API void Clear() override;
		NX_EDITOR_API void SetupMenu(NxEn::GUI::Menu& Menu) override;
		NX_EDITOR_API void SetupTarget(NxEn::Object* Instance) override;

		NX_EDITOR_API void OnGui(float TimeStep) override;

	private:
		ViewContextMode Mode;

		NxEn::World* World;
		NxFr::Handle<NxEn::GameObject> Target;
	};
}
