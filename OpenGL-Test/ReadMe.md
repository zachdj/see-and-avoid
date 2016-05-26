Build Instructions
==================
g++ -o hellot.exe main.cpp glfw3dll.a libglew32.dll.a -I include -L./ -lglew32 -lglfw3 -lopengl32

-L./ specifies that the compiler will look for library files in the project directory.  If you want to put your .dll files elsewhere, you'll have to change the directory.