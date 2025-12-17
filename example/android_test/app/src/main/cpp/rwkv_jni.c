#include <jni.h>
#include <string.h>
#include <stdlib.h>

// Forward declarations of rwkv_mobile functions from librwkv_mobile.so
// These are the C functions exported by the library

// Device/Platform info
extern const char* rwkvmobile_get_platform_name(void);
extern const char* rwkvmobile_get_soc_name(void);
extern const char* rwkvmobile_get_soc_partname(void);
extern const char* rwkvmobile_get_htp_arch(void);
extern const char* rwkvmobile_dump_log(void);
extern void rwkvmobile_set_loglevel(int level);
extern void rwkvmobile_set_cache_dir(const char* path);

// Runtime management
extern void* rwkvmobile_runtime_init(void);
extern int rwkvmobile_runtime_release(void* runtime);
extern int rwkvmobile_runtime_get_available_backend_names(char* buffer, int buffer_size);

// Model loading
extern int rwkvmobile_runtime_load_model(void* runtime, const char* model_path, const char* backend_name);
extern int rwkvmobile_runtime_load_model_with_extra(void* runtime, const char* model_path, const char* backend_name, const char* extra);
extern int rwkvmobile_runtime_release_model(void* runtime);

// State management
extern int rwkvmobile_runtime_clear_state(void* runtime);
extern int rwkvmobile_runtime_load_initial_state(void* runtime, const char* state_path);
extern int rwkvmobile_runtime_unload_initial_state(void* runtime);

// Prompt/Generation
extern int rwkvmobile_runtime_set_prompt(void* runtime, const char* prompt);
extern const char* rwkvmobile_runtime_get_prompt(void* runtime);
extern int rwkvmobile_runtime_gen_completion(void* runtime, char* buffer, int buffer_size);
extern int rwkvmobile_runtime_gen_completion_async(void* runtime);
extern int rwkvmobile_runtime_stop_generation(void* runtime);
extern int rwkvmobile_runtime_is_generating(void* runtime);

// Response buffer
extern const char* rwkvmobile_runtime_get_response_buffer_content(void* runtime);
extern void rwkvmobile_runtime_free_response_buffer(void* runtime);

// Sampler params
extern int rwkvmobile_runtime_set_sampler_params(void* runtime, float temperature, float top_p, float presence_penalty, float frequency_penalty);
extern int rwkvmobile_runtime_get_sampler_params(void* runtime, float* temperature, float* top_p, float* presence_penalty, float* frequency_penalty);

// Penalty params
extern int rwkvmobile_runtime_set_penalty_params(void* runtime, float presence_penalty, float frequency_penalty, float penalty_decay);
extern int rwkvmobile_runtime_get_penalty_params(void* runtime, float* presence_penalty, float* frequency_penalty, float* penalty_decay);

// Chat/Roles
extern int rwkvmobile_runtime_set_user_role(void* runtime, const char* role);
extern int rwkvmobile_runtime_set_response_role(void* runtime, const char* role);
extern int rwkvmobile_runtime_set_bos_token(void* runtime, const char* token);
extern int rwkvmobile_runtime_set_eos_token(void* runtime, const char* token);

// Chat with history
extern int rwkvmobile_runtime_eval_chat_with_history_async(void* runtime, const char* user_input);

// Speed stats
extern float rwkvmobile_runtime_get_avg_prefill_speed(void* runtime);
extern float rwkvmobile_runtime_get_avg_decode_speed(void* runtime);

// Seed
extern void rwkvmobile_runtime_set_seed(void* runtime, unsigned int seed);
extern unsigned int rwkvmobile_runtime_get_seed(void* runtime);

// Prefill progress
extern float rwkvmobile_runtime_get_prefill_progress(void* runtime);

// ============================================================================
// JNI Implementation
// ============================================================================

// Helper to create Java String from C string (handles NULL)
static jstring createJString(JNIEnv *env, const char* str) {
    if (str == NULL) {
        return NULL;
    }
    return (*env)->NewStringUTF(env, str);
}

