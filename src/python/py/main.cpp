#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "debug.conf.h"
#define Check(exp)                     \
    do                                 \
    {                                  \
        if (PyStatus_Exception((exp))) \
        {                              \
            goto exception;            \
        }                              \
    } while (0)
int main(int argc, char *argv[])
{
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    /* 以下是可选的但推荐使用 */
    Check(status = PyConfig_SetBytesArgv(&config, argc, argv));
    Check(PyConfig_SetBytesString(&config, &config.exec_prefix, __EXEC_PREFIX__));
    // 根据CMake 变量MODULE_NAME以configure_file实现调试指定python模块
    PyImport_AppendInittab(__MODULE_NAME__, PyInit_MODULE);

    Check(status = Py_InitializeFromConfig(&config));
    PyConfig_Clear(&config);

    return Py_RunMain();

exception:
    PyConfig_Clear(&config);
    Py_ExitStatusException(status);
}