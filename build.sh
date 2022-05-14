#!/bin/sh
PKG_DIR="pkg_build"

asssemble_pkg() {
    SRC="$1"
    DEST="$2"
    NAME="$3"
    ID="$4"
    if [ -z $ID ] 
    then 
        echo "Improper parameters in pkg assembly phase; check the build script."
        exit 1 
    fi
  
    pkgbuild \
    --component "$SRC" \
    --identifier "$ID" \
    --install-location "$DEST" \
    $PKG_DIR"/"$NAME.pkg 

}

PLUGIN_DIR="$HOME/Desktop/test"

AU_NAME="rtstft_ctl.component"
AU_SRC="Builds/MacOSX/build/Release/"$AU_NAME
AU_DEST=$PLUGIN_DIR"/Components"
AU_ID="com.soundctl.rtstftctl.au"


VST3_NAME="rtstft_ctl.vst3"
VST3_SRC="Builds/MacOSX/build/Release/"$VST3_NAME
VST3_DEST=$PLUGIN_DIR"/VST3"
VST3_ID="com.soundctl.rtstftctl.vst3"

mkdir pkg_build
asssemble_pkg $VST3_SRC $VST3_DEST $VST3_NAME $VST3_ID
asssemble_pkg $AU_SRC $AU_DEST $AU_NAME $AU_ID
# rm -rf pkg_build
