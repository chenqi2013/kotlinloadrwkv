cmake_minimum_required(VERSION 3.22.1)

project("rwkvmobiletest")

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加 JNI 桥接库
add_library(rwkv_jni SHARED
        rwkv_jni.cpp)

# 查找 Android log 库
find_library(log-lib log)

# 查找 Android 库
find_library(android-lib android)

# 设置 librwkv_mobile.so 的路径
set(RWKV_MOBILE_LIB_DIR ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

# 导入预编译的 librwkv_mobile.so
add_library(rwkv_mobile SHARED IMPORTED)
set_target_properties(rwkv_mobile PROPERTIES
        IMPORTED_LOCATION ${RWKV_MOBILE_LIB_DIR}/librwkv_mobile.so)

# 链接库
target_link_libraries(rwkv_jni
        ${log-lib}
        ${android-lib}
        rwkv_mobile)

# 设置编译选项
target_compile_options(rwkv_jni PRIVATE
        -Wall
        -Wextra
        -fvisibility=hidden)
