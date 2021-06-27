package com.stone.study.ndk.ui.jni;

import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.stone.study.ndk.databinding.ActivityJniJavaBinding;

import java.util.Random;

/**
 * desc:    native 方法：java 调用 c/c++, c/c++ 调用java；
 * author:  stone
 * email:   aa86799@163.com
 * blog :   https://stone.blog.csdn.net
 * time:    2021/6/27 17:05
 */
public class JniActivityForJava extends AppCompatActivity {

    private ActivityJniJavaBinding mBind;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mBind = ActivityJniJavaBinding.inflate(getLayoutInflater());
        setContentView(mBind.getRoot());

        mBind.btnJC.setOnClickListener(v -> {
            Toast.makeText(this, putAndGet(new Random().nextInt(2)) + "", Toast.LENGTH_SHORT).show();
        });

        mBind.btnCJ.setOnClickListener(v -> {
            putDown(new Random().nextInt(2)); // 调用 native ，其内部再调用  java ()
        });
    }

    private native int putAndGet(int arg);

    private native int putDown(int arg);

    void whoLaLa(int arg) {
        Toast.makeText(this, "receive from c call: " + arg, Toast.LENGTH_SHORT).show();
    }
}
