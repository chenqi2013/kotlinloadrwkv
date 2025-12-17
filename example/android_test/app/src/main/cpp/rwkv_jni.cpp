#include <jni.h>
#include <string>
#include <cstring>
#include <android/log.h>

#define LOG_TAG "RWKV_JNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// 声明 librwkv_mobile.so 中的 C 函数
extern "C" {
    // 设备信息
    char* rwkvmobile_get_platform_name();
    char* rwkvmobile_get_soc_name();
    char* rwkvmobile_get_soc_partname();
    char* rwkvmobile_get_htp_arch();
    char* rwkvmobile_dump_log();
    
    // 日志
    void rwkvmobile_set_loglevel(int loglevel);
    
    // Runtime API
    typedef void* rwkvmobile_runtime_t;
    rwkvmobile_runtime_t rwkvmobile_runtime_init();
    int rwkvmobile_runtime_release(rwkvmobile_runtime_t runtime);
    int rwkvmobile_runtime_get_available_backend_names(char* buffer, int buffer_size);
}

// JNI 函数实现
extern "C" {

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1get_1platform_1name(
        JNIEnv *env, jobject /* this */) {
    char* result = rwkvmobile_get_platform_name();
    if (result == nullptr) {
        return env->NewStringUTF("Unknown");
    }
    jstring jstr = env->NewStringUTF(result);
    // Note: 根据库的实现，可能需要 free(result) 或不需要
    return jstr;
}

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1get_1soc_1name(
        JNIEnv *env, jobject /* this */) {
    char* result = rwkvmobile_get_soc_name();
    if (result == nullptr) {
        return env->NewStringUTF("Unknown");
    }
    jstring jstr = env->NewStringUTF(result);
    return jstr;
}

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1get_1soc_1partname(
        JNIEnv *env, jobject /* this */) {
    char* result = rwkvmobile_get_soc_partname();
    if (result == nullptr) {
        return env->NewStringUTF("Unknown");
    }
    jstring jstr = env->NewStringUTF(result);
    return jstr;
}

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1get_1htp_1arch(
        JNIEnv *env, jobject /* this */) {
    char* result = rwkvmobile_get_htp_arch();
    if (result == nullptr) {
        return env->NewStringUTF("Unknown");
    }
    jstring jstr = env->NewStringUTF(result);
    return jstr;
}

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1dump_1log(
        JNIEnv *env, jobject /* this */) {
    char* result = rwkvmobile_dump_log();
    if (result == nullptr) {
        return env->NewStringUTF("");
    }
    jstring jstr = env->NewStringUTF(result);
    return jstr;
}

JNIEXPORT void JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1set_1loglevel(
        JNIEnv *env, jobject /* this */, jint level) {
    rwkvmobile_set_loglevel(static_cast<int>(level));
}

JNIEXPORT jlong JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1init(
        JNIEnv *env, jobject /* this */) {
    LOGI("Calling rwkvmobile_runtime_init");
    rwkvmobile_runtime_t runtime = rwkvmobile_runtime_init();
    LOGI("Runtime initialized: %p", runtime);
    return reinterpret_cast<jlong>(runtime);
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1release(
        JNIEnv *env, jobject /* this */, jlong runtime) {
    LOGI("Calling rwkvmobile_runtime_release with handle: %p", 
         reinterpret_cast<void*>(runtime));
    rwkvmobile_runtime_t rt = reinterpret_cast<rwkvmobile_runtime_t>(runtime);
    int result = rwkvmobile_runtime_release(rt);
    LOGI("Runtime released, result: %d", result);
    return static_cast<jint>(result);
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1available_1backend_1names(
        JNIEnv *env, jobject /* this */, jbyteArray buffer, jint bufferSize) {
    LOGI("Getting available backend names, buffer size: %d", bufferSize);
    
    if (buffer == nullptr) {
        LOGE("Buffer is null");
        return -1;
    }
    
    // 获取 byte array 的指针
    jbyte* bufferPtr = env->GetByteArrayElements(buffer, nullptr);
    if (bufferPtr == nullptr) {
        LOGE("Failed to get buffer pointer");
        return -2;
    }
    
    // 调用原生函数
    int result = rwkvmobile_runtime_get_available_backend_names(
        reinterpret_cast<char*>(bufferPtr), 
        static_cast<int>(bufferSize)
    );
    
    LOGI("Backend names result: %d", result);
    
    // 释放 byte array
    env->ReleaseByteArrayElements(buffer, bufferPtr, 0);
    
    return static_cast<jint>(result);
}

} // extern "C"