// ============================================================================
// Device/Platform Info
// ============================================================================

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1get_1platform_1name(JNIEnv *env, jclass clazz) {
    const char* result = rwkvmobile_get_platform_name();
    return createJString(env, result);
}

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1get_1soc_1name(JNIEnv *env, jclass clazz) {
    const char* result = rwkvmobile_get_soc_name();
    return createJString(env, result);
}

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1get_1soc_1partname(JNIEnv *env, jclass clazz) {
    const char* result = rwkvmobile_get_soc_partname();
    return createJString(env, result);
}

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1get_1htp_1arch(JNIEnv *env, jclass clazz) {
    const char* result = rwkvmobile_get_htp_arch();
    return createJString(env, result);
}

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1dump_1log(JNIEnv *env, jclass clazz) {
    const char* result = rwkvmobile_dump_log();
    return createJString(env, result);
}

JNIEXPORT void JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1set_1loglevel(JNIEnv *env, jclass clazz, jint level) {
    rwkvmobile_set_loglevel((int)level);
}

JNIEXPORT void JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1set_1cache_1dir(JNIEnv *env, jclass clazz, jstring path) {
    const char* pathStr = (*env)->GetStringUTFChars(env, path, NULL);
    rwkvmobile_set_cache_dir(pathStr);
    (*env)->ReleaseStringUTFChars(env, path, pathStr);
}

// ============================================================================
// Runtime Management
// ============================================================================

