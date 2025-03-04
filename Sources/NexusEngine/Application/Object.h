#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

#define NEXUS_OBJECT_DECLARATION(Type)\
NEXUS_ENGINE_API virtual NxFr::StringId GetObjectType() const;\
NEXUS_ENGINE_API static NxFr::StringId GetClassType();

#define NEXUS_OBJECT_IMPLEMENTATION(Type) \
NxFr::StringId Type::GetObjectType() const \
{\
	return Type::GetClassType();\
}\
NxFr::StringId Type::GetClassType()\
{\
	static NxFr::StringId T = #Type##_Sid; \
	return T; \
}

namespace NxEn
{
	class Object
	{
	public:
		NEXUS_OBJECT_DECLARATION(Object)

		NEXUS_ENGINE_API Object();
		NEXUS_ENGINE_API virtual ~Object();

		NEXUS_ENGINE_API virtual bool IsEnabled() const;
		NEXUS_ENGINE_API virtual void SetEnabled(bool enabled);

		NEXUS_ENGINE_API virtual NxFr::String ToString() const;
		NEXUS_ENGINE_API virtual NxFr::StringView GetName() const;

	protected:
		NEXUS_ENGINE_API void Initialize();
		NEXUS_ENGINE_API void Shutdown();
		NEXUS_ENGINE_API void Tick();

		NEXUS_ENGINE_API virtual void OnInitialize() { };
		NEXUS_ENGINE_API virtual void OnShutdown() { };
		NEXUS_ENGINE_API virtual void OnTick() { };
		NEXUS_ENGINE_API virtual void OnEnable() { };
		NEXUS_ENGINE_API virtual void OnDisable() { };

	private:
		bool Enabled;
	};
}
