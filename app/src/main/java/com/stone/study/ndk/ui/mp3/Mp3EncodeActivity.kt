package com.stone.study.ndk.ui.mp3

import android.app.ProgressDialog
import android.media.MediaPlayer
import android.os.Build
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.net.toUri
import androidx.lifecycle.lifecycleScope
import com.flashexpress.permission.PermissionCheck
import com.stone.study.ndk.databinding.ActivityMp3Binding
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

class Mp3EncodeActivity : AppCompatActivity() {

    private lateinit var mBind: ActivityMp3Binding
    private lateinit var mPermissionCheck: PermissionCheck
    private var mPlayer: MediaPlayer? = null
    private var mProgressUpdateFlag = true

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        mPermissionCheck = PermissionCheck()

        mBind = ActivityMp3Binding.inflate(layoutInflater)
        setContentView(mBind.root)
        mBind.tvInput.text = "input pcm path: sdcard/Download/output.pcm"
        mBind.tvOutput.text = "output mp3 path: sdcard/Download/out.mp3"

        mPermissionCheck.initPermissionLauncher(this)

        mBind.btnEncode.setOnClickListener {
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R) { // android 11之前
                mPermissionCheck.storagePermissionHandle(this) {

                }
            } else {
                mPermissionCheck.manageStorageHandle(this) {
                    val loading = ProgressDialog(this)
                    loading.setProgressStyle(ProgressDialog.STYLE_SPINNER)
                    loading.show() // 若后面紧跟一个同线程的耗时操作，就会显示不出来
                    lifecycleScope.launchWhenResumed {
                        delay(500) // delay 后，loading 就会立即显示
                        if (init(
                                "sdcard/Download/output.pcm", 2,
                                128_000, 44100, "sdcard/Download/out.mp3"
                            ) == 0) {
                            encode()
                            mPlayer = MediaPlayer.create(this@Mp3EncodeActivity, "sdcard/Download/out.mp3".toUri())
                            mBind.seekBar.max = mPlayer?.duration ?: 0
                            mPlayer?.seekTo(0)
                            mPlayer?.isLooping = true
                            mPlayer?.start()
                        }
                        loading.cancel()
                        loading.dismiss()
                    }
                }
                lifecycleScope.launch(Dispatchers.Default) {
                    while (mProgressUpdateFlag) {
                        if (mPlayer?.isPlaying == true)
                            mBind.seekBar.progress = mPlayer!!.currentPosition
                    }
                }
            }
        }
    }

    external fun init(pcmPath: String, audioChannels: Int, bitRate: Int, sampleRate: Int, mp3Path: String): Int
    external fun encode()
    external fun destroy()

    override fun onDestroy() {
        mProgressUpdateFlag = false
        destroy()
        mPlayer?.release()
        super.onDestroy()
    }
}