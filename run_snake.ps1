$ErrorActionPreference = "Stop"

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$wslDir = (& wsl.exe wslpath -a "$projectDir").Trim()

& wsl.exe --cd "$wslDir" -e bash -lc "script -q -c './snake_game' /dev/null"

Write-Host ""
Read-Host "Press Enter to close"
