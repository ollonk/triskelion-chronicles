#!/usr/bin/env python3
import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MAPS_DIR = ROOT / "data" / "maps"
LAYOUTS_PATH = ROOT / "data" / "layouts" / "layouts.json"
MAP_GROUPS_PATH = MAPS_DIR / "map_groups.json"
REGION_MAP_SECTIONS_PATH = ROOT / "src" / "data" / "region_map" / "region_map_sections.json"


def load_json(path):
    with path.open(encoding="utf-8") as f:
        return json.load(f)


def normalize_map_id(map_id):
    return map_id.removeprefix("MAP_")


def file_size(path):
    return path.stat().st_size if path.exists() else None


def main():
    errors = []
    warnings = []

    layouts_data = load_json(LAYOUTS_PATH)
    layouts = {layout["id"]: layout for layout in layouts_data["layouts"]}

    region_data = load_json(REGION_MAP_SECTIONS_PATH)
    region_sections = {section["map_section"] for section in region_data["map_sections"]}

    map_dirs = sorted(path.parent for path in MAPS_DIR.glob("*/map.json"))
    maps_by_id = {}
    maps_by_name = {}
    for map_dir in map_dirs:
        map_data = load_json(map_dir / "map.json")
        maps_by_id[map_data["id"]] = map_data
        maps_by_name[map_data["name"]] = map_data

    map_groups = load_json(MAP_GROUPS_PATH)
    grouped_map_ids = set()
    for group_name, entries in map_groups.items():
        if group_name == "group_order":
            continue
        for entry in entries:
            if entry in maps_by_name:
                grouped_map_ids.add(maps_by_name[entry]["id"])
            else:
                grouped_map_ids.add(f"MAP_{entry.upper()}")

    for map_id, map_data in sorted(maps_by_id.items()):
        name = map_data["name"]
        map_path = MAPS_DIR / name / "map.json"

        if map_id not in grouped_map_ids:
            warnings.append(f"{name}: present on disk but missing from map_groups.json")

        layout_id = map_data["layout"]
        layout = layouts.get(layout_id)
        if layout is None:
            errors.append(f"{name}: unknown layout {layout_id}")
        else:
            blockdata_path = ROOT / layout["blockdata_filepath"]
            border_path = ROOT / layout["border_filepath"]
            expected_blockdata_size = layout["width"] * layout["height"] * 2
            actual_blockdata_size = file_size(blockdata_path)
            if actual_blockdata_size is None:
                errors.append(f"{name}: missing blockdata {blockdata_path.relative_to(ROOT)}")
            elif actual_blockdata_size != expected_blockdata_size:
                errors.append(
                    f"{name}: blockdata size {actual_blockdata_size} != expected {expected_blockdata_size} "
                    f"({layout['width']}x{layout['height']}x2)"
                )
            if not border_path.exists():
                errors.append(f"{name}: missing border {border_path.relative_to(ROOT)}")

        region_map_section = map_data.get("region_map_section")
        if region_map_section and region_map_section not in region_sections:
            errors.append(f"{name}: region_map_section {region_map_section} missing from region_map_sections.json")

        connections = map_data.get("connections")
        if isinstance(connections, list):
            for connection in connections:
                dest_map = connection["map"]
                if dest_map not in maps_by_id:
                    errors.append(f"{name}: connection to unknown {dest_map}")

        for i, warp in enumerate(map_data.get("warp_events") or []):
            dest_map = warp["dest_map"]
            if dest_map == "MAP_DYNAMIC":
                continue
            if dest_map not in maps_by_id:
                errors.append(f"{name}: warp {i} points to unknown {dest_map}")
                continue

            dest_warp_id = warp["dest_warp_id"]
            if isinstance(dest_warp_id, str) and dest_warp_id.isdigit():
                dest_warp_index = int(dest_warp_id)
                dest_warps = maps_by_id[dest_map].get("warp_events") or []
                if dest_warp_index >= len(dest_warps):
                    errors.append(
                        f"{name}: warp {i} points to {dest_map} warp {dest_warp_index}, "
                        f"but destination has {len(dest_warps)} warps"
                    )

            if layout is not None:
                x = int(warp["x"])
                y = int(warp["y"])
                if x < 0 or y < 0 or x >= layout["width"] or y >= layout["height"]:
                    errors.append(f"{name}: warp {i} at ({x}, {y}) outside layout {layout_id}")

        for event_type in ("object_events", "coord_events", "bg_events"):
            for i, event in enumerate(map_data.get(event_type) or []):
                if layout is None:
                    continue
                x = int(event["x"])
                y = int(event["y"])
                if x < 0 or y < 0 or x >= layout["width"] or y >= layout["height"]:
                    errors.append(f"{name}: {event_type} {i} at ({x}, {y}) outside layout {layout_id}")

    print(f"Audited {len(maps_by_id)} maps and {len(layouts)} layouts.")
    if warnings:
        print("\nWarnings:")
        for warning in warnings:
            print(f"  - {warning}")
    if errors:
        print("\nErrors:")
        for error in errors:
            print(f"  - {error}")
        return 1

    print("No map data errors found.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
