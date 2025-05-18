# 当前目录下,除py子目录外,其余目录都是python扩展
# py子目录可通过gdb从c/c++代码层面调试自定义的python扩展
# MODULE_NAME 必须是要调试的python扩展名,一般是当前目录相应的目录名
# 当前cmake配置中,目录名与python扩展名对应
set(MODULE_NAME utils)
router_directories()