# prepares the directory for modules and executables
if (-not (Test-Path "module")) { New-Item -ItemType Directory -Name "module" | Out-Null }
if (-not (Test-Path "output")) { New-Item -ItemType Directory -Name "output" | Out-Null }

# adds the path to the `cl.exe` to the environment variable `PATH`
$temp = (Get-ChildItem cl.exe -Path "C:\Program Files*\Microsoft Visual Studio\*\*\VC\Tools\MSVC\*\bin\Hostx64\x64" -Recurse | Select-Object -First 1).FullName
$std_ixx = $temp -replace "\\bin\\Hostx64\\x64\\cl.exe", "\modules\std.ixx"
$env:PATH += ";$($temp -replace `"\\cl.exe`", `"`")"

# adds MSVC library directory to the environment variable `LIB`
$env:LIB += ";$($temp -replace `"\\bin\\Hostx64\\x64\\cl.exe`", `"\lib\x64`")"

# adds MSVC include directories to the environment variable `INCLUDE`
$temp = $temp -replace "\\bin\\Hostx64\\x64\\cl.exe", "\include"
$env:INCLUDE += ";$temp"

# adds Windows Kits library directories to the environment variable `LIB`
$temp = (Get-ChildItem ucrt -Path "C:\Program Files*\Windows Kits\10\Lib" -Recurse -Directory | Select-Object -Last 1).FullName
$temp = $temp -replace "\\ucrt", ""
$env:LIB += ";$temp\ucrt\x64;$temp\um\x64"

# adds Windows Kits include directories to the environment variable `INCLUDE`
$temp = $temp -replace "Lib", "Include"
$env:INCLUDE += ";$temp\um;$temp\ucrt;$temp\shared;$temp\winrt;$temp\cppwinrt"

# compiles `std.ixx`
Invoke-Expression "cl /std:c++latest /EHsc /nologo /W4 /utf-8 /c `"$std_ixx`" /Fomodule\std.obj /ifcOutputmodule\std.ifc"
