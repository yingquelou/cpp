#include <windows.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    freopen("E:/projects/C/demo/log.txt", "w", stdout);
    if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE)))
    {
        CLSID clsid;
        if (SUCCEEDED(CLSIDFromProgID(OLESTR("Word.Application"), &clsid)))
        {
            IDispatch *pWordApp = NULL;
            if (SUCCEEDED(CoCreateInstance(&clsid, NULL, CLSCTX_LOCAL_SERVER, &IID_IDispatch, (LPVOID*)&pWordApp)))
            {
                UINT ct = 0;
                OLECHAR *app = OLESTR("Documents");
                DISPID doc;
                if (SUCCEEDED(pWordApp->lpVtbl->GetIDsOfNames(pWordApp, &IID_NULL, &app, 1, LOCALE_USER_DEFAULT, &doc)))
                {
                    DISPPARAMS params = {NULL, NULL, 0, 0};
                    VARIANT res;
                    pWordApp->lpVtbl->Invoke(pWordApp, doc, &IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params, &res, NULL, NULL);
                    // res.vt=
                }
            }
        }
        CoUninitialize();
    }
    fclose(stdout);
    return 0;
}