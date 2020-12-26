#!/bin/bash
header_file="$1"
source_file="$2"
shift 2

scripts="$(dirname "$(readlink -e "$0")")"
first=1

for image in "$@"; do
  args=
  [[ "$first" -eq 0 ]] && args="-a"

  echo "Compiling $image"
  "$scripts/compile_asset.py" $args \
    "$image" "$(basename "$image" .png | tr '-' '_')" \
    "$header_file" \
    "$source_file"

  first=0
done
