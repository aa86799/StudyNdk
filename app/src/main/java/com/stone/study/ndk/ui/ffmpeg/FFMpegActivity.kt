package com.stone.study.ndk.ui.ffmpeg

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.flashexpress.permission.PermissionCheck
import com.stone.study.ndk.databinding.ActivityFfmpegBinding

class FFMpegActivity : AppCompatActivity() {

    private lateinit var mBind: ActivityFfmpegBinding
    private lateinit var mPermissionCheck: PermissionCheck

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mBind = ActivityFfmpegBinding.inflate(layoutInflater)
        setContentView(mBind.root)

        mPermissionCheck = PermissionCheck()
        mPermissionCheck.initPermissionLauncher(this)

        mBind.btnDeleteFile.setOnClickListener {
            // deleteAndMoveFile("sdcard/Download/test.txt")
            mPermissionCheck.manageStorageHandle(this) {
                test("sdcard/Download/input.mp4", "sdcard/Download/out.aac")
            }
        }
    }

    external fun deleteAndMoveFile(filePath: String): Int

    external fun test(filePath: String, outPath: String): Int
}