JNIEXPORT jlong JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1init(JNIEnv *env, jclass clazz) {
    void* runtime = rwkvmobile_runtime_init();
    return (jlong)(intptr_t)runtime;
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1release(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_release((void*)(intptr_t)runtime);
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1available_1backend_1names(
        JNIEnv *env, jclass clazz, jbyteArray buffer, jint bufferSize) {
    jbyte* bufferPtr = (*env)->GetByteArrayElements(env, buffer, NULL);
    int result = rwkvmobile_runtime_get_available_backend_names((char*)bufferPtr, (int)bufferSize);
    (*env)->ReleaseByteArrayElements(env, buffer, bufferPtr, 0);
    return result;
}

// ============================================================================
// Model Loading
// ============================================================================

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1load_1model(
        JNIEnv *env, jclass clazz, jlong runtime, jstring modelPath, jstring backendName) {
    const char* modelPathStr = (*env)->GetStringUTFChars(env, modelPath, NULL);
    const char* backendNameStr = (*env)->GetStringUTFChars(env, backendName, NULL);
    
    int result = rwkvmobile_runtime_load_model((void*)(intptr_t)runtime, modelPathStr, backendNameStr);
    
    (*env)->ReleaseStringUTFChars(env, modelPath, modelPathStr);
    (*env)->ReleaseStringUTFChars(env, backendName, backendNameStr);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1load_1model_1with_1extra(
        JNIEnv *env, jclass clazz, jlong runtime, jstring modelPath, jstring backendName, jstring extra) {
    const char* modelPathStr = (*env)->GetStringUTFChars(env, modelPath, NULL);
    const char* backendNameStr = (*env)->GetStringUTFChars(env, backendName, NULL);
    const char* extraStr = extra ? (*env)->GetStringUTFChars(env, extra, NULL) : NULL;
    
    int result = rwkvmobile_runtime_load_model_with_extra((void*)(intptr_t)runtime, modelPathStr, backendNameStr, extraStr);
    
    (*env)->ReleaseStringUTFChars(env, modelPath, modelPathStr);
    (*env)->ReleaseStringUTFChars(env, backendName, backendNameStr);
    if (extraStr) (*env)->ReleaseStringUTFChars(env, extra, extraStr);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1release_1model(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_release_model((void*)(intptr_t)runtime);
}

// ============================================================================
// State Management
// ============================================================================

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1clear_1state(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_clear_state((void*)(intptr_t)runtime);
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1load_1initial_1state(
        JNIEnv *env, jclass clazz, jlong runtime, jstring statePath) {
    const char* statePathStr = (*env)->GetStringUTFChars(env, statePath, NULL);
    int result = rwkvmobile_runtime_load_initial_state((void*)(intptr_t)runtime, statePathStr);
    (*env)->ReleaseStringUTFChars(env, statePath, statePathStr);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1unload_1initial_1state(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_unload_initial_state((void*)(intptr_t)runtime);
}

// ============================================================================
// Prompt/Generation
// ============================================================================

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1set_1prompt(
        JNIEnv *env, jclass clazz, jlong runtime, jstring prompt) {
    const char* promptStr = (*env)->GetStringUTFChars(env, prompt, NULL);
    int result = rwkvmobile_runtime_set_prompt((void*)(intptr_t)runtime, promptStr);
    (*env)->ReleaseStringUTFChars(env, prompt, promptStr);
    return result;
}

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1prompt(JNIEnv *env, jclass clazz, jlong runtime) {
    const char* result = rwkvmobile_runtime_get_prompt((void*)(intptr_t)runtime);
    return createJString(env, result);
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1gen_1completion(
        JNIEnv *env, jclass clazz, jlong runtime, jbyteArray buffer, jint bufferSize) {
    jbyte* bufferPtr = (*env)->GetByteArrayElements(env, buffer, NULL);
    int result = rwkvmobile_runtime_gen_completion((void*)(intptr_t)runtime, (char*)bufferPtr, (int)bufferSize);
    (*env)->ReleaseByteArrayElements(env, buffer, bufferPtr, 0);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1gen_1completion_1async(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_gen_completion_async((void*)(intptr_t)runtime);
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1stop_1generation(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_stop_generation((void*)(intptr_t)runtime);
}

JNIEXPORT jboolean JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1is_1generating(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_is_generating((void*)(intptr_t)runtime) != 0;
}

// ============================================================================
// Response Buffer
// ============================================================================

JNIEXPORT jstring JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1response_1buffer_1content(JNIEnv *env, jclass clazz, jlong runtime) {
    const char* result = rwkvmobile_runtime_get_response_buffer_content((void*)(intptr_t)runtime);
    return createJString(env, result);
}

JNIEXPORT void JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1free_1response_1buffer(JNIEnv *env, jclass clazz, jlong runtime) {
    rwkvmobile_runtime_free_response_buffer((void*)(intptr_t)runtime);
}

// ============================================================================
// Sampler Params
// ============================================================================

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1set_1sampler_1params(
        JNIEnv *env, jclass clazz, jlong runtime, 
        jfloat temperature, jfloat topP, jfloat presencePenalty, jfloat frequencyPenalty) {
    return rwkvmobile_runtime_set_sampler_params(
        (void*)(intptr_t)runtime, 
        (float)temperature, (float)topP, (float)presencePenalty, (float)frequencyPenalty
    );
}

JNIEXPORT jfloatArray JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1sampler_1params(JNIEnv *env, jclass clazz, jlong runtime) {
    float temperature, topP, presencePenalty, frequencyPenalty;
    int result = rwkvmobile_runtime_get_sampler_params(
        (void*)(intptr_t)runtime, 
        &temperature, &topP, &presencePenalty, &frequencyPenalty
    );
    
    if (result < 0) return NULL;
    
    jfloatArray params = (*env)->NewFloatArray(env, 4);
    jfloat values[4] = {temperature, topP, presencePenalty, frequencyPenalty};
    (*env)->SetFloatArrayRegion(env, params, 0, 4, values);
    return params;
}

// ============================================================================
// Penalty Params
// ============================================================================

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1set_1penalty_1params(
        JNIEnv *env, jclass clazz, jlong runtime, 
        jfloat presencePenalty, jfloat frequencyPenalty, jfloat penaltyDecay) {
    return rwkvmobile_runtime_set_penalty_params(
        (void*)(intptr_t)runtime, 
        (float)presencePenalty, (float)frequencyPenalty, (float)penaltyDecay
    );
}

JNIEXPORT jfloatArray JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1penalty_1params(JNIEnv *env, jclass clazz, jlong runtime) {
    float presencePenalty, frequencyPenalty, penaltyDecay;
    int result = rwkvmobile_runtime_get_penalty_params(
        (void*)(intptr_t)runtime, 
        &presencePenalty, &frequencyPenalty, &penaltyDecay
    );
    
    if (result < 0) return NULL;
    
    jfloatArray params = (*env)->NewFloatArray(env, 3);
    jfloat values[3] = {presencePenalty, frequencyPenalty, penaltyDecay};
    (*env)->SetFloatArrayRegion(env, params, 0, 3, values);
    return params;
}

// ============================================================================
// Chat/Roles
// ============================================================================

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1set_1user_1role(
        JNIEnv *env, jclass clazz, jlong runtime, jstring role) {
    const char* roleStr = (*env)->GetStringUTFChars(env, role, NULL);
    int result = rwkvmobile_runtime_set_user_role((void*)(intptr_t)runtime, roleStr);
    (*env)->ReleaseStringUTFChars(env, role, roleStr);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1set_1response_1role(
        JNIEnv *env, jclass clazz, jlong runtime, jstring role) {
    const char* roleStr = (*env)->GetStringUTFChars(env, role, NULL);
    int result = rwkvmobile_runtime_set_response_role((void*)(intptr_t)runtime, roleStr);
    (*env)->ReleaseStringUTFChars(env, role, roleStr);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1set_1bos_1token(
        JNIEnv *env, jclass clazz, jlong runtime, jstring token) {
    const char* tokenStr = (*env)->GetStringUTFChars(env, token, NULL);
    int result = rwkvmobile_runtime_set_bos_token((void*)(intptr_t)runtime, tokenStr);
    (*env)->ReleaseStringUTFChars(env, token, tokenStr);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1set_1eos_1token(
        JNIEnv *env, jclass clazz, jlong runtime, jstring token) {
    const char* tokenStr = (*env)->GetStringUTFChars(env, token, NULL);
    int result = rwkvmobile_runtime_set_eos_token((void*)(intptr_t)runtime, tokenStr);
    (*env)->ReleaseStringUTFChars(env, token, tokenStr);
    return result;
}

// ============================================================================
// Chat with History
// ============================================================================

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1eval_1chat_1with_1history_1async(
        JNIEnv *env, jclass clazz, jlong runtime, jstring userInput) {
    const char* inputStr = (*env)->GetStringUTFChars(env, userInput, NULL);
    int result = rwkvmobile_runtime_eval_chat_with_history_async((void*)(intptr_t)runtime, inputStr);
    (*env)->ReleaseStringUTFChars(env, userInput, inputStr);
    return result;
}

// ============================================================================
// Speed Stats
// ============================================================================

JNIEXPORT jfloat JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1avg_1prefill_1speed(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_get_avg_prefill_speed((void*)(intptr_t)runtime);
}

JNIEXPORT jfloat JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1avg_1decode_1speed(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_get_avg_decode_speed((void*)(intptr_t)runtime);
}

// ============================================================================
// Seed
// ============================================================================

JNIEXPORT void JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1set_1seed(JNIEnv *env, jclass clazz, jlong runtime, jint seed) {
    rwkvmobile_runtime_set_seed((void*)(intptr_t)runtime, (unsigned int)seed);
}

JNIEXPORT jint JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1seed(JNIEnv *env, jclass clazz, jlong runtime) {
    return (jint)rwkvmobile_runtime_get_seed((void*)(intptr_t)runtime);
}

// ============================================================================
// Prefill Progress
// ============================================================================

JNIEXPORT jfloat JNICALL
Java_com_example_rwkvmobiletest_RwkvMobile_rwkvmobile_1runtime_1get_1prefill_1progress(JNIEnv *env, jclass clazz, jlong runtime) {
    return rwkvmobile_runtime_get_prefill_progress((void*)(intptr_t)runtime);
}
