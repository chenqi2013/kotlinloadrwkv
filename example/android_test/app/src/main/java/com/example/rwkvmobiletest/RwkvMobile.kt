package com.example.rwkvmobiletest

/**
 * JNI wrapper for librwkv_mobile.so
 * 
 * This class provides Kotlin bindings to the native RWKV Mobile library.
 * The native library is loaded when this class is first accessed.
 * 
 * Architecture:
 * - librwkv_jni.so: JNI bridge library (built from src/main/cpp/)
 * - librwkv_mobile.so: The actual RWKV mobile inference library
 * 
 * The JNI bridge library wraps the C functions from librwkv_mobile.so
 * and exposes them as JNI-compatible functions.
 */
object RwkvMobile {
    
    private var isLoaded = false
    private var loadError: String? = null
    
    // Load the native libraries
    init {
        try {
            // First load the main RWKV mobile library
            System.loadLibrary("rwkv_mobile")
            println("RwkvMobile: Successfully loaded librwkv_mobile.so")
            
            // Then load our JNI bridge library
            System.loadLibrary("rwkv_jni")
            println("RwkvMobile: Successfully loaded librwkv_jni.so")
            
            isLoaded = true
        } catch (e: UnsatisfiedLinkError) {
            isLoaded = false
            loadError = e.message
            println("RwkvMobile: Failed to load native libraries - ${e.message}")
            e.printStackTrace()
        }
    }

    /**
     * Check if the native library was loaded successfully
     */
    fun isLibraryLoaded(): Boolean = isLoaded
    
    /**
     * Get the error message if library loading failed
     */
    fun getLoadError(): String? = loadError

    // ========================================================================
    // Device Information Functions
    // ========================================================================

    /**
     * Get platform name (e.g., "Android")
     */
    @JvmStatic
    external fun rwkvmobile_get_platform_name(): String?

    /**
     * Get SoC (System on Chip) name
     */
    @JvmStatic
    external fun rwkvmobile_get_soc_name(): String?

    /**
     * Get SoC part name
     */
    @JvmStatic
    external fun rwkvmobile_get_soc_partname(): String?

    /**
     * Get HTP (Hexagon Tensor Processor) architecture
     */
    @JvmStatic
    external fun rwkvmobile_get_htp_arch(): String?

    // ========================================================================
    // Logging Functions
    // ========================================================================

    /**
     * Dump log from the native library
     */
    @JvmStatic
    external fun rwkvmobile_dump_log(): String?

    /**
     * Set log level
     * @param level Log level (0=DEBUG, 1=INFO, 2=WARN, 3=ERROR)
     */
    @JvmStatic
    external fun rwkvmobile_set_loglevel(level: Int)

    /**
     * Set cache directory
     * @param path Cache directory path
     */
    @JvmStatic
    external fun rwkvmobile_set_cache_dir(path: String)

    // ========================================================================
    // Runtime Lifecycle Functions
    // ========================================================================

