// AMSIClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "amsi.h"
#include "iostream"
using namespace std;

#pragma comment(lib, "amsi.lib")

int InvokeAMSIScan()
{
    cout << "InvokeAMSIScan >>>\n";

    HAMSICONTEXT amsiContext;
    HRESULT hr = S_OK;
    int iRet = -1;

    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    hr = AmsiInitialize(L"AMSIClient", &amsiContext);

    if (SUCCEEDED(hr))
    {
        HAMSISESSION amsiSession = nullptr;
        hr = AmsiOpenSession(amsiContext, &amsiSession);
        if (SUCCEEDED(hr) && amsiSession != nullptr)
        {
            AMSI_RESULT amsiResult = AMSI_RESULT_DETECTED;
            unsigned char buffer[] = "Test";
            //hr = AmsiScanString(amsiContext, L"test", L"d:\\test.txt", amsiSession, &amsiResult);
            hr = AmsiScanBuffer(amsiContext, buffer, sizeof(buffer), L"Scan Buffer Test", amsiSession, &amsiResult);
            if (SUCCEEDED(hr))
            {
                if (AmsiResultIsMalware(amsiResult))
                {
                    iRet = 0;
                    cout << "Malware found\n";
                }
                else
                {
                    iRet = 1;
                    cout << "Malware not found\n";
                }
            }
        }
        AmsiUninitialize(amsiContext);
    }

    CoUninitialize();

    cout << "InvokeAMSIScan <<<\n";
    return iRet;
}

int main()
{
    InvokeAMSIScan();
    getchar();
    return 0;
}

