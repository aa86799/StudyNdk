package com.stone.study.ndk.ui.jni

import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.stone.study.ndk.databinding.ActivityJniJavaBinding
import java.util.*

/**
 * desc:
 * author:  stone
 * email:   aa86799@163.com
 * blog :   https://stone.blog.csdn.net
 * time:    2021/6/27 16:45
 */
class JniActivity: AppCompatActivity() {

    private lateinit var mBind: ActivityJniJavaBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mBind = ActivityJniJavaBinding.inflate(layoutInflater)
        setContentView(mBind.root)
        mBind.btnJC.setOnClickListener {
            Toast.makeText(
                this,
                putAndGet(Random().nextInt(2)).toString() + "",
                Toast.LENGTH_SHORT
            ).show()
        }
        mBind.btnCJ.setOnClickListener { v ->
            putDown(Random().nextInt(2)) // 调用 native ，其内部再调用  java ()
        }
    }

    private external fun putAndGet(arg: Int): Int

    private external fun putDown(arg: Int): Int

    fun whoLaLa(arg: Int) {
        Toast.makeText(this, "receive from c call: $arg", Toast.LENGTH_SHORT).show()
    }
}