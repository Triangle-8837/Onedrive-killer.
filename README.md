# Onedrive-killer.
📋 Core Overview
A lightweight, zero-dependency native Windows utility built purely in C, designed to help users easily manage OneDrive's auto-start behavior and background process status without complex configurations. The final executable is under 100KB, no extra runtime libraries or .NET frameworks are required, you can run it directly with a double-click.

🔍 Key Features
✅ ‌Auto Status Detection‌: Reads system registry in real time to identify whether OneDrive is set to launch on system startup, no manual registry query needed.
✅ ‌One-Toggle Startup Control‌: Modifies OneDrive's auto-start entry through native Windows registry APIs safely, leaving no leftover junk files on your system.
✅ ‌Force Process Termination‌: Uses Windows snapshot API to precisely locate all running OneDrive processes, and terminates all background instances with one click.
✅ ‌Native Win32 UI‌: No third-party GUI libraries are introduced, the interface is lightweight and responsive, fully compatible with all mainstream Windows versions.
⚠️ Important Notes
This tool only modifies registry entries under the current user account, it will never touch system-level core configurations or affect OneDrive settings for other user accounts on the same device.
It is highly recommended to finish syncing all unsynced local files before terminating the OneDrive process, to avoid unexpected sync interruptions and potential data loss.
