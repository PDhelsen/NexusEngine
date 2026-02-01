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
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, ViewerContext3D)

		NEXUS_EDITOR_API ViewerContext3D();
		NEXUS_EDITOR_API virtual ~ViewerContext3D();

		NEXUS_EDITOR_API NxEn::World* GetWorld() const { return World; }

	protected:
		NEXUS_EDITOR_API void Clear() override;
		NEXUS_EDITOR_API void SetupMenu(NxEn::GUI::Menu& Menu) override;
		NEXUS_EDITOR_API void SetupTarget(NxEn::Object* Instance) override;

		NEXUS_EDITOR_API virtual void OnGui(float TimeStep);

	private:
		ViewContextMode Mode;

		NxEn::World* World;
		NxFr::Handle<NxEn::GameObject> Target;
	};
}
