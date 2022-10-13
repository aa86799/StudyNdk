# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.10.2)
#   CMake Error at /Users/stone/Documents/sdk/ndk/21.1.6352462/build/cmake/android.toolchain.cmake:250 (message):
#   gnustl_static is no longer supported.  Please switch to either c++_shared
#   or c++_static.  See

# lame 引入，第一种方式：源码依赖编译
## 头文件目录
#include_directories(include, mp3, mp3/libmp3lame)
## 添加一个库 源码文件列表为空；后面通过 target_sources 链接
#add_library(libmp3lame "")
## 查找在某个路径下的所有源文件; 并将输出结果列表储存在指定的变量中
#AUX_SOURCE_DIRECTORY(mp3/libmp3lame  lames)
## 将源码文件变量绑定到 目标库; PRIVATE 表示 这些源只应添加到 目标库libmp3lame；PUBLIC 表示 不但添加到目标库，也添加到其它链接了libmp3lame的库中
#target_sources(libmp3lame PRIVATE ${lames})

# lame 引入，第二种方式：直接使用 lame 的静态库
# 导入方式：静态
add_library(libmp3lame STATIC IMPORTED)
# 设置 libmp3lame 静态库的位置
set_target_properties(
        libmp3lame PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/mp3/${ANDROID_ABI}/libmp3lame.a
)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        stoneNdk-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        s_java/hello.cpp
        s_kt/hello.cpp
        encrypt/Encrypt.cpp
        JniUtil.cpp
        mp3/mp3_encoder.cpp
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       stoneNdk-lib
        libmp3lame

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )