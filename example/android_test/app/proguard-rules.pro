# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.

# Keep the native methods
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep RwkvMobile class
-keep class com.example.rwkvmobiletest.RwkvMobile { *; }

