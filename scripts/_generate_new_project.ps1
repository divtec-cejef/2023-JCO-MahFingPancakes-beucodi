<#
.SYNOPSIS
Script qui génère, à partir d'un dépôt git du projet GameFramework, un nouveau squelette de projet vide.

.DESCRIPTION
Ce script génère une copie du dépôt git actuel de GameFramework et modifie les fichiers nécessaires afin
d'obtenir un projet vide, mais fonctionnel et prêt pour le développement d'un jeu.

Le nom du nouveau projet et son emplacement peuvent être fournis en paramètre. Si le script est démarré sans
paramètre, une boîte de dialogue s'ouvre pour que l'utilisateur puisse saisir ces données.

Pour que ce script fonctionne il faut qu'il soit lancé depuis un dépôt Git.

.PARAMETER ProjectName
Nom du projet (ou du nouveau jeu)

.PARAMETER SelectedFolderName
Emplacement où sera créé l'arborescence des fichiers du nouveau projet.

.PARAMETER CreateProjectFolder
Si cette option est activée, un dossier contenant tous les fichiers du projet est créé dans le dossier
SelectedFolderName. Si elle est désactivée, les fichiers du projet sont directement copiés dans le dossier
SelectedFolderName.

.NOTES
Auteur : JCO
LastEdit : 16.5.2023

.LINK
https://divtec.ch
#>
param(
    [string]$ProjectName,
    [string]$SelectedFolderName,
	[switch]$CreateProjectFolder=$False
)

Set-StrictMode -Version Latest

function Test-ValidFileName
{
    param([string]$FileName)

    $IndexOfInvalidChar = $FileName.IndexOfAny([System.IO.Path]::GetInvalidFileNameChars())

    # IndexOfAny() returns the value -1 to indicate no such character was found
    return $IndexOfInvalidChar -eq -1
}

# On va commencer par vérifier si ce script est dans un dépôt Git
if (-Not (git rev-parse --is-inside-work-tree 2> $Null)) {
	Write-Error "Ce dossier n'est pas sous contrôle de version. Pour que ce script fonctionne, il faut qu'il soit démarré depuis un dossier cloné depuis GitHub."
	
	Exit 0
}

Push-Location
set-Location .. # Il faut se positionner à la racine du projet et non pas rester dans le dossier 'scripts'


if ((-Not $ProjectName) -or (-Not $SelectedFolderName)) {

    Write-Host -ForegroundColor Yellow "Affichage de la boîte de saisie des informations"
     
    Add-Type -assembly System.Windows.Forms

    # Création de la fenêtre principale
    $main_form = New-Object System.Windows.Forms.Form
    $main_form.Text = "Création d'un nouveau projet basé sur GameFramework"
    $main_form.Width = 470
    $main_form.Height = 150
    $main_form.AutoSize = $true

    # Ajout d'un label
    $Label = New-Object System.Windows.Forms.Label
    $Label.Text = "Nom du nouveau projet : "
    $Label.Location  = New-Object System.Drawing.Point(0,10)
    $Label.AutoSize = $true
    $main_form.Controls.Add($Label)

    # Ajout d'une textbox
    $TB_ProjectName = New-Object System.Windows.Forms.TextBox
    $TB_ProjectName.Width = 200
    $TB_ProjectName.Location  = New-Object System.Drawing.Point(140,10)
    $TB_ProjectName.Text = $ProjectName
    $main_form.Controls.Add($TB_ProjectName)


    $Label2 = New-Object System.Windows.Forms.Label
    $Label2.Text = "Emplacement : "
    $Label2.Location  = New-Object System.Drawing.Point(0,40)
    $Label2.AutoSize = $true
    $main_form.Controls.Add($Label2)

    $Label3 = New-Object System.Windows.Forms.Label
    $Label3.Text = "Tous les fichiers du projet seront placés dans ce dossier."
    $Label3.Location  = New-Object System.Drawing.Point(0,70)
    $Label3.AutoSize = $true
    $main_form.Controls.Add($Label3)

    # Ajout d'une textbox
    $TB_Location = New-Object System.Windows.Forms.TextBox
    $TB_Location.Width = 200
    $TB_Location.Location  = New-Object System.Drawing.Point(140,40)
    $TB_Location.Text = $SelectedFolderName
    $main_form.Controls.Add($TB_Location)

    # Ajout d'un bouton de sélection de dossier
    $BtnSelectFolder = New-Object System.Windows.Forms.Button
    $BtnSelectFolder.Location = New-Object System.Drawing.Size(350,40)
    $BtnSelectFolder.Size = New-Object System.Drawing.Size(40,23)
    $BtnSelectFolder.Text = "..."
    $main_form.Controls.Add($BtnSelectFolder)
    $BtnSelectFolder.Add_Click(

        {
            $shell = New-Object -ComObject Shell.Application

            $selectedfolder = $shell.BrowseForFolder( 0, 'Choisir un dossier où sera créé le projet', 16, $HOME).Self.Path

            $TB_Location.Text = $selectedfolder
        }

    )

    $BtnCreateProject = New-Object System.Windows.Forms.Button
    $BtnCreateProject.Location = New-Object System.Drawing.Size(350,70)
    $BtnCreateProject.Size = New-Object System.Drawing.Size(80,23)
    $BtnCreateProject.Text = "Créer"
    $main_form.Controls.Add($BtnCreateProject)
    $BtnCreateProject.Add_Click(
    {
        $main_form.Close()
    }
    )

    $result = $main_form.ShowDialog() > $Null

Write-Host -foregroundcolor cyan $result
    $SelectedFolderName = $TB_Location.Text
    $ProjectName = $TB_ProjectName.Text
}

