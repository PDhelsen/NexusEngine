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

		Flag_1 = 1 << 3,
		Flag_2 = 1 << 4,
		Flag_3 = 1 << 5,
		Flag_4 = 1 << 6,
		Flag_5 = 1 << 7,
	};
}

NEXUS_FLAG(NxEn::ObjectFlags, uint8)

namespace NxEn
{
	class Object
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Object)

		template<typename T>
		static T* Create(bool Enable = true)
		{
			T* Instance = new T();
			Instance->Initialize();
			Instance->SetEnabled(Enable);
			return Instance;
		}

		template<typename T>
		static T* Destroy(T* Instance)
		{
			if (Instance == nullptr)
			{
				return nullptr;
			}

			Instance->SetEnabled(false);
			Instance->Shutdown();
			delete Instance;
			return nullptr;
		}

		NEXUS_ENGINE_API Object();
		NEXUS_ENGINE_API virtual ~Object();

		NEXUS_ENGINE_API virtual void Initialize();
		NEXUS_ENGINE_API virtual void Shutdown();
		NEXUS_ENGINE_API virtual void Tick(float TimeStep = 0.0f);

		NEXUS_ENGINE_API virtual bool IsInitialized() const;
		NEXUS_ENGINE_API virtual bool IsEnabled() const;
		NEXUS_ENGINE_API virtual void SetEnabled(bool Enabled);
		NEXUS_ENGINE_API virtual bool IsTickable() const;
		NEXUS_ENGINE_API virtual void SetTickable(bool Tickable);

		NEXUS_ENGINE_API virtual Object* Clone() const;
		NEXUS_ENGINE_API virtual void Clone(Object* Target);
		NEXUS_ENGINE_API virtual void Clone(const Object* Target);

		NEXUS_ENGINE_API virtual NxFr::StringView GetName() const;
		NEXUS_ENGINE_API virtual NxFr::GUID GetId() const;

	protected:
		NEXUS_ENGINE_API virtual void OnInitialize() { };
		NEXUS_ENGINE_API virtual void OnShutdown() { };
		NEXUS_ENGINE_API virtual void OnEnable() { };
		NEXUS_ENGINE_API virtual void OnDisable() { };
		NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) { };

	protected:
		NEXUS_FORCE_INLINE bool GetFlag(ObjectFlags Flag) const;
		NEXUS_FORCE_INLINE void SetFlag(ObjectFlags Flag, bool Value);

	private:
		ObjectFlags Flags;
	};
}
