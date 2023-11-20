all: onyx.exe

.cpp.obj:
	cl /nologo /std:c++20 /W3 /EHsc /MTd /Zi /Od /c $*.cpp

onyx.exe: win32_window.obj d3d11_renderer.obj win32_onyx.obj
	link /nologo /out:$@ /debug kernel32.lib user32.lib gdi32.lib d3d11.lib dxguid.lib d3dcompiler.lib $**

shaders:
	fxc /nologo /T vs_5_0 /E vs_main /Fo shader_vs.cso shader.hlsl
	fxc /nologo /T ps_5_0 /E ps_main /Fo shader_ps.cso shader.hlsl

clean:
	del *.exe *.obj *.pdb *.cso
