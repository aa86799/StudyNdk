package com.flashexpress.permission

import android.Manifest
import android.content.Context
import android.content.Intent
import android.location.LocationManager
import android.net.Uri
import android.os.Build
import android.os.Environment
import android.provider.Settings
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.result.ActivityResultLauncher
import androidx.activity.result.contract.ActivityResultContracts
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AlertDialog
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.core.content.PermissionChecker
import androidx.fragment.app.Fragment
import com.stone.permission.R

/**
 * desc:
 * author:  stone
 * email:   aa86799@163.com
 * time:    2021/6/9 14:42
 */

/**
 * 请求定位权限. 要先检查是否开启了定位服务
 */
class PermissionCheck {

    private var mGrantedCallback: (() -> Unit)? = null
    private var mDeniedCallback: (() -> Unit)? = null

    private lateinit var mRequestPermissionLauncher: ActivityResultLauncher<String>

    fun locationPermissionHandle(activity: ComponentActivity, granted: (() -> Unit)? = null) {
        if (!checkLocationService(activity)) return
        val permission = Manifest.permission.ACCESS_FINE_LOCATION
        checkSpecificPermission(activity, permission, grantedCallback = {
            granted?.invoke()
        }, rationaleCallback = {
            DialogUtil.alert(activity, activity.getString(R.string.warning), activity.getString(R.string.needLocationPermission), "confirm", {
                mRequestPermissionLauncher.launch(permission)
            }, "cancel", null)
        }, deniedCallback = {
            DialogUtil.alert(activity, activity.getString(R.string.warning),  activity.getString(R.string.location_is_required), "confirm", {
                toSelfSetting(activity)
            }, "cancel", null)
        })
    }

    /**
     * 图片等文件读写操作，可能要用到 WRITE_EXTERNAL_STORAGE 外部存储权限。
     * android 11 及以上，开启后，仅允许访问媒体文件； 管理文件，需要另申请 MANAGE_EXTERNAL_STORAGE。
     * 为了兼容性，>=6 && <=10 需要 权限检查。
     */
    fun storagePermissionHandle(activity: ComponentActivity, granted: (() -> Unit)? = null) {
        val permission = Manifest.permission.WRITE_EXTERNAL_STORAGE
        checkSpecificPermission(activity, permission, grantedCallback = {
            granted?.invoke()
        }, rationaleCallback = {
            DialogUtil.alert(activity, activity.getString(R.string.warning), activity.getString(R.string.reject_tip), "confirm", {
                mRequestPermissionLauncher.launch(permission)
            }, "cancel", null)
        }, deniedCallback = {
            DialogUtil.alert(activity, activity.getString(R.string.warning), activity.getString(R.string.reject_tip), "confirm", {
                toSelfSetting(activity)
            }, "cancel", null)
        })
    }

    /**
     * 请求相机权限. 通常也需要请求 外部存储权限 (WRITE_EXTERNAL_STORAGE)
     */
    fun cameraPermissionHandle(activity: ComponentActivity, albumPermission: Boolean = true, granted: (() -> Unit)? = null) {
        val permission = Manifest.permission.CAMERA
        checkSpecificPermission(activity, permission, grantedCallback = {
            if (albumPermission) storagePermissionHandle(activity, granted)
            else granted?.invoke()
        }, rationaleCallback = {
            DialogUtil.alert(activity, activity.getString(R.string.warning), activity.getString(R.string.needCameraPermission), "confirm", {
                mRequestPermissionLauncher.launch(permission)
            }, "cancel", null)
        }, deniedCallback = {
            DialogUtil.alert(activity, activity.getString(R.string.warning),  activity.getString(R.string.startCameraPermissionHint), "confirm", {
                toSelfSetting(activity)
            }, "cancel", null)
        })
    }

