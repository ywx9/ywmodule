$temp = $env:INCLUDE
$env:INCLUDE = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\include" + ";" + "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt"
Invoke-Expression "cl /std:c++latest /EHsc /nologo /W4 /DYWLIB /utf-8 /c ywlib.ixx /reference std=module\std.ifc /Fomodule\ /ifcOutputmodule\ywlib.ifc"
$env:INCLUDE = $temp
