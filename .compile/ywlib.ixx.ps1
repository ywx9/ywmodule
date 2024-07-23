if (-not (Test-Path ".module")) { New-Item -ItemType Directory -Name ".module" | Out-Null }
$cl = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\bin\Hostx64\x64\cl.exe"
$i0 = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include"
$i1 = "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\cppwinrt"
$i2 = "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared"
$i3 = "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt"
$i4 = "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um"
$i5 = "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\winrt"
$cmd = "&`"$cl`" /std:c++latest /EHsc /nologo /W4 /DYWLIB /utf-8 /c ywlib.ixx /I`"$i0`" /I`"$i1`" /I`"$i2`" /I`"$i3`" /I`"$i4`" /I`"$i5`" /reference std=.module\std.ifc /Fo.module\ /ifcOutput.module\ywlib.ifc"
Invoke-Expression $cmd
