param(
    [string]$BuildDir = "${PWD}",
    [string]$OutDir = "$PWD/coverage"
)

Write-Host "Generating coverage report"
Write-Host "BuildDir: $BuildDir"
Write-Host "OutDir: $OutDir"

if (-not (Test-Path -Path $BuildDir)) {
    Write-Error "Build directory '$BuildDir' does not exist"
    exit 2
}

# Find .profraw files produced by clang instrumentation
$profraws = Get-ChildItem -Path $BuildDir -Recurse -Filter *.profraw -ErrorAction SilentlyContinue
if (-not $profraws -or $profraws.Count -eq 0) {
    Write-Error "No .profraw files found under $BuildDir. Ensure tests ran with instrumentation."
    exit 3
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$profdata = Join-Path $OutDir "coverage.profdata"

$llvm_profdata = Get-Command llvm-profdata -ErrorAction SilentlyContinue
$llvm_cov = Get-Command llvm-cov -ErrorAction SilentlyContinue

if ($llvm_profdata -and $llvm_cov) {
    Write-Host "Merging ${($profraws | Measure-Object).Count} .profraw files into $profdata"
    $paths = $profraws | ForEach-Object { $_.FullName }
    & $llvm_profdata.Path merge -sparse $paths -o $profdata

    # Find test executables (heuristic: under build tree and in folders named 'tests')
    $testExes = Get-ChildItem -Path $BuildDir -Recurse -Filter *.exe -ErrorAction SilentlyContinue | Where-Object { $_.FullName -match '\\tests\\' }
    if (-not $testExes -or $testExes.Count -eq 0) {
        Write-Warning "No test executables found under $BuildDir (pattern '\\tests\\'). You can run llvm-cov manually against your binaries."
    } else {
        foreach ($exe in $testExes) {
            $name = [System.IO.Path]::GetFileNameWithoutExtension($exe.Name)
            $dest = Join-Path $OutDir $name
            Write-Host "Generating HTML coverage for $($exe.FullName) -> $dest"
            & $llvm_cov.Path show $exe.FullName -instr-profile=$profdata -format=html -output-dir $dest
        }
        Write-Host "Coverage HTML generated under: $OutDir"
    }
    exit 0
} else {
    Write-Error "llvm-profdata and/or llvm-cov not found in PATH. Install LLVM (add llvm bin to PATH) or generate coverage manually."
    exit 4
}
