CC:=g++ #g++でビルドする

CFLAGS:= -g3 #デバッグ情報最大化
CFLAGS+= -Wall #エラーを全部出す
CFLAGS+= -Wextra #追加の警告オプション
CFLAGS+= -std=gnu++20 #C++20でビルドする
CFLAGS+= -DDX_GCC_COMPILE
CFLAGS+= -DDX_NON_INLINE_ASM

TARGET = Ennichi #実行ファイルの名前
SRCS = *.cpp
ICON = icon.res #アイコンファイルの場所
INC_DIR := -I ./7_3_0 #インクルードするディレクトリ
LIB_DIR := -L ./7_3_0 #ライブラリディレクトリ

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
     -mwindows 

MAKE_ICON = windres icon.rc -O coff -o icon.res #アイコンファイルの作成

$(TARGET):
	$(CC) -o $(TARGET) $(CFLAGS) $(SRCS) $(ICON) $(INC_DIR) $(LIB_DIR) $(LDFLAGS)
