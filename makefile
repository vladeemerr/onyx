all: onyx.exe

.cpp.obj:
	cl /nologo /std:c++20 /Wall /MTd /Zi /Od /c $*.cpp

onyx.exe: win32_onyx.obj
	link /nologo /out:$@ kernel32.lib user32.lib gdi32.lib $**

clean:
	del *.exe *.obj *.pdb
