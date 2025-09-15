# Set Working Directory
Split-Path $MyInvocation.MyCommand.Path | Push-Location
[Environment]::CurrentDirectory = $PWD

Remove-Item "$env:RELOADEDIIMODS/FixedEdition/*" -Force -Recurse
dotnet publish "./FixedEdition.csproj" -c Release -o "$env:RELOADEDIIMODS/FixedEdition" /p:OutputPath="./bin/Release" /p:ReloadedILLink="true"

# Restore Working Directory
Pop-Location