    /*
     * MANAGE_EXTERNAL_STORAGE 权限授权后，在checkSpecificPermission()中还是会走到拒绝授权的代码块中；
     * 所以 增加 if (Environment.isExternalStorageManager()) 的判断
     */
    @RequiresApi(Build.VERSION_CODES.R)
    fun manageStorageHandle(activity: ComponentActivity, granted: (() -> Unit)? = null) {
        val permission = Manifest.permission.MANAGE_EXTERNAL_STORAGE
        checkSpecificPermission(activity, permission, grantedCallback = {
            granted?.invoke()
        }, rationaleCallback = {
            if (Environment.isExternalStorageManager()) {
                granted?.invoke()
                return@checkSpecificPermission
            }
            DialogUtil.alert(activity, activity.getString(R.string.warning), activity.getString(R.string.manage_file_tip), "confirm", {
                mRequestPermissionLauncher.launch(permission)
            }, "cancel", null)
        }, deniedCallback = {
            if (Environment.isExternalStorageManager()) {
                granted?.invoke()
                return@checkSpecificPermission
            }
            DialogUtil.alert(activity, activity.getString(R.string.warning), activity.getString(R.string.manage_file_tip), "confirm", {
                activity.startActivity(Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION))
            }, "cancel", null)
        })
    }

    private fun checkLocationService(activity: ComponentActivity): Boolean { // 如果打开了定位服务，再去检测定位权限；  否则 让用户去设置页面打开定位服务
        return if (isLocServiceEnable(activity.applicationContext)) {
            true
        } else {
            DialogUtil.alert(activity, "", activity.getString(R.string.if_not_be_able_connect_bluetooth_printer), "confirm", {
                val locationSet = Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS)
                activity.startActivity(locationSet)
            }, "cancel", null)
            false
        }
    }

    fun initPermissionLauncher(fragment: Fragment) {
        mRequestPermissionLauncher = requestPermission(fragment) { isGranted: Boolean ->
            if (isGranted) {
                if (mGrantedCallback != null) {
                    mGrantedCallback?.invoke()
                    return@requestPermission
                }
                Toast.makeText(fragment.requireContext(), "granted", Toast.LENGTH_SHORT).show()
            } else {
                if (mDeniedCallback != null) {
                    mDeniedCallback?.invoke()
                    return@requestPermission
                }
                Toast.makeText(fragment.requireContext(), "denied", Toast.LENGTH_SHORT).show()
                DialogUtil.alert(
                    fragment.requireContext(), "why need it", "please go to setting and open it", "confirm", null, "cancel", null
                )
            }
        }
    }

    fun initPermissionLauncher(activity: ComponentActivity) {
        mRequestPermissionLauncher = requestPermission(activity) { isGranted: Boolean ->
            if (isGranted) {
                if (mGrantedCallback != null) {
                    mGrantedCallback?.invoke()
                    return@requestPermission
                }
                Toast.makeText(activity, "granted", Toast.LENGTH_SHORT).show()
            } else {
                if (mDeniedCallback != null) {
                    mDeniedCallback?.invoke()
                    return@requestPermission
                }
                Toast.makeText(activity, "denied", Toast.LENGTH_SHORT).show()
                DialogUtil.alert(
                    activity, "why need it", "please go to setting and open it", "confirm", null, "cancel", null
                )
            }
        }
    }

    inline fun requestPermission(fragment: Fragment, crossinline block: (Boolean) -> Unit): ActivityResultLauncher<String> {
        return fragment.registerForActivityResult(ActivityResultContracts.RequestPermission()) { result ->
            block(result)
        }
    }

    inline fun requestPermission(activity: ComponentActivity, crossinline block: (Boolean) -> Unit): ActivityResultLauncher<String> {
        return activity.registerForActivityResult(ActivityResultContracts.RequestPermission()) { result ->
            block(result)
        }
    }

    fun checkSpecificPermission(
        activity: ComponentActivity,
        permission: String,
        grantedCallback: (() -> Unit)? = null,
        rationaleCallback: (() -> Unit)? = null,
        deniedCallback: (() -> Unit)? = null
    ) {
        mGrantedCallback = grantedCallback
        mDeniedCallback = deniedCallback
        val selfPermission = ContextCompat.checkSelfPermission(activity, permission)
        when {
            selfPermission == PermissionChecker.PERMISSION_GRANTED -> {
                if (mGrantedCallback != null) {
                    mGrantedCallback?.invoke()
                    return
                }
                Toast.makeText(activity, "granted", Toast.LENGTH_SHORT).show()
            }

            // 首次拒绝，但未选中 "不再提醒"
            ActivityCompat.shouldShowRequestPermissionRationale(activity, permission) -> {
                if (rationaleCallback != null) {
                    rationaleCallback.invoke()
                    return
                } // show tips, needs permission why it is
                AlertDialog.Builder(activity).setTitle("why need it ").setNegativeButton("cancel") { dialog, _ ->
                }.setPositiveButton("confirm") { dialog, _ ->
                    mRequestPermissionLauncher.launch(permission)
                }.create().show()
            }
            selfPermission == PermissionChecker.PERMISSION_DENIED -> {
                mRequestPermissionLauncher.launch(permission)
            }
        }
    }

    fun toSelfSetting(context: Context) {
        Intent().apply {
            addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
            action = Settings.ACTION_APPLICATION_DETAILS_SETTINGS
            data = Uri.fromParts("package", context.packageName, null)
            context.startActivity(this)
        }
    }

    /**
     * 手机是否开启位置服务，如果没有开启那么所有app将不能使用定位功能
     */
    fun isLocServiceEnable(context: Context): Boolean {
        val locationManager = context.getSystemService(Context.LOCATION_SERVICE) as LocationManager
        val gps = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)
        val netWork = locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER)
        return gps || netWork
    }
}