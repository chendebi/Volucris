@echo off
:: assimp 编译安装脚本 (CMD 版本)

:: 参数设置
set ASSIMP_SOURCE_DIR=%~dp0Source\ThirdParty\assimp
set INSTALL_DIR=%~dp0Binaries\ThirdParty\assimp
set BUILD_DIR=%~dp0\Build\assimp

:: 检查 CMake 是否可用
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo 错误: 未找到 CMake，请确保 CMake 已安装并添加到 PATH 环境变量中
    pause
    exit /b 1
)

:: 显示 CMake 版本
cmake --version

:: 创建构建目录
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

:: 进入构建目录
pushd "%BUILD_DIR%"

:: 运行 CMake 配置
cmake %ASSIMP_SOURCE_DIR% ^
    -G "Visual Studio 17 2022" ^
    -A x64 ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DASSIMP_BUILD_TESTS=OFF ^
    -DASSIMP_BUILD_ASSIMP_TOOLS=OFF ^
    -DASSIMP_BUILD_SAMPLES=OFF ^
    -DASSIMP_INSTALL_PDB=OFF ^
    -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%"

if %errorlevel% neq 0 (
    echo CMake 配置失败
    popd
    pause
    exit /b 1
)

:: 编译并安装
cmake --build . --config Release --target install

if %errorlevel% neq 0 (
    echo 编译安装失败
    popd
    pause
    exit /b 1
)

:: 恢复原始目录
popd

echo assimp 编译安装完成，已安装到: %INSTALL_DIR%
pause