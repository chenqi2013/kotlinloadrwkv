/**
 * rwkv_mobile.h
 * 
 * Header file declaring the C API functions exported by librwkv_mobile.so
 * This header is used by the JNI bridge to call the native library functions.
 */

#ifndef RWKV_MOBILE_H
#define RWKV_MOBILE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque handle type for the runtime
typedef void* rwkvmobile_runtime_t;

// ============================================================================
// Device Information Functions
// ============================================================================

/**
 * Get the platform name (e.g., "Android")
 * @return Platform name string (caller should NOT free this)
 */
const char* rwkvmobile_get_platform_name(void);

/**
 * Get the SoC (System on Chip) name
 * @return SoC name string
 */
const char* rwkvmobile_get_soc_name(void);

/**
 * Get the SoC part name
 * @return SoC part name string
 */
const char* rwkvmobile_get_soc_partname(void);

/**
 * Get the HTP (Hexagon Tensor Processor) architecture
 * @return HTP architecture string
 */
const char* rwkvmobile_get_htp_arch(void);

// ============================================================================
// Logging Functions
// ============================================================================

/**
 * Dump the log buffer
 * @return Log content string
 */
const char* rwkvmobile_dump_log(void);

/**
 * Set the log level
 * @param loglevel Log level (0=DEBUG, 1=INFO, 2=WARN, 3=ERROR)
 */
void rwkvmobile_set_loglevel(int loglevel);

/**
 * Set the cache directory
 * @param path Cache directory path
 */
void rwkvmobile_set_cache_dir(const char* path);

// ============================================================================
// Runtime Lifecycle Functions
// ============================================================================

/**
 * Initialize the RWKV runtime
 * @return Runtime handle, or NULL on error
 */
rwkvmobile_runtime_t rwkvmobile_runtime_init(void);

/**
 * Release the RWKV runtime and free all resources
 * @param runtime Runtime handle
 * @return 0 on success, negative value on error
 */
int rwkvmobile_runtime_release(rwkvmobile_runtime_t runtime);

// ============================================================================
// Backend Functions
// ============================================================================

/**
 * Get available backend names
 * @param buffer Buffer to receive the backend names (comma-separated)
 * @param buffer_size Size of the buffer
 * @return Number of bytes written, or negative on error
 */
int rwkvmobile_runtime_get_available_backend_names(char* buffer, int buffer_size);

/**
 * Set QNN library path
 * @param runtime Runtime handle
 * @param path Path to QNN libraries
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_set_qnn_library_path(rwkvmobile_runtime_t runtime, const char* path);

/**
 * Add ADSP library path
 * @param runtime Runtime handle
 * @param path Path to ADSP libraries
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_add_adsp_library_path(rwkvmobile_runtime_t runtime, const char* path);

// ============================================================================
// Model Loading Functions
// ============================================================================

/**
 * Load a model
 * @param runtime Runtime handle
 * @param model_path Path to the model file
 * @param backend_name Backend name (e.g., "cpu", "gpu", "npu")
 * @return Model ID (>=0) on success, negative on error
 */
int rwkvmobile_runtime_load_model(rwkvmobile_runtime_t runtime, 
                                   const char* model_path,
                                   const char* backend_name);

/**
 * Load a model with extra parameters
 * @param runtime Runtime handle
 * @param model_path Path to the model file
 * @param backend_name Backend name
 * @param extra_params Extra parameters (implementation specific)
 * @return Model ID (>=0) on success, negative on error
 */
int rwkvmobile_runtime_load_model_with_extra(rwkvmobile_runtime_t runtime,
                                              const char* model_path,
                                              const char* backend_name,
                                              const char* extra_params);

/**
 * Release a loaded model
 * @param runtime Runtime handle
 * @param model_id Model ID
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_release_model(rwkvmobile_runtime_t runtime, int model_id);

// ============================================================================
// State Management Functions
// ============================================================================

/**
 * Clear the model state
 * @param runtime Runtime handle
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_clear_state(rwkvmobile_runtime_t runtime);

/**
 * Load initial state
 * @param runtime Runtime handle
 * @param state_path Path to the state file
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_load_initial_state(rwkvmobile_runtime_t runtime, const char* state_path);

/**
 * Unload initial state
 * @param runtime Runtime handle
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_unload_initial_state(rwkvmobile_runtime_t runtime);

// ============================================================================
// Generation Functions
// ============================================================================

/**
 * Check if generation is in progress
 * @param runtime Runtime handle
 * @return 1 if generating, 0 if not
 */
int rwkvmobile_runtime_is_generating(rwkvmobile_runtime_t runtime);

/**
 * Stop ongoing generation
 * @param runtime Runtime handle
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_stop_generation(rwkvmobile_runtime_t runtime);

/**
 * Generate completion synchronously
 * @param runtime Runtime handle
 * @param prompt Input prompt
 * @param max_tokens Maximum tokens to generate
 * @return Generated text (caller should free with rwkvmobile_runtime_free_response_buffer)
 */
