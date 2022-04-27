#!/bin/bash

abort() {
  echo "ERROR: $1!"
  exit 1
}

# Avoid conflicts with PATH overrides.
CAT="/bin/cat"
CHMOD="/bin/chmod"
CURL="/usr/bin/curl"
FIND="/usr/bin/find"
MKDIR="/bin/mkdir"
MV="/bin/mv"
RM="/bin/rm"
TAR="/usr/bin/tar"
UNZIP="/usr/bin/unzip"

TOOLS=(
  "${CAT}"
  "${CHMOD}"
  "${FIND}"
  "${MKDIR}"
  "${MV}"
  "${RM}"
  "${TAR}"
  "${UNZIP}"
)

for tool in "${TOOLS[@]}"; do
  if [ ! -x "${tool}" ]; then
    abort "Missing ${tool}"
  fi
done

PROJECT_PATH="$(dirname "$0")"
# shellcheck disable=SC2181
if [ $? -ne 0 ] || [ ! -d "${PROJECT_PATH}" ]; then
  abort "Failed to determine working directory"
fi

cd "${PROJECT_PATH}"

rm -rf Uncrustify-repo uncrustify || abort "Failed to cleanup legacy Uncrustify directory"
# TODO: switch to master
src=$(curl -Lfs https://raw.githubusercontent.com/acidanthera/ocbuild/unc-build/uncrustify/uncstrap.sh) && eval "$src" || exit 1

FILE_LIST="filelist.txt"
"${RM}" -f "${FILE_LIST}" || abort "Failed to cleanup legacy ${FILE_LIST}"
"${FIND}" \
  ../.. \
  \( \
    -path "../../UDK" -o \
    -path "../../Library/OcAppleImg4Lib/libDER" -o \
    -path "../../Library/OcAppleImg4Lib/libDERImg4" -o \
    -path "../../Library/OcCompressionLib/lzss" -o \
    -path "../../Library/OcCompressionLib/lzvn" -o \
    -path "../../Library/OcCompressionLib/zlib" -o \
    -path "../../Library/OcMp3Lib/helix" -o \
    -path "../../Staging/OpenHfsPlus" -o \
    -path "../../Utilities/acdtinfo" -o \
    -path "../../Utilities/BaseTools" -o \
    -path "../../Utilities/disklabel" -o \
    -path "../../Utilities/EfiResTool" -o \
    -path "../../Utilities/icnspack" -o \
    -path "../../Utilities/LogoutHook" -o \
    -path "../../Utilities/macserial" -o \
    -path "../../Utilities/RsaTool" -o \
    -path "../../Utilities/WinNvram" -o \
    -name "AutoGenerated.c" -o \
    -name "LegacyBcopy.h" -o \
    -name "libDER_config.h" -o \
    -name "lodepng.c" -o \
    -name "lodepng.h" -o \
    -name "MsvcMath32.c" -o \
    -name "RelocationCallGate.h" -o \
    -name "Ubsan.c" -o \
    -name "Ubsan.h" -o \
    -name "UbsanPrintf.c" -o \
    -name "UmmMalloc.c" -o \
    -name "UserPseudoRandom.c" -o \
    -name "UserPseudoRandom.h" \
  \) \
  -prune -o \
  -type f \
  -name "*.[c\|h]" -print \
  > "${FILE_LIST}" || abort "Failed to dump source file list to ${FILE_LIST}"

UNCRUSTIFY_CONFIG_FILE=./uncrustify-OpenCorePkg.cfg
"${UNC_EXEC}" -c "${UNCRUSTIFY_CONFIG_FILE}" -F "${FILE_LIST}" --replace --no-backup --if-changed || abort "Failed to run Uncrustify"

git diff > uncrustify.diff || abort "Failed to generate uncrustify diff with code $?"
if [ "$(${CAT} uncrustify.diff)" != "" ]; then
  # show the diff
  "${CAT}" uncrustify.diff
  abort "Uncrustify detects codestyle problems! Please fix"
fi

exit 0
