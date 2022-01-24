#!/bin/bash

# Get script directory
MYDIR="$(cd -P "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PROJECT="ckconv"


# If the release directory already exists, delete it
if [ -d "$MYDIR/out/Release" ]
then
	rm -rf "$MYDIR/out/Release"
fi
# Create a release directory to store output
mkdir -p "$MYDIR/out/Release"


LINUX_DIR="linux-release/bin"
WINDOWS_DIR="windows-release/bin"


# MAKE LINUX RELEASE

cd "$MYDIR/out/install/$LINUX_DIR"
VER="$(./$PROJECT -vq)" # Set VER variable to the current version
zip -T9 "$PROJECT-$VER-Linux.zip" "$PROJECT"
mv ./*.zip "$MYDIR/out/Release"


# MAKE WINDOWS RELEASE

cd "$MYDIR/out/install/$WINDOWS_DIR"
zip -T9 "$PROJECT-$VER-Windows.zip" "$PROJECT.exe"
mv ./*.zip "$MYDIR/out/Release"

# DONE

echo "Created Release Archives for $PROJECT Version $VER"
echo "See output directory: $MYDIR/out/Release"
