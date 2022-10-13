package com.flashexpress.permission

import android.content.Context
import androidx.appcompat.app.AlertDialog

object DialogUtil {

    fun alert(
        context: Context,
        title: String,
        msg: String,
        positive: String,
        positiveBlock: (() -> Unit)? = null,
        negative: String,
        negativeBlock: (() -> Unit)? = null
    ) {
        AlertDialog.Builder(context)
            .setTitle(title)
            .setMessage(msg)
            .setPositiveButton(positive) { dialog, _ ->
            positiveBlock?.invoke()
        }.setNegativeButton(negative) { dialog, _ ->
            negativeBlock?.invoke()
        }.create().show()
    }
}