# RWKV Mobile Android Test

这是一个测试 `librwkv_mobile.so` 原生库的 Android Kotlin 项目。

## 项目结构

```
android_test/
├── app/
│   ├── build.gradle.kts           # App 模块构建配置
│   ├── proguard-rules.pro         # ProGuard 规则
│   └── src/main/
│       ├── AndroidManifest.xml    # Android 清单文件
│       ├── java/com/example/rwkvmobiletest/
│       │   ├── MainActivity.kt    # 主界面
│       │   └── RwkvMobile.kt      # JNI 封装类
│       ├── jniLibs/arm64-v8a/
│       │   └── librwkv_mobile.so  # 原生库
│       └── res/                   # 资源文件
├── build.gradle.kts               # 项目根构建配置
├── settings.gradle.kts            # 项目设置
├── gradle.properties              # Gradle 属性
├── gradlew                        # Gradle 包装器脚本
└── gradle/wrapper/
    ├── gradle-wrapper.jar
    └── gradle-wrapper.properties
```

## 测试的 API

项目测试了以下原生函数：

1. **加载库** - `System.loadLibrary("rwkv_mobile")`
2. **获取可用后端** - `rwkvmobile_runtime_get_available_backend_names()`
3. **初始化/释放运行时** - `rwkvmobile_runtime_init()` / `rwkvmobile_runtime_release()`
4. **获取平台信息**:
   - `rwkvmobile_get_platform_name()`
   - `rwkvmobile_get_soc_name()`
   - `rwkvmobile_get_soc_partname()`
   - `rwkvmobile_get_htp_arch()`

## 构建方法

### 方法一：使用 Android Studio

1. 用 Android Studio 打开 `android_test` 目录
2. 等待 Gradle 同步完成
3. 连接 ARM64 设备或使用模拟器
4. 点击 Run 按钮

### 方法二：使用命令行

```bash
# 进入项目目录
cd example/android_test

# 构建 Debug APK
./gradlew assembleDebug

# APK 输出位置
# app/build/outputs/apk/debug/app-debug.apk

# 安装到设备
adb install app/build/outputs/apk/debug/app-debug.apk
```

## 注意事项

1. **架构限制**: 原生库只支持 `arm64-v8a` 架构，需要在 ARM64 设备上运行
2. **最低 SDK**: 需要 Android API 24 (Android 7.0) 或更高版本
3. **JNI 绑定**: `RwkvMobile.kt` 中的 `external` 函数直接映射到原生库导出的 C 函数

## JNI 函数签名

```kotlin
// 这些函数直接对应 librwkv_mobile.so 导出的 C 函数
external fun rwkvmobile_get_platform_name(): String
external fun rwkvmobile_get_soc_name(): String
external fun rwkvmobile_get_soc_partname(): String
external fun rwkvmobile_get_htp_arch(): String
external fun rwkvmobile_dump_log(): String
external fun rwkvmobile_set_loglevel(level: Int)
external fun rwkvmobile_runtime_init(): Long
external fun rwkvmobile_runtime_release(runtime: Long): Int
external fun rwkvmobile_runtime_get_available_backend_names(buffer: ByteArray, bufferSize: Int): Int
```

## 界面说明

应用界面包含以下按钮：

- **Load Library** - 测试库是否成功加载
- **Get Backends** - 获取可用的后端列表
- **Init/Release** - 测试运行时初始化和释放
- **Platform Info** - 获取平台信息
- **Clear Log** - 清除日志输出

