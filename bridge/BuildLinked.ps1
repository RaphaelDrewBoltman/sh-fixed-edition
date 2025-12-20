# Set Working Directory
Split-Path $MyInvocation.MyCommand.Path | Push-Location
[Environment]::CurrentDirectory = $PWD

Remove-Item "$env:RELOADEDIIMODS/bridge/*" -Force -Recurse
dotnet publish "./bridge.csproj" -c Release -o "$env:RELOADEDIIMODS/bridge" /p:OutputPath="./bin/Release" /p:ReloadedILLink="true"

# Restore Working Directory
Pop-Location