Get-CimInstance Win32_Process | Where-Object {
  $_.CommandLine -match 'hermes_cli.main|gateway run|hermes serve|Hermes.exe'
} | ForEach-Object { "PID=$($_.ProcessId) PP=$($_.ParentProcessId) NAME=$($_.Name)`n$($_.CommandLine)`n----" }
