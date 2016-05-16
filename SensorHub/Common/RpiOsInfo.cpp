#include "pch.h"
#include "RpiOsInfo.h"
#include <ppltasks.h>

using namespace Windows::Data::Json;
using namespace Windows::Web::Http;
using namespace Windows::Foundation;
using namespace concurrency;

namespace AdapterLib {
	/**
		Call RPI REST API http://localhost:8080/api/os/info to gater Device Information
	*/
	RpiOsInfo::RpiOsInfo()
	{
		
	}


	void RpiOsInfo::LoadDeviceDesc()
	{
		HttpClient^ httpClient = nullptr;
		//try {
			httpClient = ref new HttpClient();
			Uri^ uri = ref new Uri(L"http://localhost:8080/api/os/info");			
			IAsyncOperationWithProgress<Platform::String ^, HttpProgress>^ asyncOperation = httpClient->GetStringAsync(uri);

			auto asyncTask = create_task(asyncOperation);
			asyncTask.then([this](Platform::String ^ tResult) {
				
				ParseJsonResponse(tResult);				
			});
	/*	}		
		finally{
			if (httpClient != nullptr) {
				httpClient->Close();
				httpClient = nullptr;
			}
		}*/
			
	}

	void RpiOsInfo::ParseJsonResponse(Platform::String ^ input)
	{
		JsonObject^ jsonResponse = JsonObject::Parse(input);
		JsonValue^ jsonVal = jsonResponse->GetNamedValue(L"SqmMachineId");
		
		_sqmMachineId = jsonVal->GetString();
		// TODO: insert return statement here
		
	}
}