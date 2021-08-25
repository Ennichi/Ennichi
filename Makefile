build:
	g++ -o  main -g3 -Wall -Wextra  -std=gnu++20 functions.cpp kingyomain.cpp game_temp.cpp \
     shooting.cpp title.cpp main.cpp \
     -I ./7_3_0 \
     -L ./7_3_0 \
     -DDX_GCC_COMPILE -DDX_NON_INLINE_ASM \
     -lDxLib \
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
     -lcelt 