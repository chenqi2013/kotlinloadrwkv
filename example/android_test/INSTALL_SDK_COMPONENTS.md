# 解决 SDK 组件安装失败问题

## 错误信息
```
Caused by: java.lang.RuntimeException: com.android.builder.sdk.InstallFailedException: 
Failed to install the following SDK components:
```

## 原因
项目需要 **NDK** 和 **CMake** 来编译 JNI 桥接代码，但这些组件未安装。

## 🔧 解决步骤

### 方法 1: Android Studio GUI（最简单）

1. **打开 SDK Manager**
   - 点击 `Tools` → `SDK Manager`
   - 或使用快捷键: macOS `⌘,` / Windows `Ctrl+Alt+S`

2. **切换到 SDK Tools 标签**
   
3. **勾选并安装以下组件**:
   ```
   ☑ NDK (Side by side)
     └─ 推荐版本: 26.1.10909125 或最新
   
   ☑ CMake
     └─ 版本: 3.22.1 或更新
   
   ☑ Android SDK Build-Tools
     └─ 版本: 34.0.0 或更新
   
   ☑ Android SDK Platform-Tools (通常已安装)
   ```

4. **点击 OK/Apply**，等待下载和安装完成

5. **重启 Android Studio**（可选但推荐）

6. **重新构建项目**:
   ```bash
   ./gradlew clean
   ./gradlew build
   ```

### 方法 2: 命令行安装

如果 GUI 安装失败，使用命令行：

#### macOS/Linux:
```bash
# 进入 SDK 目录
cd ~/Library/Android/sdk

# 列出可用的 NDK 版本
./cmdline-tools/latest/bin/sdkmanager --list | grep ndk

# 安装 NDK
./cmdline-tools/latest/bin/sdkmanager "ndk;26.1.10909125"

# 安装 CMake
./cmdline-tools/latest/bin/sdkmanager "cmake;3.22.1"

# 验证安装
./cmdline-tools/latest/bin/sdkmanager --list_installed
```

#### Windows:
```cmd
# 进入 SDK 目录
cd %LOCALAPPDATA%\Android\Sdk

# 安装 NDK
cmdline-tools\latest\bin\sdkmanager.bat "ndk;26.1.10909125"

# 安装 CMake
cmdline-tools\latest\bin\sdkmanager.bat "cmake;3.22.1"
```

### 方法 3: 手动下载安装

如果网络问题导致安装失败：

1. **下载 NDK**:
   - 访问: https://developer.android.com/ndk/downloads
   - 下载 NDK r26 或更新版本
   - 解压到 `~/Library/Android/sdk/ndk/26.1.10909125/`

2. **下载 CMake**:
   - Android Studio 会自动提示下载
   - 或从 https://cmake.org/download/ 下载

## 🚫 临时禁用 CMake（仅用于测试其他功能）

如果你暂时不需要 JNI 功能，可以注释掉 `build.gradle.kts` 中的 CMake 配置：

```kotlin
// 注释掉这两部分

// Point to CMakeLists.txt for native build
/*
externalNativeBuild {
    cmake {
        path = file("src/main/cpp/CMakeLists.txt")
        version = "3.22.1"
    }
}
*/

// 在 defaultConfig 中注释掉
/*
externalNativeBuild {
    cmake {
        cppFlags += "-std=c++17"
        arguments += "-DANDROID_STL=c++_shared"
    }
}
*/
```

⚠️ **注意**: 注释后 JNI 桥接将无法工作，所有调用 `RwkvMobile` 的代码会失败！

## ✅ 验证安装成功

安装完成后，在终端运行：

```bash
# 检查 NDK
ls ~/Library/Android/sdk/ndk/
# 应该看到版本号文件夹，如: 26.1.10909125

# 检查 CMake
ls ~/Library/Android/sdk/cmake/
# 应该看到版本号文件夹，如: 3.22.1

# 测试构建
./gradlew clean assembleDebug
```

## 🐛 常见问题

### 1. "cmdline-tools not found"
**解决**: 安装 Android SDK Command-line Tools
- SDK Manager → SDK Tools → Android SDK Command-line Tools (latest)

### 2. "License not accepted"
```bash
cd ~/Library/Android/sdk
yes | ./cmdline-tools/latest/bin/sdkmanager --licenses
```

### 3. 网络连接问题
- 配置代理: `Settings` → `HTTP Proxy`
- 或使用国内镜像源

### 4. 磁盘空间不足
- NDK 需要 ~1-2 GB
- 清理旧版本: 删除 `~/Library/Android/sdk/ndk/` 中的旧版本

## 📱 构建成功后

1. 同步项目: `File` → `Sync Project with Gradle Files`
2. 清理构建: `Build` → `Clean Project`
3. 重新构建: `Build` → `Rebuild Project`
4. 运行应用

## 💡 推荐设置

在 `local.properties` 中指定 SDK 路径（如果未自动配置）：

```properties
sdk.dir=/Users/你的用户名/Library/Android/sdk
ndk.dir=/Users/你的用户名/Library/Android/sdk/ndk/26.1.10909125
```

---

安装完成后，你的 JNI 桥接层就能正常编译了！🎉


