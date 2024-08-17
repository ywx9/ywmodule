# checks if the variables are initialized
if ($init -ne $true) {
  .compile/init.ps1
}

# checks if `.module/std.ifc` and `.module/std.obj` exists
if (-not (Test-Path ".module\ywlib.ifc") -or -not (Test-Path ".module\ywlib.obj")) {
  .compile/std.ps1
}

# checks if `ywlib.ifc` and `ywlib.obj` exists
if (-not (Test-Path "ywlib.ifc") -or -not (Test-Path "ywlib.obj")) {
  .compile/ywlib.ixx.ps1
}

# command to compile `ywlib.cpp`
$cmd = "&`"$cl_exe`" /std:c++latest /EHsc /nologo /W4 /DYWLIB /utf-8 ywlib.cpp .module\ywlib.obj /Fe.output\ /Fo.output\ /reference ywlib=.module\ywlib.ifc /reference std=.module\std.ifc /link .module\std.obj /LIBPATH:`"$msvc_lib`" /LIBPATH:`"$win_kits_ucrt_lib`" /LIBPATH:`"$win_kits_um_lib`""

# compiles `ywlib.cpp`
Invoke-Expression $cmd
