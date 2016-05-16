#pragma once
namespace AdapterLib {

ref class RpiOsInfo 
	{
	internal:
		RpiOsInfo();

		// The device name
		Platform::String^           Name;

		// The device manufacturer name
		Platform::String^           VendorName;

		// The device model name
		Platform::String^           Model;

		// The device version number
		Platform::String^           Version;

		// The device serial number
		static Platform::String^           _sqmMachineId;

		// The specific device description
		Platform::String^           Description;

	public:
		void LoadDeviceDesc();

		/*
		{"ComputerName" : "minwinpc",
		"Language" : "en-us", "MacAddress" : "B827EBA95A4E",
		"OsEdition" : "IoTUAP", "OsVersion" : "10240.16384.armfre.th1.150709-1700",
		"Platform" : "Raspberry Pi 2 Model B",
		"SqmMachineId" : "{251FA899-C3A6-41A7-BD53-B2C8DBB79E50}"}
		*/
		static property Platform::String^ SqmMachineId
		{
			Platform::String^ get() { return _sqmMachineId; }
		}
	private:
		void ParseJsonResponse(Platform::String ^ input);
	};


}