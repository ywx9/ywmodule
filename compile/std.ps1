# checks if the variables are initialized
if ($init -ne $true) {
  .compile/init.ps1
}

# command to compile `std.ixx`
$cmd = "&`"$cl_exe`" /std:c++latest /EHsc /nologo /W4 /utf-8 /c `"$std_ixx`" /I`"$msvc_inc`" /I`"$inc_cppwinrt`" /I`"$inc_shared`" /I`"$inc_ucrt`" /I`"$inc_um`" /I`"$inc_winrt`" /Fo.module\std.obj /ifcOutput.module\std.ifc"

# compiles `std.ixx`
Invoke-Expression $cmd
