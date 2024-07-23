if (-not (Test-Path ".output")) { New-Item -ItemType Directory -Name ".output" | Out-Null }
$cl = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\bin\Hostx64\x64\cl.exe"
$l0 = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\lib\x64"
$l1 = "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64"
$l2 = "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64"
$cmd = "&`"$cl`" ywlib.cpp /std:c++latest /EHsc /nologo /W4 /DYWLIB /utf-8 /Fe.output\ /Fo.output\ /reference ywlib=.module\ywlib.ifc /reference std=.module\std.ifc /link .module\std.obj /LIBPATH:`"$l0`" /LIBPATH:`"$l1`" /LIBPATH:`"$l2`""
Invoke-Expression $cmd
