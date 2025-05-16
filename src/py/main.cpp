#include <Python.h>
int main(int argc, char **argv)
{
    PyConfig pyConfig;
    PyConfig_InitPythonConfig(&pyConfig);
    // PyConfig_SetBytesString(&pyConfig,&pyConfig.exec_prefix,"std");
    Py_InitializeFromConfig(&pyConfig);
    return Py_BytesMain(argc,argv);
}
