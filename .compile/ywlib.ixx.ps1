$cl = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\bin\Hostx64\x64\cl.exe"
$cmd = "&`"$cl`" /std:c++latest /EHsc /nologo /W4 /DYWLIB /c ywlib.ixx /reference std=.module\std.ifc /Fo.module\ /ifcOutput .module\ywlib.ifc"
Invoke-Expression $cmd