if (-Not $ProjectName -or -Not $SelectedFolderName) {

	Write-Warning "User canceled"
	
	Exit 0
}

# données de test
#$SelectedFolderName = "C:\Users\conujer.DIVTEC\Documents\work\tmp"
#$ProjectName = "test_gf"

Write-Host -ForegroundColor Yellow "Démarrage de la création du projet."

if (-Not (Test-Path $SelectedFolderName -PathType Container)) {

    Write-Error "Le dossier de destination n'existe pas."
    Pop-Location
    Exit 1

}

if (-Not (Test-ValidFileName $ProjectName)) {

    Write-Error "Le nom du projet ($ProjectName) est invalide."
    Pop-Location
    Exit 1
}

if ($CreateProjectFolder) {
	# Option de création d'un dossier activée : dans le dossier choisi par l'utilisateur ($SelectedFolderName),
	# un sous-dossier pour le projet est créé, dans lequel se trouveront l'arborescence du projet.
	$FullClonePath = Join-Path $SelectedFolderName $ProjectName
	if (Test-Path $FullClonePath -PathType Any) {

		Write-Error "Le projet '$ProjectName' existe déjà."
		Pop-Location
		Exit 1

	}
} else {
	# Option de création d'un dossier désactivée : l'arborescende du projet est directement copiée dans
	# le dossier choisir par l'utilisateur ($SelectedFolderName).
	# C'est cette solution qu'il faudrait utiliser si le dossier sélectioné est le dossier Git du projet.
	$FullClonePath = $SelectedFolderName
	if (Test-Path $FullClonePath -PathType Any) {

		Write-Warning "Le projet '$ProjectName' existe déjà."
	}
}

$ArchiveName = "_latest.zip"

# Clonage du dépôt
# L'appel à la commande git est révélateur d'un problème intéressant avec l'usage des canaux.
# Cet article stackoverflow donne une explication très intéressante : https://stackoverflow.com/a/59941191
# C'est son approche pragmatique que j'ai implémentée ici :

# Invoke git and capture both its stdout and stderr streams, as strings.
#$result = git clone https://github.com/divtec-cejef/2018-JCO-GameFramework.git $FullClonePath 2>&1 | % ToString
$result = git archive -o $ArchiveName HEAD

# Throw an error, if git indicated failure.
if ($LASTEXITCODE) { Throw "git failed (exit code: $LASTEXITCODE):`n$($result -join "`n")" }                                            

# Output the captured result, as an array of lines.
$result

Expand-Archive $ArchiveName -DestinationPath $FullClonePath -Force

Remove-Item $ArchiveName

# Suppression des fichiers inutiles
Set-Location $FullClonePath
Set-Location src

# Il serait plus logique (mais un peu plus compliqué) d'indiquer plutôt les fichiers à garder
Remove-Item automaticwalkinghandler.*,blueball.*,bouncingspritehandler.*,bullettrajectorytickhandler.*,keytickhandler.*,manualwalkinghandler.*, `
            painterpathtickhandler.*,randommovetickhandler.*,spinningpinwheel.*,walkingman.*, gamecore.h, gamecore.cpp, main.cpp, GameFramework.pro

#Remove-Item ..\res\images\demo -Recurse
#Remove-Item ..\res\images\tutorial -Recurse
Remove-Item ..\res\sounds\demo -Recurse
Remove-Item ..\doc\*.docx
Remove-Item ..\doc\*.vsdx

# Edition du pro et du main_blank.cpp pour y intégrer le nom du projet
((Get-Content BlankProjectTemplate.pro -Raw) -replace "{{ProjectName}}", "$ProjectName") | Set-Content BlankProjectTemplate.pro
((Get-Content main_blank.cpp -Raw) -replace "{{ProjectName}}", "$ProjectName") | Set-Content main_blank.cpp

# Edition du Doxyfile pour y intégrer le nom du projet
((Get-Content Doxyfile -Raw) -replace "GameFramework", "$ProjectName") | Set-Content Doxyfile

# Renommage de certains fichiers
$ProjectFileName = "$ProjectName.pro"
Rename-Item BlankProjectTemplate.pro $ProjectFileName

Rename-Item gamecore_blank.h gamecore.h
Rename-Item gamecore_blank.cpp gamecore.cpp
Rename-Item main_blank.cpp main.cpp

# Génération doxygen
Get-Location
#.\..\scripts\generate_codedoc.bat

Pop-Location

Exit 0
