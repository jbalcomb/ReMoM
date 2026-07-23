# `packaging/`

Files consumed by the Linux packaging targets. Single source of truth — both the
AppImage build and the Debian package install these, so the two cannot drift.

| File | Used by | Purpose |
|------|---------|---------|
| `AppRun` | AppImage (`linuxdeploy --custom-apprun`) | Launcher with portable-mode detection. See [`doc/#Devel/Devel-Linux-AppImage.md`](../doc/%23Devel/Devel-Linux-AppImage.md). |
| `remom.desktop` | AppImage + `.deb` (`/usr/share/applications/`) | Desktop menu entry. |
| `remom.png` | AppImage + `.deb` (`/usr/share/icons/hicolor/256x256/apps/`) | Application icon, 256x256. |

> **`remom.png` is a placeholder** — a flat 256x256 image, promoted from the
> generated blob that used to live inline in `release.yml`. It exists so the icon
> is a tracked, reviewable file at a size `linuxdeploy` accepts. Replace it with
> real artwork at the same path and size; nothing else needs to change.

Background: [`PRD-Linux-Package-Install.md`](../doc/%23AI_Plans/PRD-Linux-Package-Install.md).
