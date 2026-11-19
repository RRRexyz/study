#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

void enable_utf8_console() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    // 非 Windows 平台什么都不做，Linux/macOS 终端本来就默认 UTF-8
}

int main(int, char**) {
    enable_utf8_console();

    std::cout << "你好世界！" << std::endl;
}
