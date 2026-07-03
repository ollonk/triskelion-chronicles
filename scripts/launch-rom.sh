#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
emulator="$repo_root/tools/emulators/mgba/mGBA.AppImage"
rom="$repo_root/pokeemerald.gba"
log_file="${TMPDIR:-/tmp}/triskelion-mgba.log"
build=1
detach=0

while [[ $# -gt 0 ]]; do
  case "$1" in
    --detach)
      detach=1
      ;;
    --no-build)
      build=0
      ;;
    *)
      echo "Unknown argument: $1" >&2
      exit 2
      ;;
  esac
  shift
done

if [[ ! -x "$emulator" ]]; then
  echo "mGBA AppImage is missing or not executable: $emulator" >&2
  exit 1
fi

if [[ "$build" -eq 1 ]]; then
  make -C "$repo_root" -j"$(nproc)"
elif [[ ! -f "$rom" ]]; then
  echo "ROM is missing: $rom" >&2
  exit 1
fi

echo "Launching mGBA:"
echo "  emulator: $emulator"
echo "  rom:      $rom"

if [[ "$detach" -eq 1 ]]; then
  nohup "$emulator" "$rom" >"$log_file" 2>&1 &
  echo "  pid:      $!"
  echo "  log:      $log_file"
  exit 0
fi

exec "$emulator" "$rom"
