#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Misc/Object/ObjectInstance.h"
#include "NexusEngine/Systems/GUI/Draw.h"

namespace NxEn
{
	namespace ObjectId
	{
		template<typename T, typename = void>
		struct ObjectIdResolver
		{
			static ObjectInstance<T> Resolve(NxFr::GUID Id)
			{
				return ObjectInstance<T>();
			}
		};

		template<typename T>
		NxFr::GUID GetId(ObjectInstance<T> Instance)
		{
			return Instance ? Instance->GetId() : Object::NullId;
		}

		template<typename T>
		ObjectInstance<T> Resolve(NxFr::GUID Id)
		{
			return ObjectIdResolver<T>::Resolve(Id);
		}

		template<typename T, typename P>
		void Assign(P* Target, ObjectInstance<T> Instance)
		{
			if constexpr (NxFr::IsHandle<P>::Value)
			{
				NX_ASSERT_RETURN(Instance.IsHandle(), , Default, "Instance mode doesn't match the expected one");
				*Target = Instance.AsHandle();
			}
			else
			{
				NX_ASSERT_RETURN(Instance.IsPointer(), , Default, "Instance mode doesn't match the expected one");
				*Target = Instance.AsPointer();
			}
		}

		template<typename T, typename P>
		void Assign(P* Target, NxFr::GUID Id)
		{
			Assign<T, P>(Target, ObjectId::Resolve<T>(Id));
		}

		template<typename T>
		ObjectInstance<T> Clone(ObjectInstance<T> Instance)
		{
			return Resolve<T>(GetId<T>(Instance));
		}

		template<typename T>
		NxFr::Yaml::Node Serialize(ObjectInstance<T> Instance)
		{
			NxFr::Yaml::Node Node;
			Node = GetId<T>(Instance);
			return Node;
		}

		template<typename T>
		ObjectInstance<T> Deserialize(NxFr::Yaml::Node Node)
		{
			return Resolve<T>(Node.As<NxFr::GUID>());
		}
	};

	namespace GUI
	{
		namespace Draw
		{
			template<typename T, typename P>
			void PropertyObject(P Target, NxFr::StringView Label, const Transform& Visual = {})
			{
				ObjectInstance<T> Instance = Target;

				NxFr::GUID Id = ObjectId::GetId<T>(Instance);
				NxFr::StringView Name = Instance ? Instance->GetName() : (NxFr::StringView)NxFr::StringUtility::Empty;

				Transform ItemVisual = Visual;
				ItemVisual.Size.x = Styles::WidthLabel();

				Drawer<NxFr::GUID>::Property(Id, Label, ItemVisual);
				Utils::SameLine();
				Text(Name);
			}

			template<typename T, typename P>
			bool FieldObject(P* Target, NxFr::StringView Label, const Transform& Visual = {})
			{
				ObjectInstance<T> Instance = *Target;

				NxFr::GUID Id = ObjectId::GetId<T>(Instance);
				NxFr::StringView Name = Instance ? Instance->GetName() : (NxFr::StringView)NxFr::StringUtility::Empty;
				NxFr::String Buffer = NxFr::StringUtility::ToString(Id);
				NxFr::String ImGuiId = Utils::GenerateStringId(Buffer, Label);

				Transform ItemVisual = Visual;
				ItemVisual.Size.x = Styles::WidthLabel();

				bool Result = Drawer<NxFr::GUID>::Field(Id, Label, ImGuiId, ItemVisual);
				Utils::SameLine();
				Text(Name);

				if (Result)
				{
					ObjectId::Assign<T, P>(Target,Id);
				}

				return Result;
			}
		}
	}
}
