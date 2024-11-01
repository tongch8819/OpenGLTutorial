# gcc -g -Wall $1 -lopengl32 -lglu32 -lfreeglut

# gcc -c -o ex1.o ex1.cpp -I"D:\Projects\CPP\OpenGLTest\Misc\glutdlls37beta"
# gcc -o example.exe example.o -L"C:\Program Files\Common Files\MinGW\GLUT\lib" -lglut32 -lopengl32 -Wl,--subsystem,windows

## Dynamic Link
# gcc -c -o example.o example.c -I"D:\Projects\CPP\OpenGLTest\freeglut\include"
# gcc -o example.exe example.o -L"D:\Projects\CPP\OpenGLTest\freeglut\lib" -lfreeglut -lopengl32 -lglu32 -Wl,--subsystem,windows

## Static Link
# gcc -c -o example.o example.c -D FREEGLUT_STATIC -I"D:\Projects\CPP\OpenGLTest\freeglut\include"
gcc -o example.exe example.o -L"D:\Projects\CPP\OpenGLTest\freeglut\lib" -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -Wl,--subsystem,windows