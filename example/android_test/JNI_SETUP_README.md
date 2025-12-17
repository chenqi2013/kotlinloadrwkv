# RWKV Mobile Android JNI 桥接配置说明

## 问题背景

`librwkv_mobile.so` 导出的是纯 C 函数（如 `rwkvmobile_runtime_init`），而不是 JNI 函数格式（如 `Java_com_example_...`）。因此需要创建一个 JNI 桥接层来连接 Java/Kotlin 和 C 库。

## 解决方案架构

```
Java/Kotlin (RwkvMobile.kt)
    ↓ JNI 调用
rwkv_jni.so (JNI 桥接层, C++)
    ↓ C 函数调用
librwkv_mobile.so (原始 RWKV 库)
```

## 文件结构

```
app/
├── build.gradle.kts          # 配置 CMake 构建
├── src/main/
    ├── cpp/
    │   ├── CMakeLists.txt    # CMake 构建脚本
    │   └── rwkv_jni.cpp      # JNI 桥接实现
    ├── jniLibs/
    │   └── arm64-v8a/
    │       └── librwkv_mobile.so  # 原始 RWKV 库
    └── java/com/example/rwkvmobiletest/
        ├── MainActivity.kt   # 测试界面
        └── RwkvMobile.kt     # Kotlin JNI 接口
```

## 已实现的 JNI 函数

### 设备信息
- `rwkvmobile_get_platform_name()` → String
- `rwkvmobile_get_soc_name()` → String  
- `rwkvmobile_get_soc_partname()` → String
- `rwkvmobile_get_htp_arch()` → String
- `rwkvmobile_dump_log()` → String

### Runtime 管理
- `rwkvmobile_runtime_init()` → Long (runtime handle)
- `rwkvmobile_runtime_release(handle: Long)` → Int
- `rwkvmobile_runtime_get_available_backend_names(buffer: ByteArray, size: Int)` → Int

### 日志
- `rwkvmobile_set_loglevel(level: Int)`

## 构建步骤

1. **确保 NDK 已安装**
   - Android Studio → SDK Manager → SDK Tools → NDK (Side by side)
   - 推荐版本: NDK 25.x 或更新

2. **同步 Gradle**
   ```bash
   ./gradlew clean
   ./gradlew build
   ```

3. **CMake 会自动编译 JNI 桥接库**
   - 输出: `app/build/intermediates/cmake/.../librwkv_jni.so`
   - 自动打包进 APK

## 运行测试

1. 连接 Android 设备或启动模拟器（arm64-v8a 架构）
2. 运行应用
3. 在界面上点击测试按钮:
   - **Test Library Loading**: 检查库加载状态
   - **Get Platform Info**: 获取设备平台信息
   - **Init Runtime**: 测试 runtime 初始化和释放
   - **Get Backends**: 获取可用的后端列表

## 查看日志

使用 logcat 查看详细日志:
```bash
adb logcat | grep -E "RWKV_JNI|RwkvMobile"
```

## 常见问题

### 1. UnsatisfiedLinkError: dlopen failed
- 检查 `librwkv_mobile.so` 是否在 `jniLibs/arm64-v8a/` 目录
- 检查设备架构是否为 arm64-v8a
- 查看 logcat 详细错误信息

### 2. No implementation found for ...
- 说明 JNI 函数名不匹配
- 检查 `rwkv_jni.cpp` 中的函数签名
- 确认 RwkvMobile 类的包名正确

### 3. CMake 构建失败
- 检查 NDK 是否安装
- 检查 CMakeLists.txt 路径是否正确
- 查看 Build Output 中的详细错误

### 4. 找不到 librwkv_mobile.so
- CMakeLists.txt 中的 RWKV_MOBILE_LIB_DIR 路径需要正确
- 确保库文件在编译时可访问

## 添加更多 JNI 函数

如果需要添加更多 C 函数的 JNI 绑定:

1. **在 `rwkv_jni.cpp` 中添加 extern 声明**:
   ```cpp
   extern "C" {
       int your_c_function(int param);
   }
   ```

2. **实现 JNI 包装函数**:
   ```cpp
   JNIEXPORT jint JNICALL
   Java_com_example_rwkvmobiletest_RwkvMobile_your_1c_1function(
           JNIEnv *env, jobject, jint param) {
       int result = your_c_function(param);
       return static_cast<jint>(result);
   }
   ```

3. **在 RwkvMobile.kt 中声明**:
   ```kotlin
   @JvmStatic
   external fun your_c_function(param: Int): Int
   ```

4. **重新编译即可**

## 库函数参考

查看 `librwkv_mobile.so` 导出的所有函数:
```bash
nm -D app/src/main/jniLibs/arm64-v8a/librwkv_mobile.so | grep " T "
```

主要 API 包括:
- 模型加载: `rwkvmobile_runtime_load_model*`
- 推理: `rwkvmobile_runtime_gen_completion*`, `infer*`
- 状态管理: `rwkvmobile_runtime_*_state`
- 采样器: `rwkvmobile_runtime_*_sampler_params`
- Vision: `rwkvmobile_runtime_load_vision_encoder`
- 音频: `rwkvmobile_runtime_load_whisper_encoder`

## 许可证

参考原始 RWKV Mobile 库的许可证。

