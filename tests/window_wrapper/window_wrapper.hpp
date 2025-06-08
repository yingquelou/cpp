#ifndef WINDOW_WRAPPER_H
#define WINDOW_WRAPPER_H 1
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

namespace windows
{
    class Window
    {
    public:
        // 窗口样式配置
        struct Style
        {
            DWORD dwExStyle;
            DWORD dwStyle;
            int x;
            int y;
            int width;
            int height;
            HWND hWndParent;
            HMENU hMenu;
            HBRUSH hBackground;
            HCURSOR hCursor;
            HICON hIcon;
            HICON hIconSm;

            Style();
        };

        // 构造函数
        Window(HINSTANCE hInstance,
               const std::wstring &title = L"Window",
               const Style &style = Style(),
               Window *parent = nullptr);

        // 虚析构函数
        virtual ~Window();

        // 禁用拷贝
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        // 移动语义
        Window(Window &&other) noexcept;

        Window &operator=(Window &&other) noexcept;

        // 窗口树管理
        void AddChild(Window *child);

        void RemoveChild(Window *child);

        Window *FindChildByHandle(HWND hWnd) const;

        template <typename Visitor>
        void Traverse(Visitor visitor, bool includeSelf = true)
        {
            if (includeSelf)
            {
                visitor(this);
            }
            for (auto child : m_children)
            {
                child->Traverse(visitor, true);
            }
        }

        // 窗口操作
        void Show(int nCmdShow = SW_SHOW);

        void Hide();

        void Enable();

        void Disable();

        void SetFocus();

        void SetPosition(int x, int y, int width, int height, UINT flags = SWP_NOZORDER);

        RECT GetPosition() const;

        RECT GetClientRect() const;

        bool IsVisible() const;

        bool IsEnabled() const;

        bool IsActive() const;

        // 消息处理
        using MessageHandler = std::function<LRESULT(Window &, UINT, WPARAM, LPARAM)>;

        void addMessageHandler(UINT message, MessageHandler handler);

        LRESULT sendMessage(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);

        BOOL postMessage(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);

        static int RunMessageLoop(Window *mainWindow = nullptr);

        // 用户数据管理
        template <typename T>
        void SetUserData(const std::string &key, std::unique_ptr<T> data)
        {
            m_userData[key] = std::unique_ptr<void, VoidDeleter>(
                data.release(),
                VoidDeleter{[](void *ptr)
                            { delete static_cast<T *>(ptr); }});
        }

        template <typename T>
        T *GetUserData(const std::string &key)
        {
            auto it = m_userData.find(key);
            if (it != m_userData.end())
            {
                return static_cast<T *>(it->second.get());
            }
            return nullptr;
        }

        // template <typename T>
        // const T *GetUserData(const std::string &key) const
        // {
        //     auto it = m_userData.find(key);
        //     if (it != m_userData.end())
        //     {
        //         return static_cast<const T *>(it->second.get());
        //     }
        //     return nullptr;
        // }

        bool RemoveUserData(const std::string &key);

        void ClearUserData();

        // 访问器
        HWND GetHandle() const;
        HINSTANCE GetInstance() const;
        Window *GetParent() const;
        const std::vector<Window *> &GetChildren() const;
        const std::wstring &GetTitle() const;
        const Style &GetStyle() const;

        void SetTitle(const std::wstring &title);

    protected:
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void OnCreate();
        virtual void OnClose();
        virtual void OnDestroy();
        virtual LRESULT OnPaint();
        virtual void OnDraw(HDC hdc, const PAINTSTRUCT &ps);
        virtual void OnSize(UINT state, int width, int height) {}
        virtual void OnCommand(int id, int code, HWND hWndCtrl) {}
        virtual LRESULT OnNotify(int id, NMHDR *pNMHDR) { return 0; }
        virtual void OnParentNotify(UINT message, UINT childID, LPARAM lParam);

    private:
        struct VoidDeleter
        {
            void (*deleter)(void *);
            void operator()(void *ptr) const { deleter(ptr); }
        };

        HINSTANCE m_hInstance;
        HWND m_hWnd = nullptr;
        std::wstring m_title;
        Style m_style;
        Window *m_parent = nullptr;
        std::vector<Window *> m_children;
        std::unordered_map<UINT, std::vector<MessageHandler>> m_messageHandlers;
        std::unordered_map<std::string, std::unique_ptr<void, VoidDeleter>> m_userData;

        // 添加退出标志
        bool m_shouldQuit = false;
        static bool s_globalShouldQuit; // 用于没有主窗口的情况

        void Destroy();

        bool RegisterWindowClass();

        bool Create();

        static LPCWSTR GetWindowClassName();

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };

} // namespace windows

#endif