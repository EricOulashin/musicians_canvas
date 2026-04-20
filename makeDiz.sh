#!/usr/bin/bash

# Arguments:
# 1: OS

if [ "$1" == "" ]; then
   echo "Missing 1st argumen: Operating system"
   exit
fi

#datetime=$(date '+%Y-%m-%d_%H%M')
#date=$(date '+%Y-%m-%d')
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="${SCRIPT_DIR}"

if [ ! -f "${PROJECT_DIR}/CMakeLists.txt" ]; then
  echo "Error: CMakeLists.txt not found at ${PROJECT_DIR}/CMakeLists.txt"
  exit 1
fi

version=$(
  grep 'project(musicians_canvas VERSION' "${PROJECT_DIR}/CMakeLists.txt" \
    | sed -E 's/.*VERSION ([0-9.]+).*/\1/' \
    | head -n 1
)

if [ "${version}" = "" ]; then
  echo "Error: Could not extract version from CMakeLists.txt"
  exit 1
fi

sed "s/<VERSION>/$version/g" "${PROJECT_DIR}/FILE_ID_Template.DIZ" \
  | sed "s/<OS>/${1}/g" \
  | sed "s/<DATE>/$(date '+%Y-%m-%d')/g" \
  > "${PROJECT_DIR}/FILE_ID.DIZ"

