package com.example.rwkvmobiletest

import android.os.Bundle
import android.widget.Button
import android.widget.ScrollView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class MainActivity : AppCompatActivity() {

    private lateinit var tvOutput: TextView
    private lateinit var btnTestLibrary: Button
    private lateinit var btnGetBackends: Button
    private lateinit var btnInitRuntime: Button
    private lateinit var btnGetPlatformInfo: Button
    private lateinit var btnClearLog: Button
    private lateinit var scrollView: ScrollView

    private var runtimeHandle: Long = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        initViews()
        setupClickListeners()

        appendLog("=== RWKV Mobile Test App ===")
        appendLog("Testing librwkv_mobile.so JNI calls")
        appendLog("")
        
        // Check if library is loaded
        if (RwkvMobile.isLibraryLoaded()) {
            appendLog("✓ Library pre-loaded successfully")
        } else {
            appendLog("✗ Library not loaded: ${RwkvMobile.getLoadError()}")
        }
        appendLog("")
    }

    private fun initViews() {
        tvOutput = findViewById(R.id.tvOutput)
        btnTestLibrary = findViewById(R.id.btnTestLibrary)
        btnGetBackends = findViewById(R.id.btnGetBackends)
        btnInitRuntime = findViewById(R.id.btnInitRuntime)
        btnGetPlatformInfo = findViewById(R.id.btnGetPlatformInfo)
        btnClearLog = findViewById(R.id.btnClearLog)
        scrollView = findViewById(R.id.scrollView)
    }

    private fun setupClickListeners() {
        btnTestLibrary.setOnClickListener {
            testLibraryLoading()
        }

        btnGetBackends.setOnClickListener {
            testGetAvailableBackends()
        }

        btnInitRuntime.setOnClickListener {
            testRuntimeInitAndRelease()
        }

        btnGetPlatformInfo.setOnClickListener {
            testGetPlatformInfo()
        }

        btnClearLog.setOnClickListener {
            tvOutput.text = ""
            appendLog("=== Log Cleared ===")
        }
    }

    private fun testLibraryLoading() {
        appendLog("--- Testing Library Loading ---")
        
        if (!RwkvMobile.isLibraryLoaded()) {
            appendLog("✗ Library not loaded!")
            appendLog("  Error: ${RwkvMobile.getLoadError()}")
            appendLog("")
            return
        }
        
        appendLog("✓ Library is loaded!")
        appendLog("")
    }

    private fun testGetAvailableBackends() {
        appendLog("--- Testing Get Available Backends ---")
        
        if (!RwkvMobile.isLibraryLoaded()) {
            appendLog("✗ Library not loaded!")
            appendLog("")
            return
        }
        
        lifecycleScope.launch {
            withContext(Dispatchers.IO) {
                try {
                    val backends = RwkvMobile.getAvailableBackendNames()
                    withContext(Dispatchers.Main) {
                        if (backends != null && backends.isNotEmpty()) {
                            appendLog("✓ Available backends:")
                            backends.split(",").forEach { backend ->
                                if (backend.isNotBlank()) {
                                    appendLog("  - ${backend.trim()}")
                                }
                            }
                        } else {
                            appendLog("  No backends returned (empty or null)")
                        }
                    }
                } catch (e: UnsatisfiedLinkError) {
                    withContext(Dispatchers.Main) {
                        appendLog("✗ JNI Error: ${e.message}")
                        appendLog("  Note: Function may not be exported or has different signature")
                    }
                } catch (e: Exception) {
                    withContext(Dispatchers.Main) {
                        appendLog("✗ Exception: ${e.message}")
                    }
                }
            }
            appendLog("")
        }
    }

    private fun testRuntimeInitAndRelease() {
        appendLog("--- Testing Runtime Init/Release ---")
        
        if (!RwkvMobile.isLibraryLoaded()) {
            appendLog("✗ Library not loaded!")
            appendLog("")
            return
        }
        
        lifecycleScope.launch {
            withContext(Dispatchers.IO) {
                try {
                    // Initialize runtime
                    appendLogOnMain("Calling rwkvmobile_runtime_init()...")
                    val handle = RwkvMobile.rwkvmobile_runtime_init()
                    
                    withContext(Dispatchers.Main) {
                        if (handle != 0L) {
                            appendLog("✓ Runtime initialized!")
                            appendLog("  Handle: 0x${handle.toString(16)}")
                        } else {
                            appendLog("✗ Runtime init returned null handle")
                        }
                    }

                    // Release runtime
                    if (handle != 0L) {
                        appendLogOnMain("Calling rwkvmobile_runtime_release()...")
                        val releaseResult = RwkvMobile.rwkvmobile_runtime_release(handle)
                        withContext(Dispatchers.Main) {
                            if (releaseResult == 0) {
                                appendLog("✓ Runtime released successfully!")
                            } else {
                                appendLog("✗ Runtime release returned: $releaseResult")
                            }
                        }
                    }
                } catch (e: UnsatisfiedLinkError) {
                    withContext(Dispatchers.Main) {
                        appendLog("✗ JNI Error: ${e.message}")
                    }
                } catch (e: Exception) {
                    withContext(Dispatchers.Main) {
                        appendLog("✗ Exception: ${e.message}")
                        e.printStackTrace()
                    }
                }
            }
            appendLog("")
        }
    }

    private fun testGetPlatformInfo() {
        appendLog("--- Testing Platform Info ---")
        
        if (!RwkvMobile.isLibraryLoaded()) {
            appendLog("✗ Library not loaded!")
            appendLog("")
            return
        }
        
        lifecycleScope.launch {
            withContext(Dispatchers.IO) {
                try {
                    appendLogOnMain("Calling platform info functions...")
                    
                    val platformName = try { 
                        RwkvMobile.rwkvmobile_get_platform_name() 
                    } catch (e: Exception) { 
                        "Error: ${e.message}" 
                    }
                    
                    val socName = try { 
                        RwkvMobile.rwkvmobile_get_soc_name() 
                    } catch (e: Exception) { 
                        "Error: ${e.message}" 
                    }
                    
                    val socPartName = try { 
                        RwkvMobile.rwkvmobile_get_soc_partname() 
                    } catch (e: Exception) { 
                        "Error: ${e.message}" 
                    }
                    
                    val htpArch = try { 
                        RwkvMobile.rwkvmobile_get_htp_arch() 
                    } catch (e: Exception) { 
                        "Error: ${e.message}" 
                    }

                    withContext(Dispatchers.Main) {
                        appendLog("✓ Platform Info:")
                        appendLog("  Platform: $platformName")
                        appendLog("  SoC Name: $socName")
                        appendLog("  SoC Part: $socPartName")
                        appendLog("  HTP Arch: $htpArch")
                    }
                } catch (e: UnsatisfiedLinkError) {
                    withContext(Dispatchers.Main) {
                        appendLog("✗ JNI Error: ${e.message}")
                    }
                } catch (e: Exception) {
                    withContext(Dispatchers.Main) {
                        appendLog("✗ Exception: ${e.message}")
                    }
                }
            }
            appendLog("")
        }
    }
    
    private suspend fun appendLogOnMain(message: String) {
        withContext(Dispatchers.Main) {
            appendLog(message)
        }
    }

    private fun appendLog(message: String) {
        runOnUiThread {
            tvOutput.append("$message\n")
            scrollView.post {
                scrollView.fullScroll(ScrollView.FOCUS_DOWN)
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        // Release runtime if it was initialized and not released
        if (runtimeHandle != 0L) {
            try {
                RwkvMobile.rwkvmobile_runtime_release(runtimeHandle)
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }
}
