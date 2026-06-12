$ErrorActionPreference = "Stop"

if (-not (Get-Command wsl.exe -ErrorAction SilentlyContinue)) {
    Write-Host "WSL is not installed or wsl.exe is not available."
    Read-Host "Press Enter to close"
    exit 1
}

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$wslDir = (& wsl.exe wslpath -a "$projectDir").Trim()

if ([string]::IsNullOrWhiteSpace($wslDir)) {
    Write-Host "Could not convert this project path to a WSL path."
    Read-Host "Press Enter to close"
    exit 1
}

& wsl.exe --cd "$wslDir" -e bash -lc "TERM=xterm make snake_game && TERM=xterm ./snake_game"
$exitCode = $LASTEXITCODE

Write-Host ""
Read-Host "Press Enter to close"
exit $exitCode
