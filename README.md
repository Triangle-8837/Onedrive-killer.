# OneDrivekiller
A lightweight, zero-dependency native Windows utility for managing OneDrive auto-start behavior and background processes.

---

## 🎯 Overview
OneDrivekiller is a pure C Win32 program under 100KB, no extra runtime libraries or .NET frameworks required. It helps users easily control OneDrive's startup status and terminate background running processes without complex configurations.

## ✨ Key Features
- ✅ &zwnj;**Real-time Status Detection**&zwnj;: Automatically reads Windows registry to identify OneDrive auto-start status, no manual registry query needed
- ✅ &zwnj;**One-Toggle Startup Control**&zwnj;: Modifies OneDrive startup entry through native Windows registry APIs safely, no leftover junk files
- ✅ &zwnj;**Force Process Termination**&zwnj;: Uses system snapshot API to precisely locate all running OneDrive instances and terminate them with one click
- ✅ &zwnj;**Native Lightweight UI**&zwnj;: No third-party GUI libraries introduced, fully compatible with all mainstream Windows versions

## 🚀 Quick Start
1. Download the pre-built `OneDrivekiller.exe` from Releases page
2. Double-click to run directly, no installation required
3. View current OneDrive status on the main interface
4. Click buttons to toggle auto-start or force kill running processes

## ⚠️ Important Notes
- This tool only modifies registry entries under the current user account, it will never touch system-level core configurations
- Please finish syncing all unsynced local files before terminating OneDrive process, to avoid unexpected sync interruptions
- Administrator privilege is not required for most operations, no system permission escalation will be triggered

## 🛠️ Build From Source
You can compile the project manually with MinGW:
```bash
git clone https://github.com/[Triangle-8837]/Onedrive-killer.git
cd Onedrive-killer
mingw32-make
