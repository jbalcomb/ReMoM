#!/usr/bin/env bash
# install-sdl3-linux.sh — Build and install SDL3 from source on Linux
#
# Usage:
#   ./scripts/install-sdl3-linux.sh              # install v3.4.2 (project default)
#   ./scripts/install-sdl3-linux.sh 3.4.2        # install specific version
#   INSTALL_DIR=/usr/local ./scripts/install-sdl3-linux.sh  # custom prefix
#
# After install, CMake will find SDL3 automatically if installed to /usr/local (default).
# If you use a custom INSTALL_DIR, pass it to CMake:
#   cmake -DCMAKE_PREFIX_PATH=/your/path ...

set -euo pipefail

SDL3_VERSION="${1:-3.4.2}"
INSTALL_DIR="${INSTALL_DIR:-/usr/local}"
JOBS="${JOBS:-$(nproc)}"

TARBALL="SDL3-${SDL3_VERSION}.tar.gz"
URL="https://github.com/libsdl-org/SDL/releases/download/release-${SDL3_VERSION}/SDL3-${SDL3_VERSION}.tar.gz"

echo "============================================"
echo "  SDL3 installer for Linux"
echo "============================================"
echo "  Version:     ${SDL3_VERSION}"
echo "  Install dir: ${INSTALL_DIR}"
echo "  Build jobs:  ${JOBS}"
echo "  URL:         ${URL}"
echo "============================================"
echo ""

# Check build dependencies
MISSING_DEPS=()
for cmd in cmake gcc g++ make curl; do
    if ! command -v "$cmd" &>/dev/null; then
        MISSING_DEPS+=("$cmd")
    fi
done
if [ ${#MISSING_DEPS[@]} -gt 0 ]; then
    echo "ERROR: Missing required tools: ${MISSING_DEPS[*]}"
    echo ""
    echo "Install them with:"
    echo "  sudo apt install build-essential cmake curl"
    exit 1
fi

# Recommend common SDL3 system dependencies
echo "Make sure you have the SDL3 build dependencies installed:"
echo "  sudo apt install build-essential cmake curl pkg-config \\"
echo "    libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev \\"
echo "    libxfixes-dev libxss-dev libwayland-dev libxkbcommon-dev \\"
echo "    libegl1-mesa-dev libgles2-mesa-dev libgl1-mesa-dev \\"
echo "    libpulseaudio-dev libasound2-dev libpipewire-0.3-dev \\"
echo "    libdbus-1-dev libudev-dev libdecor-0-dev"
echo ""
# When piped (curl | bash), stdin is the script itself, so read from /dev/tty
if [ -t 0 ]; then
    read -rp "Continue with build? [Y/n] " REPLY
else
    read -rp "Continue with build? [Y/n] " REPLY </dev/tty || REPLY="y"
fi
if [[ "$REPLY" =~ ^[Nn] ]]; then
    echo "Aborted."
    exit 0
fi

# Download to a temp directory
TMPDIR="$(mktemp -d)"
trap 'rm -rf "$TMPDIR"' EXIT

echo ""
echo "Downloading SDL3 ${SDL3_VERSION}..."
curl -fSL --progress-bar -o "${TMPDIR}/${TARBALL}" "${URL}"

echo "Extracting..."
tar -xzf "${TMPDIR}/${TARBALL}" -C "${TMPDIR}"

SRCDIR="${TMPDIR}/SDL3-${SDL3_VERSION}"
if [ ! -d "$SRCDIR" ]; then
    echo "ERROR: Expected directory ${SRCDIR} not found after extraction."
    exit 1
fi

BUILDDIR="${TMPDIR}/build"
mkdir -p "$BUILDDIR"

echo ""
echo "Configuring..."
cmake -S "$SRCDIR" -B "$BUILDDIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
    -DSDL_SHARED=ON \
    -DSDL_STATIC=OFF \
    -DSDL_TEST_LIBRARY=OFF \
    -DSDL_TESTS=OFF

echo ""
echo "Building with ${JOBS} jobs..."
cmake --build "$BUILDDIR" -j "$JOBS"

echo ""
echo "Installing to ${INSTALL_DIR}..."
if [ -w "$INSTALL_DIR" ] || [ -w "$(dirname "$INSTALL_DIR")" ]; then
    cmake --install "$BUILDDIR"
else
    sudo cmake --install "$BUILDDIR"
fi

# Update ldconfig so the shared lib is found at runtime
if command -v ldconfig &>/dev/null; then
    echo "Running ldconfig..."
    sudo ldconfig
fi

echo ""
echo "Installed SDL3 ${SDL3_VERSION} to ${INSTALL_DIR}"
echo ""

# Verify CMake can find it
echo "Verifying CMake can find SDL3..."
VERIFY_DIR="$(mktemp -d)"
cat > "${VERIFY_DIR}/CMakeLists.txt" <<'CMEOF'
cmake_minimum_required(VERSION 3.15)
project(sdl3check C)
find_package(SDL3 REQUIRED)
message(STATUS "SDL3 found: ${SDL3_VERSION}")
CMEOF
if cmake -S "$VERIFY_DIR" -B "${VERIFY_DIR}/build" -DCMAKE_PREFIX_PATH="$INSTALL_DIR" 2>&1 | grep -q "SDL3 found"; then
    echo "  OK — CMake finds SDL3 at ${INSTALL_DIR}"
else
    echo "  WARNING — CMake could not find SDL3. You may need to pass:"
    echo "    -DCMAKE_PREFIX_PATH=${INSTALL_DIR}"
fi
rm -rf "$VERIFY_DIR"

echo ""
echo "Done."
