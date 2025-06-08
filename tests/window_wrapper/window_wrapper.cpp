#include "window_wrapper.hpp"
// #include <windowsx.h>
#include <algorithm>
#include <stdexcept>
#include <cassert>
bool windows::Window::s_globalShouldQuit = false;
windows::Window::Window(HINSTANCE hInstance, const std::wstring &title, const Style &style, Window *parent)
    : m_hInstance(hInstance),
      m_title(title),
      m_style(style),
      m_parent(parent),
      m_shouldQuit(false)
{ // 初始化退出标志

    if (!hInstance)
    {
        throw std::invalid_argument("Invalid instance handle");
    }

    if (parent)
    {
        m_style.hWndParent = parent->GetHandle();
        parent->AddChild(this);
    }

    if (!RegisterWindowClass())
    {
        throw std::runtime_error("Failed to register window class");
    }
    if (!Create())
    {
        throw std::runtime_error("Failed to create window");
    }
}
windows::Window::~Window()
{
    Destroy();
}
windows::Window::Window(Window &&other) noexcept
{
    *this = std::move(other);
}
windows::Window &windows::Window::operator=(Window &&other) noexcept
{
    if (this != &other)
    {
        Destroy();

        m_hInstance = other.m_hInstance;
        m_hWnd = other.m_hWnd;
        m_title = std::move(other.m_title);
        m_style = other.m_style;
        m_parent = other.m_parent;
        m_children = std::move(other.m_children);
        m_messageHandlers = std::move(other.m_messageHandlers);
        m_userData = std::move(other.m_userData);

        for (auto child : m_children)
        {
            child->m_parent = this;
        }

        if (m_hWnd)
        {
            SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        }

        other.m_hWnd = nullptr;
        other.m_parent = nullptr;
    }
    return *this;
}
void windows::Window::AddChild(Window *child)
{
    if (!child || child == this || child->m_parent == this)
        return;

    if (child->m_parent)
    {
        child->m_parent->RemoveChild(child);
    }

    m_children.push_back(child);
    child->m_parent = this;
    child->m_style.hWndParent = m_hWnd;

    if (m_hWnd && child->m_hWnd)
    {
        SetParent(child->m_hWnd, m_hWnd);
    }
}

void windows::Window::RemoveChild(Window *child)
{
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end())
    {
        (*it)->m_parent = nullptr;
        if ((*it)->m_hWnd && m_hWnd)
        {
            SetParent((*it)->m_hWnd, nullptr);
        }
        m_children.erase(it);
    }
}

windows::Window *windows::Window::FindChildByHandle(HWND hWnd) const
{
    if (!hWnd)
        return nullptr;

    for (auto child : m_children)
    {
        if (child->m_hWnd == hWnd)
        {
            return child;
        }
        if (auto found = child->FindChildByHandle(hWnd))
        {
            return found;
        }
    }
    return nullptr;
}

void windows::Window::Show(int nCmdShow)
{
    if (m_hWnd)
    {
        ShowWindow(m_hWnd, nCmdShow);
        UpdateWindow(m_hWnd);

        for (auto child : m_children)
        {
            child->Show(nCmdShow);
        }
    }
}

void windows::Window::Hide()
{
    if (m_hWnd)
    {
        ShowWindow(m_hWnd, SW_HIDE);
    }
}

void windows::Window::Enable()
{
    if (m_hWnd)
    {
        EnableWindow(m_hWnd, TRUE);
    }
}

void windows::Window::Disable()
{
    if (m_hWnd)
    {
        EnableWindow(m_hWnd, FALSE);
    }
}

void windows::Window::SetFocus()
{
    if (m_hWnd)
    {
        ::SetFocus(m_hWnd);
    }
}

void windows::Window::SetPosition(int x, int y, int width, int height, UINT flags)
{
    if (m_hWnd)
    {
        SetWindowPos(m_hWnd, nullptr, x, y, width, height, flags);
    }
}

RECT windows::Window::GetPosition() const
{
    RECT rect = {0};
    if (m_hWnd)
    {
        GetWindowRect(m_hWnd, &rect);
    }
    return rect;
}

RECT windows::Window::GetClientRect() const
{
    RECT rect = {0};
    if (m_hWnd)
    {
        ::GetClientRect(m_hWnd, &rect);
    }
    return rect;
}

bool windows::Window::IsVisible() const
{
    return m_hWnd && IsWindowVisible(m_hWnd);
}

bool windows::Window::IsEnabled() const
{
    return m_hWnd && IsWindowEnabled(m_hWnd);
}

bool windows::Window::IsActive() const
{
    return m_hWnd && (GetActiveWindow() == m_hWnd);
}

void windows::Window::addMessageHandler(UINT message, MessageHandler handler)
{
    m_messageHandlers[message].push_back(handler);
}

LRESULT windows::Window::sendMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    return m_hWnd ? SendMessage(m_hWnd, msg, wParam, lParam) : 0;
}

BOOL windows::Window::postMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    return m_hWnd ? PostMessage(m_hWnd, msg, wParam, lParam) : FALSE;
}

int windows::Window::RunMessageLoop(Window *mainWindow)
{
    MSG msg = {0};

    // 如果指定了主窗口，则使用其退出标志
    bool &shouldQuit = mainWindow ? mainWindow->m_shouldQuit : s_globalShouldQuit;

    while (!shouldQuit && GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

void windows::Window::ClearUserData()
{
    m_userData.clear();
}

HWND windows::Window::GetHandle() const
{
    return m_hWnd;
}

HINSTANCE windows::Window::GetInstance() const
{
    return m_hInstance;
}

windows::Window *windows::Window::GetParent() const
{
    return m_parent;
}

const std::vector<windows::Window *> &windows::Window::GetChildren() const
{
    return m_children;
}

const std::wstring &windows::Window::GetTitle() const
{
    return m_title;
}
bool windows::Window::Create()
{
    m_hWnd = CreateWindowEx(
        m_style.dwExStyle,
        GetWindowClassName(),
        m_title.c_str(),
        m_style.dwStyle,
        m_style.x,
        m_style.y,
        m_style.width,
        m_style.height,
        m_style.hWndParent,
        m_style.hMenu,
        m_hInstance,
        this);

    if (!m_hWnd)
    {
        return false;
    }

    SendMessage(m_hWnd, WM_CREATE, 0, 0);
    return true;
}
void windows::Window::OnCreate() {}
void windows::Window::OnClose() { DestroyWindow(m_hWnd); }
bool windows::Window::RemoveUserData(const std::string &key)
{
    return m_userData.erase(key) > 0;
}
void windows::Window::Destroy()
{
    for (auto child : m_children)
    {
        delete child;
    }
    m_children.clear();

    if (m_hWnd)
    {
        if (m_parent)
        {
            m_parent->RemoveChild(this);
        }

        SetWindowLongPtr(m_hWnd, GWLP_USERDATA, 0);
        DestroyWindow(m_hWnd);
        m_hWnd = nullptr;
    }
}
void windows::Window::OnDestroy()
{
    // 标记需要退出消息循环
    m_shouldQuit = true;

    // 如果是主窗口，则发送退出消息
    if (!m_parent)
    {
        PostQuitMessage(0);
    }
}
LRESULT windows::Window::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);
    OnDraw(hdc, ps);
    EndPaint(m_hWnd, &ps);
    return 0;
}
void windows::Window::OnDraw(HDC hdc, const PAINTSTRUCT &ps)
{
    RECT rc;
    ::GetClientRect(m_hWnd, &rc);
    FillRect(hdc, &rc, m_style.hBackground);
}
void windows::Window::OnParentNotify(UINT message, UINT childID, LPARAM lParam)
{
    if (m_parent)
    {
        SendMessage(m_parent->GetHandle(), WM_PARENTNOTIFY,
                    MAKEWPARAM(message, childID), lParam);
    }
}
const windows::Window::Style &windows::Window::GetStyle() const
{
    return m_style;
}

void windows::Window::SetTitle(const std::wstring &title)
{
    m_title = title;
    if (m_hWnd)
    {
        SetWindowText(m_hWnd, title.c_str());
    }
}

LRESULT windows::Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto it = m_messageHandlers.find(uMsg);
    if (it != m_messageHandlers.end())
    {
        for (auto &handler : it->second)
        {
            LRESULT result = handler(*this, uMsg, wParam, lParam);
            if (result != 0)
            {
                return result;
            }
        }
    }

    switch (uMsg)
    {
    case WM_CLOSE:
        OnClose();
        return 0;

    case WM_DESTROY:
        OnDestroy();
        return 0;

    case WM_PAINT:
        return OnPaint();

    case WM_SIZE:
        OnSize(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_COMMAND:
        OnCommand(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam));
        return 0;

    case WM_NOTIFY:
        return OnNotify(static_cast<int>(wParam), reinterpret_cast<NMHDR *>(lParam));

    case WM_PARENTNOTIFY:
        OnParentNotify(LOWORD(wParam), HIWORD(wParam), lParam);
        return 0;

    default:
        return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
}

bool windows::Window::RegisterWindowClass()
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.hCursor = m_style.hCursor;
    wc.hbrBackground = m_style.hBackground;
    wc.lpszClassName = GetWindowClassName();
    wc.hIcon = m_style.hIcon;
    wc.hIconSm = m_style.hIconSm;

    return RegisterClassEx(&wc) != 0;
}

LPCWSTR windows::Window::GetWindowClassName()
{
    static const wchar_t *className = L"WindowClass";
    return className;
}

LRESULT windows::Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window *pThis = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
        pThis = reinterpret_cast<Window *>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->m_hWnd = hwnd;
    }
    else
    {
        pThis = reinterpret_cast<Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

windows::Window::Style::Style()
    : dwExStyle(0),
      dwStyle(WS_OVERLAPPEDWINDOW),
      x(CW_USEDEFAULT),
      y(CW_USEDEFAULT),
      width(800),
      height(600),
      hWndParent(nullptr),
      hMenu(nullptr),
      hBackground((HBRUSH)(COLOR_WINDOW + 1)),
      hCursor(LoadCursor(nullptr, IDC_ARROW)),
      hIcon(LoadIcon(nullptr, IDI_APPLICATION)),
      hIconSm(nullptr)
{
}
