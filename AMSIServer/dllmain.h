// dllmain.h : Declaration of module class.

class CAMSIServerModule : public ATL::CAtlDllModuleT< CAMSIServerModule >
{
public :
	DECLARE_LIBID(LIBID_AMSIServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_AMSISERVER, "{10334B57-5565-46C8-92DE-16BC857B7318}")
};

extern class CAMSIServerModule _AtlModule;
