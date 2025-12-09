$maxAttempts = 30
$filePath = "d:\Trae CN\InnovativePracticeProject-main\src\Enemy.cpp"

for ($i = 0; $i -lt $maxAttempts; $i++) {
    try {
        [System.IO.File]::Delete($filePath)
        Write-Host "File deleted successfully"
        exit 0
    } catch {
        Start-Sleep -Seconds 1
        Write-Host "Attempt $($i+1): File still in use, retrying..."
    }
}
Write-Host "Failed to delete file after $maxAttempts attempts"
