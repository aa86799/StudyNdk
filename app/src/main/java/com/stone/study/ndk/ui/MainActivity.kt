package com.stone.study.ndk.ui

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.stone.study.ndk.R
import com.stone.study.ndk.base.SampleAdapter
import com.stone.study.ndk.databinding.ActivityMainBinding
import com.stone.study.ndk.ui.encrypt.EncryptActivity
import com.stone.study.ndk.ui.jni.JniActivity
import com.stone.study.ndk.ui.jni.JniActivityForJava
import com.stone.study.ndk.ui.mp3.Mp3EncodeActivity
import com.stone.study.ndk.util.showLong

class MainActivity : AppCompatActivity() {

    private lateinit var mBind: ActivityMainBinding

    init {
        System.loadLibrary("stoneNdk-lib")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mBind = ActivityMainBinding.inflate(layoutInflater)
        setContentView(mBind.root)

        initMenu()
    }

    private fun initMenu() {
        mBind.rvMenu.adapter = SampleAdapter(TITLES) { index, title ->
            when (title.substring("$index.".length)) {
                "jni-java" -> startAct(JniActivityForJava::class.java)
                "jni" -> startAct(JniActivity::class.java)
                "text encrypt" -> startAct(EncryptActivity::class.java)
                "mp3 encode" -> startAct(Mp3EncodeActivity::class.java)
            }
        }

        showLong(R.string.app_name)
    }

    private fun <T> startAct(cls: Class<T>) {
        startActivity(Intent(this, cls))
    }

    companion object {
        val TITLES = listOf(
            "jni-java",
            "jni",
            "text encrypt",
            "mp3 encode"
        ).mapIndexed { index, s -> "$index.$s" }
    }
}