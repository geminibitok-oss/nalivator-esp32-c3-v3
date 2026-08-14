@echo off
chcp 65001 >nul
title NALIVATOR ESP32 - Serial Terminal (115200)

powershell.exe -NoProfile -ExecutionPolicy Bypass -Command ^
  "[Console]::OutputEncoding = [System.Text.Encoding]::UTF8;" ^
  "[Console]::InputEncoding = [System.Text.Encoding]::UTF8;" ^
  "Write-Host '======================================================' -ForegroundColor Cyan;" ^
  "Write-Host '       NALIVATOR ESP32 - UART TERMINAL (115200)       ' -ForegroundColor Cyan;" ^
  "Write-Host '======================================================' -ForegroundColor Cyan;" ^
  "Write-Host '';" ^
  "$ports = [System.IO.Ports.SerialPort]::GetPortNames();" ^
  "if ($ports.Count -eq 0) {" ^
  "    Write-Host '[-] COM-порты не найдены. Подключите ESP32 по USB.' -ForegroundColor Yellow;" ^
  "    $port = Read-Host 'Введите порт вручную (например COM3)';" ^
  "} elseif ($ports.Count -eq 1) {" ^
  "    $port = $ports[0];" ^
  "    Write-Host ('[+] Найден порт: ' + $port) -ForegroundColor Green;" ^
  "    $ans = Read-Host 'Нажмите Enter для подключения или введите другой';" ^
  "    if ($ans.Trim()) { $port = $ans.Trim() };" ^
  "} else {" ^
  "    Write-Host '[*] Доступные порты:' -ForegroundColor Yellow;" ^
  "    for ($i=0; $i -lt $ports.Count; $i++) { Write-Host (' [' + ($i+1) + '] ' + $ports[$i]) };" ^
  "    $c = Read-Host 'Выберите номер или введите имя порта';" ^
  "    if ($c -match '^\d+$' -and [int]$c -le $ports.Count) { $port = $ports[[int]$c - 1] } else { $port = $c };" ^
  "};" ^
  "$port = $port.ToUpper().Trim();" ^
  "if (-not $port) { exit };" ^
  "Write-Host '';" ^
  "Write-Host ('Подключение к ' + $port + ' (115200 baud)...') -ForegroundColor White;" ^
  "Write-Host 'Для выхода напишите: exit' -ForegroundColor Gray;" ^
  "Write-Host '------------------------------------------------------' -ForegroundColor DarkGray;" ^
  "$sp = New-Object System.IO.Ports.SerialPort($port, 115200, [System.IO.Ports.Parity]::None, 8, [System.IO.Ports.StopBits]::One);" ^
  "$sp.Handshake = [System.IO.Ports.Handshake]::None;" ^
  "$sp.ReadTimeout = 50;" ^
  "$sp.WriteTimeout = 1000;" ^
  "$sp.DtrEnable = $false;" ^
  "$sp.RtsEnable = $false;" ^
  "try {" ^
  "    $sp.Open();" ^
  "    Write-Host ('[+] Порт ' + $port + ' открыт!') -ForegroundColor Green;" ^
  "    Write-Host '------------------------------------------------------' -ForegroundColor DarkGray;" ^
  "} catch {" ^
  "    Write-Host ('[ОШИБКА] Не удалось открыть ' + $port + ': ' + $_.Exception.Message) -ForegroundColor Red;" ^
  "    Write-Host 'Проверьте, не занят ли порт другой программой.' -ForegroundColor Yellow;" ^
  "    Read-Host 'Нажмите Enter для выхода...';" ^
  "    exit;" ^
  "};" ^
  "$running = $true;" ^
  "$reader = [System.Threading.Thread]::new([System.Threading.ThreadStart]{" ^
  "    while ($running) {" ^
  "        try {" ^
  "            if ($sp.IsOpen -and $sp.BytesToRead -gt 0) {" ^
  "                $text = $sp.ReadExisting();" ^
  "                if ($text) { [Console]::Write($text) }" ^
  "            }" ^
  "        } catch {}" ^
  "        [System.Threading.Thread]::Sleep(10);" ^
  "    }" ^
  "});" ^
  "$reader.IsBackground = $true;" ^
  "$reader.Start();" ^
  "while ($sp.IsOpen) {" ^
  "    $line = [Console]::ReadLine();" ^
  "    if ($null -eq $line) { continue };" ^
  "    if ($line.Trim().ToLower() -eq 'exit' -or $line.Trim().ToLower() -eq 'quit') { break };" ^
  "    try {" ^
  "        if ($sp.IsOpen) {" ^
  "            $bytes = [System.Text.Encoding]::UTF8.GetBytes($line + \"`r`n\");" ^
  "            $sp.BaseStream.Write($bytes, 0, $bytes.Length);" ^
  "            $sp.BaseStream.Flush();" ^
  "        }" ^
  "    } catch {" ^
  "        Write-Host ('[Ошибка отправки] ' + $_.Exception.Message) -ForegroundColor Red;" ^
  "    };" ^
  "};" ^
  "$running = $false;" ^
  "$sp.Close();" ^
  "$sp.Dispose();" ^
  "Write-Host 'Порт закрыт.';" ^
  "Read-Host 'Нажмите Enter для выхода...';"
