package com.stone.study.ndk.ui.encrypt

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.stone.study.ndk.databinding.ActivityEncryptBinding

class EncryptActivity : AppCompatActivity() {

    private lateinit var mBind: ActivityEncryptBinding
    private var mEncodeByteArray: ByteArray? = null
    private var mDecodeStr = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mBind = ActivityEncryptBinding.inflate(layoutInflater)
        setContentView(mBind.root)

        mBind.btnEncode.setOnClickListener {
            mEncodeByteArray =
                encode(mBind.etContent.text.toString().toByteArray()) ?: return@setOnClickListener
            mBind.tvEncodeResult.text = "encode after: ".plus(String(mEncodeByteArray!!))
        }

        mBind.btnDecode.setOnClickListener {
            mEncodeByteArray ?: return@setOnClickListener
            Log.i("stone.stone", "onCreate: ${mEncodeByteArray?.size}")
            val decodeByteArray = decode(mEncodeByteArray!!) ?: return@setOnClickListener
            mDecodeStr = String(decodeByteArray)
            mBind.tvDecodeResult.text = "decode after:".plus(mDecodeStr)
        }
    }

    external fun encode(textAry: ByteArray): ByteArray?

    external fun decode(textAry: ByteArray): ByteArray?

}