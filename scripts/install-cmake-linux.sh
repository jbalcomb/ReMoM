#!/usr/bin/env bash
# install-cmake-linux.sh — Install latest CMake from Kitware's official binaries
#
# Usage:
#   ./scripts/install-cmake-linux.sh              # install latest to /opt/cmake
#   ./scripts/install-cmake-linux.sh 3.31.6       # install specific version
#   INSTALL_DIR=/usr/local ./scripts/install-cmake-linux.sh  # custom prefix
#
# After install, add to PATH:
#   export PATH="/opt/cmake/bin:$PATH"
# Or add that line to ~/.bashrc / ~/.profile for persistence.

set -euo pipefail

CMAKE_VERSION="${1:-}"
INSTALL_DIR="${INSTALL_DIR:-/opt/cmake}"
ARCH="$(uname -m)"

# Map architecture to Kitware's naming
case "$ARCH" in
    x86_64)  CMAKE_ARCH="x86_64" ;;
    aarch64) CMAKE_ARCH="aarch64" ;;
    *)
        echo "ERROR: Unsupported architecture: $ARCH"
        exit 1
        ;;
esac

# If no version specified, query the latest release from GitHub
if [ -z "$CMAKE_VERSION" ]; then
    echo "Querying latest CMake release..."
    CMAKE_VERSION=$(curl -fsSL "https://api.github.com/repos/Kitware/CMake/releases/latest" \
        | grep '"tag_name"' \
        | sed -E 's/.*"v([^"]+)".*/\1/')
    if [ -z "$CMAKE_VERSION" ]; then
        echo "ERROR: Could not determine latest CMake version from GitHub API."
        echo "Try specifying a version manually: $0 3.31.6"
        exit 1
    fi
fi

TARBALL="cmake-${CMAKE_VERSION}-linux-${CMAKE_ARCH}.tar.gz"
URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${TARBALL}"

echo "============================================"
echo "  CMake installer for Linux"
echo "============================================"
echo "  Version:     ${CMAKE_VERSION}"
echo "  Arch:        ${CMAKE_ARCH}"
echo "  Install dir: ${INSTALL_DIR}"
echo "  URL:         ${URL}"
echo "============================================"
echo ""

# Check for curl
if ! command -v curl &>/dev/null; then
    echo "ERROR: curl is required. Install it with: sudo apt install curl"
    exit 1
fi

# Download to a temp directory
TMPDIR="$(mktemp -d)"
trap 'rm -rf "$TMPDIR"' EXIT

echo "Downloading ${TARBALL}..."
curl -fSL --progress-bar -o "${TMPDIR}/${TARBALL}" "${URL}"

echo "Extracting..."
tar -xzf "${TMPDIR}/${TARBALL}" -C "${TMPDIR}"

# The tarball extracts to cmake-VERSION-linux-ARCH/
EXTRACTED_DIR="${TMPDIR}/cmake-${CMAKE_VERSION}-linux-${CMAKE_ARCH}"
if [ ! -d "$EXTRACTED_DIR" ]; then
    echo "ERROR: Expected directory ${EXTRACTED_DIR} not found after extraction."
    exit 1
fi

# Install — needs sudo if installing to /opt
if [ -w "$(dirname "$INSTALL_DIR")" ]; then
    rm -rf "$INSTALL_DIR"
    mv "$EXTRACTED_DIR" "$INSTALL_DIR"
else
    echo "Need sudo to install to ${INSTALL_DIR}..."
    sudo rm -rf "$INSTALL_DIR"
    sudo mv "$EXTRACTED_DIR" "$INSTALL_DIR"
fi

# Verify
echo ""
echo "Installed:"
"${INSTALL_DIR}/bin/cmake" --version
echo ""

# Check if it's on PATH
if ! command -v cmake &>/dev/null || [ "$(command -v cmake)" != "${INSTALL_DIR}/bin/cmake" ]; then
    echo "NOTE: ${INSTALL_DIR}/bin is not on your PATH (or another cmake takes priority)."
    echo ""
    echo "Add this to your ~/.bashrc or ~/.profile:"
    echo ""
    echo "    export PATH=\"${INSTALL_DIR}/bin:\$PATH\""
    echo ""
    echo "Then reload with:  source ~/.bashrc"
fi

echo "Done."
