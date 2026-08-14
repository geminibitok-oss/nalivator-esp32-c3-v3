# NALIVATOR ESP32 UART Terminal
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
[Console]::InputEncoding = [System.Text.Encoding]::UTF8

Write-Host "======================================================" -ForegroundColor Cyan
Write-Host "       NALIVATOR ESP32 - SERIAL TERMINAL (115200)     " -ForegroundColor Cyan
Write-Host "======================================================" -ForegroundColor Cyan
Write-Host ""

# Получение списка доступных COM-портов
$ports = [System.IO.Ports.SerialPort]::GetPortNames()

if ($ports.Count -eq 0) {
    Write-Host "[!] Подключенные COM-порты не обнаружены." -ForegroundColor Yellow
    Write-Host "    Убедитесь, что ESP32 подключена кабелем к USB порту ПК." -ForegroundColor Gray
    Write-Host ""
    $portName = Read-Host "Введите имя порта вручную (например COM3)"
} elseif ($ports.Count -eq 1) {
    $portName = $ports[0]
    Write-Host "[+] Автоматически обнаружен порт: $portName" -ForegroundColor Green
    $userChoice = Read-Host "Использовать $portName? (Нажмите Enter для подтверждения или введите другой)"
    if ($userChoice.Trim().Length -gt 0) {
        $portName = $userChoice.Trim()
    }
} else {
    Write-Host "[*] Доступные COM-порты:" -ForegroundColor Yellow
    for ($i = 0; $i -lt $ports.Count; $i++) {
        Write-Host "  [$($i+1)] $($ports[$i])" -ForegroundColor White
    }
    $choice = Read-Host "Выберите номер порта (1-$($ports.Count)) или введите имя (например COM3)"
    if ($choice -match '^\d+$' -and [int]$choice -ge 1 -and [int]$choice -le $ports.Count) {
        $portName = $ports[[int]$choice - 1]
    } else {
        $portName = $choice.Trim()
    }
}

$portName = $portName.ToUpper().Trim()
if ([string]::IsNullOrWhiteSpace($portName)) {
    Write-Host "[ERROR] Порт не указан." -ForegroundColor Red
    Read-Host "Нажмите Enter для выхода..."
    exit
}

$baudRate = 115200

Write-Host ""
Write-Host "======================================================" -ForegroundColor DarkGray
Write-Host " Подключение к $portName (Скорость: $baudRate)..." -ForegroundColor White
Write-Host " Для справки отправьте: help" -ForegroundColor Gray
Write-Host " Для выхода введите: exit или закройте окно" -ForegroundColor Gray
Write-Host "======================================================" -ForegroundColor DarkGray
Write-Host ""

try {
    $serial = New-Object System.IO.Ports.SerialPort $portName, $baudRate, [System.IO.Ports.Parity]::None, 8, [System.IO.Ports.StopBits]::One
    $serial.ReadTimeout = 500
    $serial.WriteTimeout = 500
    $serial.DtrEnable = $true
    $serial.RtsEnable = $true
    $serial.Open()

    Write-Host "[+] Порт $portName успешно открыт!" -ForegroundColor Green
    Write-Host "------------------------------------------------------" -ForegroundColor DarkGray

    $global:running = $true

    # Фоновый поток для чтения данных из UART
    $readJob = [System.Threading.Thread]::new([System.Threading.ThreadStart]{
        while ($global:running) {
            try {
                if ($serial.IsOpen -and $serial.BytesToRead -gt 0) {
                    $incoming = $serial.ReadExisting()
                    [Console]::Write($incoming)
                }
            } catch {
                # Игнорируем таймауты чтения
            }
            [System.Threading.Thread]::Sleep(20)
        }
    })
    $readJob.IsBackground = $true
    $readJob.Start()

    # Главный цикл для отправки команд пользователем
    while ($serial.IsOpen) {
        $inputLine = [Console]::ReadLine()
        if ($null -eq $inputLine) { continue }
        if ($inputLine.Trim().ToLower() -eq 'exit' -or $inputLine.Trim().ToLower() -eq 'quit') {
            break
        }
        if ($serial.IsOpen) {
            $serial.WriteLine($inputLine)
        }
    }

    $global:running = $false
    $serial.Close()
} catch {
    Write-Host ""
    Write-Host "[ОШИБКА] Не удалось открыть порт $portName :" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Совет: проверьте, не открыт ли этот COM-порт в Arduino IDE или другой программе." -ForegroundColor Gray
}

Write-Host ""
Read-Host "Нажмите Enter для выхода..."