    /**
     * Initialize the RWKV runtime
     * @return Runtime handle (pointer as Long), or 0 on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_init(): Long

    /**
     * Release the RWKV runtime
     * @param runtime Runtime handle from rwkvmobile_runtime_init()
     * @return 0 on success, negative value on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_release(runtime: Long): Int

    // ========================================================================
    // Backend Functions
    // ========================================================================

    /**
     * Get available backend names
     * @param buffer Byte array to receive backend names
     * @param bufferSize Size of the buffer
     * @return Number of bytes written, or negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_get_available_backend_names(buffer: ByteArray, bufferSize: Int): Int

    /**
     * Set QNN library path
     * @param runtime Runtime handle
     * @param path Path to QNN libraries
     * @return 0 on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_set_qnn_library_path(runtime: Long, path: String): Int

    /**
     * Add ADSP library path
     * @param runtime Runtime handle
     * @param path Path to ADSP libraries
     * @return 0 on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_add_adsp_library_path(runtime: Long, path: String): Int

    // ========================================================================
    // Model Loading Functions
    // ========================================================================

    /**
     * Load a model
     * @param runtime Runtime handle
     * @param modelPath Path to the model file
     * @param backendName Backend name (e.g., "cpu", "gpu", "npu")
     * @return Model ID (>=0) on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_load_model(runtime: Long, modelPath: String, backendName: String): Int

    /**
     * Load a model with extra parameters
     * @param runtime Runtime handle
     * @param modelPath Path to the model file
     * @param backendName Backend name
     * @param extraParams Extra parameters (JSON or implementation specific)
     * @return Model ID (>=0) on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_load_model_with_extra(
        runtime: Long, 
        modelPath: String, 
        backendName: String, 
        extraParams: String?
    ): Int

    /**
     * Release a loaded model
     * @param runtime Runtime handle
     * @param modelId Model ID
     * @return 0 on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_release_model(runtime: Long, modelId: Int): Int

    // ========================================================================
    // State Management Functions
    // ========================================================================

    /**
     * Clear the model state
     * @param runtime Runtime handle
     * @return 0 on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_clear_state(runtime: Long): Int

    /**
     * Load initial state from file
     * @param runtime Runtime handle
     * @param statePath Path to the state file
     * @return 0 on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_load_initial_state(runtime: Long, statePath: String): Int

    /**
     * Unload initial state
     * @param runtime Runtime handle
     * @return 0 on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_unload_initial_state(runtime: Long): Int

    // ========================================================================
    // Generation Control Functions
    // ========================================================================

    /**
     * Check if generation is in progress
     * @param runtime Runtime handle
     * @return 1 if generating, 0 if not
     */
    @JvmStatic
    external fun rwkvmobile_runtime_is_generating(runtime: Long): Int

    /**
     * Stop ongoing generation
     * @param runtime Runtime handle
     * @return 0 on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_stop_generation(runtime: Long): Int

    /**
     * Generate completion synchronously
     * @param runtime Runtime handle
     * @param prompt Input prompt
     * @param maxTokens Maximum tokens to generate
     * @return Generated text, or null on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_gen_completion(runtime: Long, prompt: String, maxTokens: Int): String?

    /**
     * Get response buffer content
     * @param runtime Runtime handle
     * @return Response content string
     */
    @JvmStatic
    external fun rwkvmobile_runtime_get_response_buffer_content(runtime: Long): String?

    // ========================================================================
    // Sampler Parameters
    // ========================================================================

    /**
     * Set sampler parameters
     * @param runtime Runtime handle
     * @param temperature Temperature for sampling (higher = more random)
     * @param topP Top-p (nucleus) sampling threshold
     * @param topK Top-k sampling (0 to disable)
     * @return 0 on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_set_sampler_params(
        runtime: Long, 
        temperature: Float, 
        topP: Float, 
        topK: Int
    ): Int

    /**
     * Get sampler parameters
     * @param runtime Runtime handle
     * @return Float array [temperature, topP, topK] or null on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_get_sampler_params(runtime: Long): FloatArray?

    // ========================================================================
    // Prompt/Chat Functions
    // ========================================================================

    /**
     * Set the prompt
     * @param runtime Runtime handle
     * @param prompt Prompt text
     * @return 0 on success, negative on error
     */
    @JvmStatic
    external fun rwkvmobile_runtime_set_prompt(runtime: Long, prompt: String): Int

    /**
     * Get the current prompt
     * @param runtime Runtime handle
     * @return Current prompt string
     */
    @JvmStatic
    external fun rwkvmobile_runtime_get_prompt(runtime: Long): String?

    /**
     * Set BOS (Beginning of Sequence) token
     * @param runtime Runtime handle
     * @param token BOS token string
     * @return 0 on success
     */
    @JvmStatic
    external fun rwkvmobile_runtime_set_bos_token(runtime: Long, token: String): Int

    /**
     * Set EOS (End of Sequence) token
     * @param runtime Runtime handle
     * @param token EOS token string
     * @return 0 on success
     */
    @JvmStatic
    external fun rwkvmobile_runtime_set_eos_token(runtime: Long, token: String): Int

    /**
     * Set user role for chat formatting
     * @param runtime Runtime handle
     * @param role User role string (e.g., "User")
     * @return 0 on success
     */
    @JvmStatic
    external fun rwkvmobile_runtime_set_user_role(runtime: Long, role: String): Int

    /**
     * Set response/assistant role for chat formatting
     * @param runtime Runtime handle
     * @param role Response role string (e.g., "Assistant")
     * @return 0 on success
     */
    @JvmStatic
    external fun rwkvmobile_runtime_set_response_role(runtime: Long, role: String): Int

    // ========================================================================
    // Speed/Performance Metrics
    // ========================================================================

    /**
     * Get average decode speed
     * @param runtime Runtime handle
     * @return Decode speed in tokens per second
     */
    @JvmStatic
    external fun rwkvmobile_runtime_get_avg_decode_speed(runtime: Long): Float

    /**
     * Get average prefill speed
     * @param runtime Runtime handle
     * @return Prefill speed in tokens per second
     */
    @JvmStatic
    external fun rwkvmobile_runtime_get_avg_prefill_speed(runtime: Long): Float

    /**
     * Get prefill progress (0.0 to 1.0)
     * @param runtime Runtime handle
     * @return Progress value
     */
    @JvmStatic
    external fun rwkvmobile_runtime_get_prefill_progress(runtime: Long): Float

    // ========================================================================
    // Seed Functions
    // ========================================================================

    /**
     * Set random seed for reproducible generation
     * @param runtime Runtime handle
     * @param seed Seed value
     * @return 0 on success
     */
    @JvmStatic
    external fun rwkvmobile_runtime_set_seed(runtime: Long, seed: Long): Int

    /**
     * Get current random seed
     * @param runtime Runtime handle
     * @return Current seed value
     */
    @JvmStatic
    external fun rwkvmobile_runtime_get_seed(runtime: Long): Long

    // ========================================================================
    // Constants
    // ========================================================================

    const val LOG_LEVEL_DEBUG = 0
    const val LOG_LEVEL_INFO = 1
    const val LOG_LEVEL_WARN = 2
    const val LOG_LEVEL_ERROR = 3
    
    // ========================================================================
    // Kotlin-friendly Helper Functions
    // ========================================================================
    
    /**
     * Get available backend names as a String
     */
    fun getAvailableBackendNames(bufferSize: Int = 1024): String? {
        return try {
            val buffer = ByteArray(bufferSize)
            val result = rwkvmobile_runtime_get_available_backend_names(buffer, bufferSize)
            if (result >= 0) {
                String(buffer, 0, result, Charsets.UTF_8)
            } else {
                null
            }
        } catch (e: Exception) {
            e.printStackTrace()
            null
        }
    }

    /**
     * Get device information as a formatted string
     */
    fun getDeviceInfo(): String {
        return buildString {
            appendLine("Platform: ${rwkvmobile_get_platform_name() ?: "Unknown"}")
            appendLine("SoC: ${rwkvmobile_get_soc_name() ?: "Unknown"}")
            appendLine("SoC Part: ${rwkvmobile_get_soc_partname() ?: "Unknown"}")
            appendLine("HTP Arch: ${rwkvmobile_get_htp_arch() ?: "Unknown"}")
            appendLine("Available Backends: ${getAvailableBackendNames() ?: "Unknown"}")
        }
    }

    /**
     * Data class for sampler parameters
     */
    data class SamplerParams(
        val temperature: Float,
        val topP: Float,
        val topK: Int
    )

    /**
     * Get sampler parameters as a data class
     */
    fun getSamplerParamsAsObject(runtime: Long): SamplerParams? {
        val params = rwkvmobile_runtime_get_sampler_params(runtime) ?: return null
        if (params.size < 3) return null
        return SamplerParams(
            temperature = params[0],
            topP = params[1],
            topK = params[2].toInt()
        )
    }
}
