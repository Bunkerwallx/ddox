Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

# Definir el formulario y sus propiedades
$form = New-Object System.Windows.Forms.Form
$form.Text = "Progreso de Carga"
$form.Size = New-Object System.Drawing.Size(300,150)
$form.StartPosition = "CenterScreen"
$form.BackColor = "White"

# Definir el etiqueta de progreso
$label = New-Object System.Windows.Forms.Label
$label.Location = New-Object System.Drawing.Point(80,20)
$label.Size = New-Object System.Drawing.Size(150,20)
$label.Text = "Cargando..."
$label.BackColor = "White"

# Definir la barra de progreso
$progressBar = New-Object System.Windows.Forms.ProgressBar
$progressBar.Location = New-Object System.Drawing.Point(40,60)
$progressBar.Size = New-Object System.Drawing.Size(200,20)

# Agregar los controles al formulario
$form.Controls.Add($label)
$form.Controls.Add($progressBar)

# Función para actualizar el progreso
function Update-Progress {
    param(
        [int]$Value
    )
    $progressBar.Value = $Value
    $form.Refresh()
}

# Mostrar el formulario
$form.ShowDialog()

# Simular la carga
for ($i = 10; $i -le 100; $i += 10) {
    Start-Sleep -Seconds 1
    Update-Progress -Value $i
}

# Simular la carga completa
Start-Sleep -Seconds 3

# Cerrar 
$Form.Close()
