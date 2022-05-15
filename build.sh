#!/bin/sh

VERSION="0.1.0"
PLUGIN_DIR="$HOME/Desktop/test"
PLUGIN_DIR="/Library/Audio/Plug-Ins"

mkdir pkg_build
cd pkg_build

# https://github.com/kurasu/surge/blob/main/installer_mac/make_installer.sh




echo $"======== CREATING TXT FILES  ========\n"

echo "Welcome to the rtstft_ctl installer." > readme.txt
echo "license" > license.txt

echo $"===== CREATING DISTRIBUTION XML =====\n"

AU_NAME="rtstft_ctl.component"
AU_SRC="../Builds/MacOSX/build/Release/"$AU_NAME
AU_DEST=$PLUGIN_DIR"/Components"
AU_ID="com.soundctl.rtstftctl.au"


VST3_NAME="rtstft_ctl.vst3"
VST3_SRC="../Builds/MacOSX/build/Release/"$VST3_NAME
VST3_DEST=$PLUGIN_DIR"/VST3"
VST3_ID="com.soundctl.rtstftctl.vst3"

cat > distribution.xml << XMLEND
<?xml version="1.0" encoding="UTF-8"?>
<installer-gui-script>
    <options/>
    <title>rtstft_ctl</title>
    <background file="background" scaling="tofit" alignment="center"/>
    <welcome file="readme.txt"  mime-type="text/plain" />
    <license file="license.txt" mime-type="text/plain" />
    <choices-outline>
        <line choice="${AU_NAME}"/>
        <line choice="${VST_NAME}"/>
    </choices-outline>
    <choice id="${AU_NAME}" title="Audio Unit" description="rtstft_ctl Audio Unit Installation">
        <pkg-ref id="${AU_ID}"/>
    </choice>
    <choice id="${VST_NAME}" title="VST3" description="rtstft_ctl VST3 Installation">
        <pkg-ref id="${VST_ID}"/>
    </choice>
    <pkg-ref id="${AU_ID}" version="0.1.0" auth="Root">${AU_NAME}.pkg</pkg-ref>
	<pkg-ref id="${VST_ID}" version="0.1.0" auth="Root">${VST_NAME}.pkg</pkg-ref>
</installer-gui-script>
XMLEND

echo $"======= CREATING SUB-PACKAGES =======\n"



DIST_SIG="Apple Distribution: Jackson Kaplan"
PKG_SIG="3rd Party Mac Developer Installer: Jackson Kaplan"

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

    cp -r $SRC $NAME
    codesign -s "$DIST_SIG" $NAME
  
    pkgbuild \
    --component "$NAME" \
    --identifier "$ID" \
    --version $VERSION \
    --install-location "$DEST" \
    --sign "$PKG_SIG" \
    $NAME.pkg # > /dev/null 2>&1
    echo $"\n"
}

asssemble_pkg $AU_SRC $AU_DEST $AU_NAME $AU_ID
asssemble_pkg $VST3_SRC $VST3_DEST $VST3_NAME $VST3_ID

echo $"======== FINAL PACKAGE BUILD ========\n"
PKG_NAME_FINAL="OSX_rtstft_ctl_"$VERSION".pkg"
productbuild --sign "$PKG_SIG" --distribution distribution.xml --package-path ./ $PKG_NAME_FINAL # > /dev/null 2>&1

echo $"\n============== CLEANUP ==============\n"
mv $PKG_NAME_FINAL ..
cd ..
rm -rf pkg_build