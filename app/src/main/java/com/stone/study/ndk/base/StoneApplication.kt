package com.stone.study.ndk.base

import android.app.Activity
import android.content.Context
import androidx.multidex.MultiDex
import androidx.multidex.MultiDexApplication

/**
 * desc:
 * author:  stone
 * email:   aa86799@163.com
 * blog :   https://stone.blog.csdn.net
 * time:    2020/4/4 14:27
 */
class StoneApplication : MultiDexApplication() {

    var mTopActivity: Activity? = null

    companion object {
        lateinit var instance: StoneApplication
        const val DOKIT_PID = "5e73502e25295284cd3a812b95af99a0"
    }

    override fun attachBaseContext(base: Context?) {
        super.attachBaseContext(base)
        MultiDex.install(this)
    }

    override fun onCreate() {
        super.onCreate()
        instance = this
    }

}