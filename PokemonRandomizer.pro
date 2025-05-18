QT       += core gui network widgets concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
QMAKE_CXXFLAGS += -std=c++20

# Include paths for thirdparty, SV_FLATBUFFERS, and images
INCLUDEPATH += $$PWD/thirdparty
INCLUDEPATH += $$PWD/SV_FLATBUFFERS
INCLUDEPATH += $$PWD/assets
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/logs

# Add files to DISTFILES for deployment
DISTFILES += $$files($$PWD/thirdparty/*, true)
DISTFILES += $$files($$PWD/SV_FLATBUFFERS/*, true)
DISTFILES += $$files($$PWD/assets/*, true)
DISTFILES += $$files($$PWD/include/*, true)
DISTFILES += $$files($$PWD/logs/*, true)

# Enable compatibility for building across platforms
win32 {
    CONFIG += windows
    CONFIG -= debug_and_release debug_and_release_target
    QMAKE_HOST.arch:equals(x86_64) {
        DEFINES += ARCH_64
        message("Building for 64-bit Windows")
    } else {
        DEFINES += ARCH_32
        message("Building for 32-bit Windows")
    }

    RC_FILE = app_icon.rc

    # Paths for source and destination directories

    THIRDPARTY_SRC = $$PWD/thirdparty
    IMAGES_SRC = $$PWD/assets
    SV_FLATBUFFERS_SRC = $$PWD/SV_FLATBUFFERS
    INCLUDE_SRC = $$PWD/include
    LOGS_SRC = $$PWD/logs

    # Destination directory adjusted for release/debug
    CONFIG(debug, debug|release) {
        BUILD_DIR = $$OUT_PWD
    } else {
        BUILD_DIR = $$OUT_PWD
    }

    THIRDPARTY_DEST = $$BUILD_DIR/thirdparty
    IMAGES_DEST = $$BUILD_DIR/assets
    SV_FLATBUFFERS_DEST = $$BUILD_DIR/SV_FLATBUFFERS
    INCLUDE_DEST = $$BUILD_DIR/include
    LOGS_DEST = $$BUILD_DIR/logs


    SOURCE_DIR_WIN = $$replace(THIRDPARTY_SRC, /, \\)
    DEST_DIR_WIN = $$replace(THIRDPARTY_DEST, /, \\)
    SOURCE_IMAGE_WIN = $$replace(IMAGES_SRC, /, \\)
    DEST_IMAGE_WIN = $$replace(IMAGES_DEST, /, \\)
    SOURCE_FLAT_WIN = $$replace(SV_FLATBUFFERS_SRC, /, \\)
    DEST_FLAT_WIN = $$replace(SV_FLATBUFFERS_DEST, /, \\)
    SOURCE_INCLUDE_WIN = $$replace(INCLUDE_SRC, /, \\)
    DEST_INCLUDE_WIN = $$replace(INCLUDE_DEST, /, \\)
    DEST_LOGS_WIN = $$replace(LOGS_DEST, /, \\)
    SOURCE_LOGS_WIN = $$replace(LOGS_SRC, /, \\)

    copy_third.commands = \
        $$QMAKE_COPY_DIR \"$$SOURCE_DIR_WIN\" \"$$DEST_DIR_WIN\" && \
        $$QMAKE_COPY_DIR \"$$SOURCE_IMAGE_WIN\" \"$$DEST_IMAGE_WIN\" && \
        $$QMAKE_COPY_DIR \"$$SOURCE_FLAT_WIN\" \"$$DEST_FLAT_WIN\" && \
        $$QMAKE_COPY_DIR \"$$SOURCE_INCLUDE_WIN\" \"$$DEST_INCLUDE_WIN\" && \
        $$QMAKE_COPY_DIR \"$$SOURCE_LOGS_WIN\" \"$$DEST_LOGS_WIN\"


    QMAKE_EXTRA_TARGETS += copy_third
    PRE_TARGETDEPS += copy_third
    CONFIG(debug, debug|release) {
        BUILD_DIR = $$OUT_PWD
    } else {
        DEPLOY_PATH = $$OUT_PWD
        DEPLOY_SCRIPT = $$PWD/scripts/deploy_windows.bat
        post_build.commands = $$DEPLOY_SCRIPT $$OUT_PWD/PokemonRandomizer.exe $$DEPLOY_PATH
        QMAKE_EXTRA_TARGETS += post_build
        QMAKE_POST_LINK += $(MAKE) -f Makefile post_build
    }
}

macx {
    QMAKE_CXXFLAGS += -arch arm64
    QMAKE_LFLAGS += -arch arm64
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0
    CONFIG -= x86_64
    CONFIG += macx
    contains(QMAKE_CXXFLAGS, -arch\\s*x86_64): DEFINES += MAC_INTEL
    contains(QMAKE_CXXFLAGS, -arch\\s*arm64): DEFINES += MAC_ARM

    # Paths for source and destination directories
    THIRDPARTY_SRC = $$PWD/thirdparty
    THIRDPARTY_DEST = $$OUT_PWD/thirdparty
    IMAGES_SRC = $$PWD/assets
    IMAGES_DEST = $$OUT_PWD/assets
    SV_FLATBUFFERS_SRC = $$PWD/SV_FLATBUFFERS
    SV_FLATBUFFERS_DEST = $$OUT_PWD/SV_FLATBUFFERS
    INCLUDE_SRC = $$PWD/include
    INCLUDE_DEST = $$OUT_PWD/include
    LOGS_DEST = $$OUT_PWD/logs
    LOGS_SRC = $$PWD/logs

    # Custom command to copy the include folder
    copy_include.commands = mkdir -p $$INCLUDE_DEST && rsync -a $$INCLUDE_SRC/. $$INCLUDE_DEST
    QMAKE_EXTRA_TARGETS += copy_include

    # Custom command to copy the images folder
    copy_images.commands = mkdir -p $$IMAGES_DEST && rsync -a $$IMAGES_SRC/. $$IMAGES_DEST
    QMAKE_EXTRA_TARGETS += copy_images

    # Custom command to copy the thirdparty folder
    copy_thirdparty.commands = mkdir -p $$THIRDPARTY_DEST && rsync -a $$THIRDPARTY_SRC/. $$THIRDPARTY_DEST
    QMAKE_EXTRA_TARGETS += copy_thirdparty

    # Custom command to copy the SV_FLATBUFFERS folder
    copy_sv_flatbuffers.commands = mkdir -p $$SV_FLATBUFFERS_DEST && rsync -a $$SV_FLATBUFFERS_SRC/. $$SV_FLATBUFFERS_DEST
    QMAKE_EXTRA_TARGETS += copy_sv_flatbuffers

    # Custom command to copy the logs folder
    copy_logs.commands = mkdir -p $$LOGS_DEST && rsync -a $$LOGS_SRC/. $$LOGS_DEST
    QMAKE_EXTRA_TARGETS += copy_logs

    # Ensure the copy commands are executed before building
    PRE_TARGETDEPS += copy_images copy_thirdparty copy_sv_flatbuffers copy_include

    # Deployment for macOS using macdeployqt
    DEPLOY_PATH = $$OUT_PWD/deploy
    DEPLOY_SCRIPT = $$PWD/scripts/deploy_mac.sh
    post_build.commands = bash $$DEPLOY_SCRIPT $$OUT_PWD/PokemonRandomizer.app $$DEPLOY_PATH
    QMAKE_EXTRA_TARGETS += post_build
    QMAKE_POST_LINK += $(MAKE) -f Makefile post_build
}

unix:!macx {
    CONFIG += linux

    # Paths for source and destination directories
    THIRDPARTY_SRC = $$PWD/thirdparty
    THIRDPARTY_DEST = $$OUT_PWD/thirdparty
    IMAGES_SRC = $$PWD/assets
    IMAGES_DEST = $$OUT_PWD/assets
    SV_FLATBUFFERS_SRC = $$PWD/SV_FLATBUFFERS
    SV_FLATBUFFERS_DEST = $$OUT_PWD/SV_FLATBUFFERS
    INCLUDE_SRC = $$PWD/include
    INCLUDE_DEST = $$OUT_PWD/include
    LOGS_DEST = $$OUT_PWD/logs
    LOGS_SRC = $$PWD/logs

    # Custom command to copy the include folder
    copy_include.commands = mkdir -p $$INCLUDE_DEST && cp -r $$INCLUDE_SRC/. $$INCLUDE_DEST
    QMAKE_EXTRA_TARGETS += copy_include

    # Custom command to copy the images folder
    copy_images.commands = mkdir -p $$IMAGES_DEST && cp -r $$IMAGES_SRC/. $$IMAGES_DEST
    QMAKE_EXTRA_TARGETS += copy_images

    # Custom command to copy the thirdparty folder
    copy_thirdparty.commands = mkdir -p $$THIRDPARTY_DEST && cp -r $$THIRDPARTY_SRC/. $$THIRDPARTY_DEST
    QMAKE_EXTRA_TARGETS += copy_thirdparty

    # Custom command to copy the SV_FLATBUFFERS folder
    copy_sv_flatbuffers.commands = mkdir -p $$SV_FLATBUFFERS_DEST && cp -r $$SV_FLATBUFFERS_SRC/. $$SV_FLATBUFFERS_DEST
    QMAKE_EXTRA_TARGETS += copy_sv_flatbuffers

    # Custom command to copy the logs folder
    copy_logs.commands = mkdir -p $$LOGS_DEST && cp -r $$LOGS_SRC/. $$LOGS_DEST
    QMAKE_EXTRA_TARGETS += copy_logs

    # Ensure the copy commands are executed before building
    PRE_TARGETDEPS += copy_images copy_thirdparty copy_sv_flatbuffers

    contains(QMAKE_CXX, g++) {
        message("Using g++ on Linux")
        DEFINES += LINUX_GCC
    } else: contains(QMAKE_CXX, clang++) {
        message("Using clang++ on Linux")
        DEFINES += LINUX_CLANG
    }

    # Deployment for Linux using linuxdeployqt
    # Uncomment the next lines if you have Ubuntu 20.04 (Focal Fossa) with glibc 2.31
    # DEPLOY_PATH = $$OUT_PWD/deploy
    # DEPLOY_SCRIPT = $$PWD/scripts/deploy_linux.sh
    # post_build.commands = bash $$DEPLOY_SCRIPT $$OUT_PWD/release/PokemonRandomizer $$DEPLOY_PATH
    # QMAKE_EXTRA_TARGETS += post_build
    # QMAKE_POST_LINK += $(MAKE) -f Makefile post_build

}

# Subfolders visibility in the project sidebar
VPATH += $$unique($$dirname($$files(src/*, true)))
VPATH += $$unique($$dirname($$files(headers/*, true)))

# Recursively include all .cpp and .h files from src and headers directories
SOURCES += $$files(src/*.cpp, true) \

HEADERS += $$files(headers/*.h, true) \

FORMS +=

# Deployment paths
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Handle platform-specific libraries and dependencies
macx {
    LIBS += -framework Cocoa
}

win32 {
    LIBS += -luser32 -lgdi32 -lcomdlg32
}

unix:!macx {
    LIBS += -lpthread
}

# Additional build configurations
DEFINES += QT_DEPRECATED_WARNINGS

message($$system($$QMAKE_CXX --version))
