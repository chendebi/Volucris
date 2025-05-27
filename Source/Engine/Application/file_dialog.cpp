#include <Application/file_dialog.h>
#include <Windows.h>
#include <commdlg.h>
#include <Application/application.h>
#include <Application/window.h>
#include <Core/volucris.h>

std::string volucris::FileDialog::GetOpenFilePath(const char* filter, const char* title, const char* initialDirectory)
{
    OPENFILENAMEA ofn;          // 文件对话框结构体（A 表示 ANSI 版本）
    char szFile[260] = { 0 };   // 存储选择的文件路径

    // 初始化 OPENFILENAME 结构
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;       // 父窗口句柄（可以是 NULL）
    ofn.lpstrFile = szFile;     // 接收文件路径的缓冲区
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;   // 文件类型过滤器
    ofn.nFilterIndex = 1;       // 默认过滤器索引
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // 打开文件对话框
    if (GetOpenFileName(&ofn))
    {
        return ofn.lpstrFile;    // 返回选择的文件路径
    }
    return "";  // 用户取消选择
}
