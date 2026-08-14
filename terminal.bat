@echo off
chcp 65001 >nul
title NALIVATOR ESP32 - Serial Terminal (115200)

powershell.exe -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ports = [System.IO.Ports.SerialPort]::GetPortNames();" ^
  "Write-Host '======================================================' -ForegroundColor Cyan;" ^
  "Write-Host '       NALIVATOR ESP32 - UART TERMINAL (115200)       ' -ForegroundColor Cyan;" ^
  "Write-Host '======================================================' -ForegroundColor Cyan;" ^
  "Write-Host '';" ^
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
  "    if ($c -match '^\d+$' -and [int]$c -ge 1 -and [int]$c -le $ports.Count) { $port = $ports[[int]$c - 1] } else { $port = $c };" ^
  "};" ^
  "$port = $port.ToUpper().Trim();" ^
  "if (-not $port) { exit };" ^
  "Write-Host '';" ^
  "Write-Host ('Подключение к ' + $port + ' (115200 baud)...') -ForegroundColor White;" ^
  "Write-Host 'Для выхода закройте окно или напишите exit' -ForegroundColor Gray;" ^
  "Write-Host '------------------------------------------------------' -ForegroundColor DarkGray;" ^
  "$sp = New-Object System.IO.Ports.SerialPort($port, 115200, [System.IO.Ports.Parity]::None, 8, [System.IO.Ports.StopBits]::One);" ^
  "$sp.Handshake = [System.IO.Ports.Handshake]::None;" ^
  "$sp.DtrEnable = $false;" ^
  "$sp.RtsEnable = $false;" ^
  "try {" ^
  "    $sp.Open();" ^
  "    Write-Host ('[+] Порт ' + $port + ' успешно открыт! Ожидание данных...') -ForegroundColor Green;" ^
  "    Write-Host '------------------------------------------------------' -ForegroundColor DarkGray;" ^
  "} catch {" ^
  "    Write-Host ('[ОШИБКА] ' + $_.Exception.Message) -ForegroundColor Red;" ^
  "    Read-Host 'Нажмите Enter для выхода...';" ^
  "    exit;" ^
  "};" ^
  "$inputBuffer = '';" ^
  "while ($sp.IsOpen) {" ^
  "    try {" ^
  "        while ($sp.BytesToRead -gt 0) {" ^
  "            $chunk = $sp.ReadExisting();" ^
  "            if ($chunk) { [Console]::Write($chunk) }" ^
  "        }" ^
  "    } catch {}" ^
  "    if ([Console]::KeyAvailable) {" ^
  "        $k = [Console]::ReadKey($true);" ^
  "        if ($k.Key -eq [ConsoleKey]::Enter) {" ^
  "            [Console]::WriteLine();" ^
  "            if ($inputBuffer.Trim().ToLower() -eq 'exit') { break };" ^
  "            try {" ^
  "                $b = [System.Text.Encoding]::UTF8.GetBytes($inputBuffer + \"`r`n\");" ^
  "                $sp.BaseStream.Write($b, 0, $b.Length);" ^
  "                $sp.BaseStream.Flush();" ^
  "            } catch {" ^
  "                Write-Host ('[Ошибка отправки] ' + $_.Exception.Message) -ForegroundColor Red;" ^
  "            };" ^
  "            $inputBuffer = '';" ^
  "        } elseif ($k.Key -eq [ConsoleKey]::Backspace) {" ^
  "            if ($inputBuffer.Length -gt 0) {" ^
  "                $inputBuffer = $inputBuffer.Substring(0, $inputBuffer.Length - 1);" ^
  "                [Console]::Write(\"`b `b\");" ^
  "            }" ^
  "        } else {" ^
  "            $c = $k.KeyChar;" ^
  "            if ($c -ne 0) {" ^
  "                $inputBuffer += $c;" ^
  "                [Console]::Write($c);" ^
  "            }" ^
  "        }" ^
  "    }" ^
  "    [System.Threading.Thread]::Sleep(15);" ^
  "};" ^
  "try { $sp.Close(); $sp.Dispose() } catch {};" ^
  "Write-Host ''; Write-Host 'Терминал остановлен.';" ^
  "Read-Host 'Нажмите Enter для выхода...';"
