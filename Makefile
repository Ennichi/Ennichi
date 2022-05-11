MAKEFLAGS=-j8
MAKEFLAGS+= --no-builtin-rules
.SUFFIXES:	#サフィックス無効
CXX:=g++ #g++でビルドする

DEBUG:= -g3 #デバッグ情報最大化
CFLAGS:= -Wall #エラーを全部出す
CFLAGS+= -Wextra #追加の警告オプション
CFLAGS+= -Wcast-align #ポインタのキャスト
CFLAGS+= -Wdouble-promotion #floatからdouble
CFLAGS+= -Wconversion #暗黙的方変換
CFLAGS+= -Weffc++ #Effective C++
CFLAGS+= -Wfloat-equal #浮動小数点の比較
CFLAGS+= -Wlogical-op #正しい論理演算子
CFLAGS+= -Wmissing-declarations #前にないグローバル関数
CFLAGS+= -Wmissing-include-dirs #存在しないinclude
CFLAGS+= -Wold-style-cast #C風のキャストの防止
CFLAGS+= -Wswitch-default #default文の矯正
CFLAGS+= -Wunsafe-loop-optimizations #ループのオーバーフロー

CFLAGS+= -std=c++23 #C++23でビルドする
CFLAGS+= -pipe #パイプ処理でコンパイル

CFLAGS+= -DDX_GCC_COMPILE
CFLAGS+= -DDX_NON_INLINE_ASM

RELEASE:= -O2	#最適化オプション
RELEASE+= -fomit-frame-pointer #バイナリのサイズ減少
RELEASE+= -march=native #このアーキテクチャのみのバイナリ生成
RELEASE+= -pipe

TARGET = Ennichi #実行ファイルの名前
SRCS := ./src/game_main/*.cpp
HEADER := src/header/stdafx.h
HEADER += src/header/main.hpp

GENERIC := src/class/header/generic/*
OBJECT := src/class/header/object/*
INC_DIR := -isystem ./7_3_0_x86_64_w64_win32_seh_rt_v5_rev0 #インクルードするディレクトリ
LIB_DIR := -L ./7_3_0_x86_64_w64_win32_seh_rt_v5_rev0 #ライブラリディレクトリ

#リンク時のフラグ
LDFLAGS := -lDxLib \
     -lDxUseCLib \
     -lDxDrawFunc \
     -ljpeg \
     -lpng \
     -lzlib \
     -ltiff \
     -ltheora_static \
     -lvorbis_static \
     -lvorbisfile_static \
     -logg_static \
     -lbulletdynamics \
     -lbulletcollision \
     -lbulletmath \
     -lopusfile \
     -lopus \
     -lsilk_common \
     -lcelt \
     -mwindows \
     -fexec-charset=cp932      
DEBUG:
	$(CXX) -o $(TARGET) $(DEBUG) $(CFLAGS) -include $(HEADER) $(SRCS)  $(INC_DIR) $(LIB_DIR)  $(LDFLAGS) 
RELEASE:
	$(CXX) -o $(TARGET) $(RELEASE) $(CFLAGS) -include $(HEADER) $(SRCS)  $(INC_DIR) $(LIB_DIR)  $(LDFLAGS)
	