const char* rwkvmobile_runtime_gen_completion(rwkvmobile_runtime_t runtime,
                                               const char* prompt,
                                               int max_tokens);

/**
 * Free response buffer
 * @param buffer Buffer to free
 */
void rwkvmobile_runtime_free_response_buffer(char* buffer);

/**
 * Get response buffer content
 * @param runtime Runtime handle
 * @return Response content string
 */
const char* rwkvmobile_runtime_get_response_buffer_content(rwkvmobile_runtime_t runtime);

// ============================================================================
// Async Generation Functions (callbacks)
// ============================================================================

// Callback function type for token generation
typedef void (*rwkvmobile_token_callback_t)(const char* token, void* user_data);
typedef void (*rwkvmobile_completion_callback_t)(int status, void* user_data);

/**
 * Generate completion asynchronously
 * @param runtime Runtime handle
 * @param prompt Input prompt
 * @param max_tokens Maximum tokens to generate
 * @param token_callback Callback for each generated token
 * @param completion_callback Callback when generation completes
 * @param user_data User data passed to callbacks
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_gen_completion_async(rwkvmobile_runtime_t runtime,
                                             const char* prompt,
                                             int max_tokens,
                                             rwkvmobile_token_callback_t token_callback,
                                             rwkvmobile_completion_callback_t completion_callback,
                                             void* user_data);

// ============================================================================
// Sampler Parameters
// ============================================================================

/**
 * Set sampler parameters
 * @param runtime Runtime handle
 * @param temperature Temperature for sampling
 * @param top_p Top-p (nucleus) sampling
 * @param top_k Top-k sampling
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_set_sampler_params(rwkvmobile_runtime_t runtime,
                                           float temperature,
                                           float top_p,
                                           int top_k);

/**
 * Get sampler parameters
 * @param runtime Runtime handle
 * @param temperature Output: temperature
 * @param top_p Output: top_p
 * @param top_k Output: top_k
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_get_sampler_params(rwkvmobile_runtime_t runtime,
                                           float* temperature,
                                           float* top_p,
                                           int* top_k);

// ============================================================================
// Prompt/Chat Functions
// ============================================================================

/**
 * Set the prompt
 * @param runtime Runtime handle
 * @param prompt Prompt text
 * @return 0 on success, negative on error
 */
int rwkvmobile_runtime_set_prompt(rwkvmobile_runtime_t runtime, const char* prompt);

/**
 * Get the current prompt
 * @param runtime Runtime handle
 * @return Prompt string
 */
const char* rwkvmobile_runtime_get_prompt(rwkvmobile_runtime_t runtime);

/**
 * Set BOS token
 * @param runtime Runtime handle
 * @param token BOS token string
 * @return 0 on success
 */
int rwkvmobile_runtime_set_bos_token(rwkvmobile_runtime_t runtime, const char* token);

/**
 * Set EOS token
 * @param runtime Runtime handle
 * @param token EOS token string
 * @return 0 on success
 */
int rwkvmobile_runtime_set_eos_token(rwkvmobile_runtime_t runtime, const char* token);

/**
 * Set user role for chat
 * @param runtime Runtime handle
 * @param role User role string
 * @return 0 on success
 */
int rwkvmobile_runtime_set_user_role(rwkvmobile_runtime_t runtime, const char* role);

/**
 * Set response role for chat
 * @param runtime Runtime handle
 * @param role Response role string
 * @return 0 on success
 */
int rwkvmobile_runtime_set_response_role(rwkvmobile_runtime_t runtime, const char* role);

// ============================================================================
// Speed/Performance Metrics
// ============================================================================

/**
 * Get average decode speed (tokens per second)
 * @param runtime Runtime handle
 * @return Decode speed in tokens/second
 */
float rwkvmobile_runtime_get_avg_decode_speed(rwkvmobile_runtime_t runtime);

/**
 * Get average prefill speed (tokens per second)
 * @param runtime Runtime handle
 * @return Prefill speed in tokens/second
 */
float rwkvmobile_runtime_get_avg_prefill_speed(rwkvmobile_runtime_t runtime);

/**
 * Get prefill progress (0.0 to 1.0)
 * @param runtime Runtime handle
 * @return Progress value
 */
float rwkvmobile_runtime_get_prefill_progress(rwkvmobile_runtime_t runtime);

// ============================================================================
// Seed Functions
// ============================================================================

/**
 * Set random seed
 * @param runtime Runtime handle
 * @param seed Seed value
 * @return 0 on success
 */
int rwkvmobile_runtime_set_seed(rwkvmobile_runtime_t runtime, uint64_t seed);

/**
 * Get current random seed
 * @param runtime Runtime handle
 * @return Current seed value
 */
uint64_t rwkvmobile_runtime_get_seed(rwkvmobile_runtime_t runtime);

#ifdef __cplusplus
}
#endif

#endif // RWKV_MOBILE_H

