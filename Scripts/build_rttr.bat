@echo off
REM RTTR编译脚本 - 生成动态库
setlocal enabledelayedexpansion

REM 设置环境变量

set RTTR_SOURCE_DIR=%1Source\ThirdParty\rttr
set INSTALL_DIR=%1Binaries\ThirdParty\rttr
set BUILD_DIR=%1\Build\rttr

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
cmake %RTTR_SOURCE_DIR% ^
    -G "Visual Studio 17 2022" ^
    -A x64 ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBUILD_SHARED_LIBS=ON ^
    -DBUILD_UNIT_TESTS=OFF ^
    -DBUILD_EXAMPLES=OFF ^
    -DBUILD_BENCHMARKS=OFF ^
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
    -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%"

if %errorlevel% neq 0 (
    echo CMake Failed
    popd
    pause
    exit /b 1
)

:: 编译并安装
cmake --build . --config Release --target install

if %errorlevel% neq 0 (
    echo build and install failed
    popd
    pause
    exit /b 1
)

:: 恢复原始目录
popd

echo rttr generate finished, install to: %INSTALL_DIR%

endlocal