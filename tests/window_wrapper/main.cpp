#include"window_wrapper.hpp"
// 使用示例
class MainWindow : public windows::Window
{
public:
    MainWindow(HINSTANCE hInst) : Window(hInst, L"Main Window")
    {
        addMessageHandler(WM_CREATE, [this](Window &, UINT, WPARAM, LPARAM)
                          {
            // 创建子窗口
            Window::Style childStyle;
            childStyle.width = 300;
            childStyle.height = 200;
            
            auto child = std::make_unique<Window>(
                GetInstance(), 
                L"Child Window", 
                childStyle, 
                this
            );
            
            SetUserData("child", std::move(child));
            return 0; });

        addMessageHandler(WM_SIZE, [this](Window &, UINT, WPARAM, LPARAM lParam)
                          {
            if (auto child = GetUserData<Window>("child")) {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                child->SetPosition(10, 10, width/2-20, height-20);
            }
            return 0; });
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    try
    {
        MainWindow mainWindow(hInstance);
        mainWindow.Show(nCmdShow);

        return windows::Window::RunMessageLoop();
    }
    catch (const std::exception &e)
    {
        MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
        return -1;
    }
}