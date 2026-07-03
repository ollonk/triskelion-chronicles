#!/usr/bin/env python3

import sys
from pathlib import Path
from PIL import Image

BLANK_PIXEL = 47
AFFINE_TILEMAP_WIDTH = 64
AFFINE_TILEMAP_HEIGHT = 64


def main():
    if len(sys.argv) != 4:
        raise SystemExit(
            "usage: build_affine_8bpp_bg.py input.png output.8bpp output.bin"
        )

    input_path = Path(sys.argv[1])
    tiles_path = Path(sys.argv[2])
    tilemap_path = Path(sys.argv[3])

    image = Image.open(input_path).convert("P")
    width, height = image.size
    if width % 8 != 0 or height % 8 != 0:
        raise SystemExit(f"{input_path}: dimensions must be 8x8 aligned")

    tiles_w = width // 8
    tiles_h = height // 8
    if tiles_w > AFFINE_TILEMAP_WIDTH or tiles_h > AFFINE_TILEMAP_HEIGHT:
        raise SystemExit(
            f"{input_path}: affine tilemap cannot exceed "
            f"{AFFINE_TILEMAP_WIDTH}x{AFFINE_TILEMAP_HEIGHT} tiles"
        )

    blank_tile = bytes([BLANK_PIXEL]) * 64
    tile_indexes = {}
    tile_data = [blank_tile]
    tilemap = bytearray(AFFINE_TILEMAP_WIDTH * AFFINE_TILEMAP_HEIGHT)

    for tile_y in range(tiles_h):
        for tile_x in range(tiles_w):
            tile = image.crop(
                (tile_x * 8, tile_y * 8, tile_x * 8 + 8, tile_y * 8 + 8)
            )
            data = bytes(tile.getdata())
            if data == blank_tile:
                tilemap[tile_y * AFFINE_TILEMAP_WIDTH + tile_x] = 0
                continue
            if data not in tile_indexes:
                tile_indexes[data] = len(tile_data)
                tile_data.append(data)
            tilemap[tile_y * AFFINE_TILEMAP_WIDTH + tile_x] = tile_indexes[data]

    if len(tile_data) > 256:
        raise SystemExit(f"{input_path}: {len(tile_data)} unique tiles exceeds 256")

    tiles_path.write_bytes(b"".join(tile_data))
    tilemap_path.write_bytes(tilemap)


if __name__ == "__main__":
    main()
