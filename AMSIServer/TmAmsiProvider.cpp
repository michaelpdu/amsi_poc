// TmAmsiProvider.cpp : Implementation of CTmAmsiProvider

#include "stdafx.h"
#include "TmAmsiProvider.h"

#include "iostream"
// CTmAmsiProvider

static BOOL g_bDumpAttr = TRUE;
static BOOL g_bDumpData = TRUE;

static BOOL _ScanBuffer(PVOID data, ULONGLONG content_size)
{
	PWCHAR signature = L"[guid]::newguid()";
	PWCHAR p = (PWCHAR)data;
	PWCHAR q = signature;
	ULONGLONG size = 0;

	while (*p && size < content_size) {
		if (*p != *q) {
			//std::wcout << L"\ndata[i]= " << *p << L" signature[i]= " << *q << L"\n";
			return TRUE;
		}

		++p, ++q, size += 2;
	}

	std::wcout << L"\n" << __FUNCTIONW__ << L":!!!Got guidgen!!!\n";
	return FALSE;
}

HRESULT STDMETHODCALLTYPE CTmAmsiProvider::Scan(
    /* [in] */ IAmsiStream *stream,
    /* [out] */ AMSI_RESULT *result)
{
#if 1
    std::cout << "\n" << __FUNCTION__ << ">>>\n";
    std::cout << "Hello! AMSI\n";

	*result = AMSI_RESULT_CLEAN;
	ULONG retData = 0;

	// try content name
	PVOID content_name = NULL;
    HRESULT hr = stream->GetAttribute(AMSI_ATTRIBUTE_CONTENT_NAME,
        0,
        NULL,
        &retData);

    std::cout << std::showbase << std::hex;

    if (hr == E_NOT_SUFFICIENT_BUFFER) {
		if (retData) {
			content_name = malloc(retData);
		}
    }
    else {
		std::cout << "Get AMSI_ATTRIBUTE_CONTENT_NAME size error! HResult= " << hr << "\n";
    }
	
	if (content_name) {
		hr = stream->GetAttribute(AMSI_ATTRIBUTE_CONTENT_NAME,
									retData,
									(unsigned char*)content_name,
									&retData);
		if (hr != S_OK) {
			std::cout << "Get AMSI_ATTRIBUTE_CONTENT_NAME error! HResult= " << hr << "\n";
		}
		else {
			if (g_bDumpAttr) {
				std::wcout << L"Content name: " << LPWSTR(content_name) << L", read size= " << retData << L" bytes\n";
			}
		}
	}


	// try app name
	PVOID app_name = NULL;
	hr = stream->GetAttribute(AMSI_ATTRIBUTE_APP_NAME,
								0,
								NULL,
								&retData);

	if (hr == E_NOT_SUFFICIENT_BUFFER) {
		if (retData) {
			app_name = malloc(retData);
		}
	}
	else {
		std::cout << "Get AMSI_ATTRIBUTE_CONTENT_NAME size error! HResult= " << hr << "\n";
	}

	if (app_name) {
		hr = stream->GetAttribute(AMSI_ATTRIBUTE_APP_NAME,
			retData,
			(unsigned char*)app_name,
			&retData);

		if (hr != S_OK) {
			std::cout << "Get AMSI_ATTRIBUTE_APP_NAME error! HResult= " << hr << "\n";
		}
		else {
			if (g_bDumpAttr) {
				std::wcout << L"App name: " << LPWSTR(app_name) << L", read size= " << retData << L" bytes\n";
			}
		}
	}

	PVOID content_addr = NULL;
	hr = stream->GetAttribute(AMSI_ATTRIBUTE_CONTENT_ADDRESS,
		sizeof(PVOID),
		(unsigned char*)&content_addr,
		&retData);
	if (hr != S_OK) {
		std::cout << "Get AMSI_ATTRIBUTE_CONTENT_ADDRESS error! HResult= " << hr << "\n";
	}
	else {
		if (g_bDumpAttr) {
			std::wcout << L"Content addr: " << content_addr << L", read size= " << retData << L" bytes\n";
		}
	}

	ULONGLONG content_size = 0;
	hr = stream->GetAttribute(AMSI_ATTRIBUTE_CONTENT_SIZE,
		sizeof(ULONGLONG),
		(unsigned char *)&content_size,
		&retData);
	if (hr != S_OK) {
		std::cout << "Get AMSI_ATTRIBUTE_CONTENT_SIZE error! HResult= " << hr << "\n";
	}
	else {
		if (g_bDumpAttr) {
			std::wcout << L"Content size: " << content_size << L", read size= " << retData << L" bytes\n";
		}
	}

	unsigned char *data = NULL;
	if (content_addr && content_size) {
		data = (unsigned char *)malloc(content_size);
		if (data) {
			RtlCopyMemory(data, content_addr, content_size);
			if (g_bDumpData) {

				std::wcout << L"Dump data: ";
				for (ULONGLONG i = 0; i < content_size;) {
					std::wcout << (WCHAR)data[i];
					i += 2;
				}
				std::cout << "\nDone\n";
			}

			if (!_ScanBuffer((PVOID)data, content_size)) {
				*result = AMSI_RESULT_DETECTED;
			}
		}
		else {
			std::cout << "malloc data buffer for" << content_size << "bytes failed\n";
		}
	}

	// try get next session
#if 0
	PVOID session = NULL;
	hr = stream->GetAttribute(AMSI_ATTRIBUTE_SESSION,
		sizeof(PVOID),
		(unsigned char *)&session,
		&retData);

	if (hr != S_OK) {
		std::cout << "Get AMSI_ATTRIBUTE_SESSION error! HResult= " << hr << "\n";
	}
	else {
		//std::wcout << L"Next protion ptr: " << session << L", read size= " << retData << L" bytes\n";
	}
#endif

	// try read from stream
#if 0
	if (data) {
		hr = stream->Read(0, 128, data, &retData);
		if (hr != S_OK) {
			std::cout << "Read buffer error! HResult= " << hr << "\n";
		}
		else {
			std::cout << "Content: " << LPSTR(data) << ", size= " << retData << "bytes\n";
		}
	}
#endif
	//prepare to leave
	if (content_name) {
		free(content_name);
	}

	if (app_name) {
		free(app_name);
	}

	if (data) {
		free(data);
	}
#endif
	std::cout << __FUNCTION__ << "<<<\n";
    return S_OK;
}

void STDMETHODCALLTYPE CTmAmsiProvider::CloseSession(
    /* [in] */ ULONGLONG session)
{
    std::cout << __FUNCTION__ << "\n";
}

HRESULT STDMETHODCALLTYPE CTmAmsiProvider::DisplayName(
    /* [annotation][string][out] */
    _Out_  LPWSTR *displayName)
{
	std::cout << __FUNCTION__ << "\n";
	return S_OK;
}
