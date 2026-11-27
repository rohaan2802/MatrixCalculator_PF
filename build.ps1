param([switch]$Test)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$buildDirectory = Join-Path $root 'build'
$configure = @('-S', $root, '-B', $buildDirectory)
if (-not (Test-Path -LiteralPath (Join-Path $buildDirectory 'CMakeCache.txt'))) {
    $compiler = Get-Command g++ -ErrorAction SilentlyContinue
    $ninja = Get-Command ninja -ErrorAction SilentlyContinue
    if ($compiler -and $ninja) {
        $configure += @('-G', 'Ninja', '-DCMAKE_BUILD_TYPE=Release', "-DCMAKE_CXX_COMPILER=$($compiler.Source)")
    }
}
cmake @configure
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
cmake --build $buildDirectory --config Release
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
if ($Test) {
    ctest --test-dir $buildDirectory -C Release --output-on-failure
    exit $LASTEXITCODE
}
