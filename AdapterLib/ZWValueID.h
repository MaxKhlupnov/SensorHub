//-----------------------------------------------------------------------------
//
//      ZWValueID.h
//
//      Cli/C++ wrapper for the C++ OpenZWave Manager class
//
//      Copyright (c) 2010 Amer Harb <harb_amer@hotmail.com>
//
//      SOFTWARE NOTICE AND LICENSE
//
//      This file is part of OpenZWave.
//
//      OpenZWave is free software: you can redistribute it and/or modify
//      it under the terms of the GNU Lesser General Public License as published
//      by the Free Software Foundation, either version 3 of the License,
//      or (at your option) any later version.
//
//      OpenZWave is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU Lesser General Public License for more details.
//
//      You should have received a copy of the GNU Lesser General Public License
//      along with OpenZWave.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------


#pragma once
#include "AdapterConfig.h"
#include "AdapterUtils.h"

#include "Manager.h"
#include "ValueID.h"
#include "Windows.h"

using namespace OpenZWave;
using namespace Windows::System;

namespace AdapterLib 
{

	public enum class ValueGenre
	{
		Basic = ValueID::ValueGenre_Basic,
		User = ValueID::ValueGenre_User,
		Config = ValueID::ValueGenre_Config,
		System = ValueID::ValueGenre_System
	};

	public enum class ValueType
	{
		Bool = ValueID::ValueType_Bool,
		Byte = ValueID::ValueType_Byte,
		Decimal = ValueID::ValueType_Decimal,
		Int = ValueID::ValueType_Int,
		List = ValueID::ValueType_List,
		Schedule = ValueID::ValueType_Schedule,
		Short = ValueID::ValueType_Short,
		String = ValueID::ValueType_String,
		Button = ValueID::ValueType_Button,
		Raw = ValueID::ValueType_Raw
	};

	public ref class ZWValueID sealed
	{
	

	public: 
		virtual ~ZWValueID()
		{ 
			//delete m_valueId;
		}

		property uint32 HomeId;

		property uint8 NodeId;

		property ValueGenre	Genre;
			
		property uint8	CommandClassId;
		property uint8	Instance;
		property uint8	Index;
		property ValueType	Type;
		property uint64		Id;

		property Platform::Object^ Value;
		property Platform::String^ ValueLabel;
		property Platform::String^ ValueHelp;
		property Platform::String^ ValueUnits;

	internal:
		

		ZWValueID(ValueID const& valueId)
		{
			HomeId = valueId.GetHomeId();
			NodeId = valueId.GetNodeId();
			Genre = static_cast<ValueGenre>(valueId.GetGenre());
			CommandClassId = valueId.GetCommandClassId();
			Instance = valueId.GetInstance();
			Index = valueId.GetIndex();
			Type = static_cast<ValueType>(valueId.GetType());
			Id = valueId.GetId();
			Value = GetValue(valueId);
			ValueLabel = ToPlatformString(Manager::Get()->GetValueLabel(valueId));
			ValueHelp = ToPlatformString(Manager::Get()->GetValueHelp(valueId));
			ValueUnits = ToPlatformString(Manager::Get()->GetValueUnits(valueId));
		}

	private:
		
		/// <summary>
		/// Gets the value.
		/// </summary>
		/// <param name="v">The v.</param>
		/// <returns></returns>
		Platform::Object^  ZWValueID::GetValue(ValueID const& valueId)
		{
			try {
				switch (valueId.GetType())
				{
				case ValueID::ValueType::ValueType_String:
				{
					std::string* s_value = new std::string();
					if (Manager::Get()->GetValueAsString(valueId, s_value)) {		
						std::wstring* w_value = new std::wstring(s_value->begin(), s_value->end());
						return ref new Platform::String(w_value->c_str());
					}
					break;
				}
				case ValueID::ValueType::ValueType_Byte:
				{
					uint8 i_value = 0;
					if (Manager::Get()->GetValueAsByte(valueId, & i_value)) {
						//Platform::ValueType b_value = static_cast<bool>(i_value > 0);
						return i_value;
					}
					break;
				}
				case ValueID::ValueType::ValueType_Bool:
				{
					bool b_value = false;
					if (Manager::Get()->GetValueAsBool(valueId, & b_value)) {
						return b_value;
					}
					break;
				}
				case ValueID::ValueType::ValueType_Short: {
					short st_value = 0;
					if (Manager::Get()->GetValueAsShort(valueId, & st_value)) {
						return st_value;
					}
					break;
				}
				case ValueID::ValueType::ValueType_Int: {
					int i_value = 0;
					if (Manager::Get()->GetValueAsInt(valueId, & i_value)) {
						return i_value;
					}
					break;
				}
				case ValueID::ValueType::ValueType_Decimal: {
					
					float f_value = 0.0;
					
					if (Manager::Get()->GetValueAsFloat(valueId, & f_value)) {	
						
						return float(f_value);
					}
					break;
				}
				default:
					break;
				}
			}
			catch (OZWException* err) {
				// TODO: add error handling
				return	ToPlatformString(err->GetMsg());
			}
	
			return	nullptr;
	
		}
	
	};
}
