# Script de reorganisation du projet Tower Defence
# Lance depuis la racine du projet : .\reorganise.ps1

$root = Get-Location

# ============================================================
# 1. CREATION DES DOSSIERS
# ============================================================
$folders = @(
    "src/ressource",
    "src/gameManager",
    "src/Map",
    "src/mob/logic",
    "src/mob/view",
    "src/gestionVague",
    "src/castle/logic",
    "src/castle/view",
    "src/menu",
    "src/ui",
    "src/controller",
    "src/score",
    "assets/Skeleton_Crew"
)

foreach ($f in $folders) {
    New-Item -ItemType Directory -Force -Path "$root/$f" | Out-Null
}
Write-Host "Dossiers crees." -ForegroundColor Green

# ============================================================
# 2. DEPLACEMENT DES FICHIERS DU COEQUIER (view/ controller/ model/)
# ============================================================

# menu/
$menuFiles = @("menu", "param", "tabScores", "cloud")
foreach ($f in $menuFiles) {
    if (Test-Path "view/include/$f.hpp") { Move-Item "view/include/$f.hpp" "src/menu/$f.hpp" -Force }
    if (Test-Path "view/src/$f.cpp")     { Move-Item "view/src/$f.cpp"     "src/menu/$f.cpp" -Force }
}

# ui/
$uiFiles = @("bouton", "store", "towerCard", "window", "soundManager")
foreach ($f in $uiFiles) {
    if (Test-Path "view/include/$f.hpp") { Move-Item "view/include/$f.hpp" "src/ui/$f.hpp" -Force }
    if (Test-Path "view/src/$f.cpp")     { Move-Item "view/src/$f.cpp"     "src/ui/$f.cpp" -Force }
}

# Tour (dans ui aussi)
if (Test-Path "view/include/Tour.hpp") { Move-Item "view/include/Tour.hpp" "src/ui/Tour.hpp" -Force }
if (Test-Path "view/src/Tour.cpp")     { Move-Item "view/src/Tour.cpp"     "src/ui/Tour.cpp" -Force }

# controller/
if (Test-Path "controller/include/controlManager.hpp") { Move-Item "controller/include/controlManager.hpp" "src/controller/controlManager.hpp" -Force }
if (Test-Path "controller/src/controlManager.cpp")     { Move-Item "controller/src/controlManager.cpp"     "src/controller/controlManager.cpp" -Force }
if (Test-Path "controller/include/menuManager.hpp")    { Move-Item "controller/include/menuManager.hpp"    "src/controller/menuManager.hpp" -Force }
if (Test-Path "controller/src/menuManager.cpp")        { Move-Item "controller/src/menuManager.cpp"        "src/controller/menuManager.cpp" -Force }
if (Test-Path "controller/include/Score.hpp")          { Move-Item "controller/include/Score.hpp"          "src/score/Score.hpp" -Force }
if (Test-Path "controller/src/Score.cpp")              { Move-Item "controller/src/Score.cpp"              "src/score/Score.cpp" -Force }

# score/
if (Test-Path "model/include/scoreLoader.hpp") { Move-Item "model/include/scoreLoader.hpp" "src/score/scoreLoader.hpp" -Force }
if (Test-Path "model/src/scoreLoader.cpp")     { Move-Item "model/src/scoreLoader.cpp"     "src/score/scoreLoader.cpp" -Force }

Write-Host "Fichiers du coequier deplaces." -ForegroundColor Green

# ============================================================
# 3. DEPLACEMENT DES ASSETS (asset/ -> assets/)
# ============================================================
if (Test-Path "asset/Skeleton_Crew") {
    Copy-Item "asset/Skeleton_Crew" "assets/Skeleton_Crew" -Recurse -Force
    Write-Host "Skeleton_Crew copie dans assets/." -ForegroundColor Green
}

# ============================================================
# 4. MISE A JOUR DES CHEMINS D'INCLUDE
# ============================================================
Write-Host "Mise a jour des includes..." -ForegroundColor Yellow

# Regles de remplacement : ancien include -> nouveau include
$replacements = @(
    # Anciens chemins view/include
    @{ old = '#include "bouton.hpp"';        new = '#include "../ui/bouton.hpp"' },
    @{ old = '#include "store.hpp"';         new = '#include "../ui/store.hpp"' },
    @{ old = '#include "towerCard.hpp"';     new = '#include "../ui/towerCard.hpp"' },
    @{ old = '#include "window.hpp"';        new = '#include "../ui/window.hpp"' },
    @{ old = '#include "soundManager.hpp"';  new = '#include "../ui/soundManager.hpp"' },
    @{ old = '#include "Tour.hpp"';          new = '#include "../ui/Tour.hpp"' },
    @{ old = '#include "menu.hpp"';          new = '#include "../menu/menu.hpp"' },
    @{ old = '#include "param.hpp"';         new = '#include "../menu/param.hpp"' },
    @{ old = '#include "tabScores.hpp"';     new = '#include "../menu/tabScores.hpp"' },
    @{ old = '#include "cloud.hpp"';         new = '#include "../menu/cloud.hpp"' },
    @{ old = '#include "controlManager.hpp"'; new = '#include "../controller/controlManager.hpp"' },
    @{ old = '#include "menuManager.hpp"';   new = '#include "../controller/menuManager.hpp"' },
    @{ old = '#include "Score.hpp"';         new = '#include "../score/Score.hpp"' },
    @{ old = '#include "scoreLoader.hpp"';   new = '#include "../score/scoreLoader.hpp"' },
    # Chemins avec sous-dossiers
    @{ old = '#include "../include/Tour.hpp"'; new = '#include "../ui/Tour.hpp"' },
    # Assets
    @{ old = '"asset/Skeleton_Crew';         new = '"assets/Skeleton_Crew' }
)

# Applique les remplacements sur tous les .cpp et .hpp dans src/
$files = Get-ChildItem -Recurse -Path "src/" -Include "*.cpp","*.hpp"
foreach ($file in $files) {
    $content = Get-Content $file.FullName -Raw -Encoding UTF8
    $modified = $false
    foreach ($r in $replacements) {
        if ($content -match [regex]::Escape($r.old)) {
            $content = $content -replace [regex]::Escape($r.old), $r.new
            $modified = $true
        }
    }
    if ($modified) {
        Set-Content $file.FullName $content -Encoding UTF8 -NoNewline
        Write-Host "  Mis a jour : $($file.Name)" -ForegroundColor Cyan
    }
}

# ============================================================
# 5. CREATION DE GameState.hpp
# ============================================================
$gameState = @"
#pragma once

enum class GameState {
    Menu,
    InGame,
    Pause,
    GameOver,
    Victory
};
"@
Set-Content "src/GameState.hpp" $gameState -Encoding UTF8
Write-Host "GameState.hpp cree." -ForegroundColor Green

# ============================================================
# 6. SUPPRESSION DES DOSSIERS VIDES
# ============================================================
$toRemove = @("view", "controller", "model", "asset")
foreach ($d in $toRemove) {
    if (Test-Path $d) {
        $remaining = Get-ChildItem -Recurse -Path $d -File
        if ($remaining.Count -eq 0) {
            Remove-Item $d -Recurse -Force
            Write-Host "Dossier $d supprime (vide)." -ForegroundColor DarkGray
        } else {
            Write-Host "Dossier $d non supprime - contient encore $($remaining.Count) fichier(s)." -ForegroundColor Yellow
        }
    }
}

Write-Host ""
Write-Host "Reorganisation terminee !" -ForegroundColor Green
Write-Host "Verifie les includes manuellement si des erreurs de compilation apparaissent." -ForegroundColor Yellow
