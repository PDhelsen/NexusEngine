#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

#define NEXUS_OBJECT_DECLARATION(Dll, Type)\
Dll virtual NxFr::StringId GetObjectType() const;\
Dll static NxFr::StringId GetClassType();

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
	enum class ObjectFlags : uint8
	{
		None = 0,

		Initialized = 1 << 0,
		Enabled = 1 << 1,
		Tickable = 1 << 2,
	};
	NEXUS_ENUM_TO_FLAG(ObjectFlags)

	class Object
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Object)

		template<typename T>
		static T* Create(bool Enable = true)
		{
			T* Instance = new T();
			Instance->Initialize(Enable);
			return Instance;
		}

		template<typename T>
		static void Destroy(T** Instance)
		{
			(*Instance)->Shutdown();
			delete *Instance;
			*Instance = nullptr;
		}

		NEXUS_ENGINE_API Object();
		NEXUS_ENGINE_API virtual ~Object();

		NEXUS_ENGINE_API void Initialize(bool Enable = true);
		NEXUS_ENGINE_API void Shutdown();
		NEXUS_ENGINE_API void Tick(float TimeStep = 0.0f);

		NEXUS_ENGINE_API bool IsInitialized() const;
		NEXUS_ENGINE_API bool IsEnabled() const;
		NEXUS_ENGINE_API void SetEnabled(bool Enabled);
		NEXUS_ENGINE_API bool IsTickable() const;
		NEXUS_ENGINE_API void SetTickable(bool Tickable);

		NEXUS_ENGINE_API virtual NxFr::String ToString() const;
		NEXUS_ENGINE_API virtual NxFr::StringView GetName() const;

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize() { };
		NEXUS_ENGINE_API virtual void OnShutdown() { };
		NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) { };
		NEXUS_ENGINE_API virtual void OnEnable() { };
		NEXUS_ENGINE_API virtual void OnDisable() { };

	private:
		NEXUS_FORCE_INLINE bool GetFlag(ObjectFlags Flag) const;
		NEXUS_FORCE_INLINE void SetFlag(ObjectFlags Flag, bool Value);

	private:
		ObjectFlags Flags;
	};
}
