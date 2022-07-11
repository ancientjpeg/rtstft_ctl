#!/bin/sh

VERSION="0.1.0"
PLUGIN_DIR="$HOME/Desktop/test"
PLUGIN_DIR="/Library/Audio/Plug-Ins"
SUPPORT_DIR="/Library/Application Support/sound_ctl"
PLUGIN_SUPPORT_DIR="$SUPPORT_DIR/rtstft_ctl"
PLUGIN_PRESETS_DIR="$PLUGIN_SUPPORT_DIR/presets"

bold=$(tput bold)
norm=$(tput sgr0)

function print_help() {
cat << PRINT_HELP

${bold}NAME:${norm}

    build.sh:

${bold}USAGE:${norm}

    ./build.sh [-p [DIR]] [-f]
    
${bold}OPTIONS:${norm}
    
    -p [DIR]: location of presets dir to copy into the repository before assembling the installation package\n
    -f:       copy the default presets factory directory (${PLUGIN_PRESETS_DIR}) into the repository before packaging

PRINT_HELP
}

function error_out() {
    print_help
    exit 1
}

while getopts 'p:fh' OPT; do 
    case "$OPT" in
    p) 
        PRESETS_FACTORY_DIR=$OPTARG
        if [ -d "$OPTARG" ]; then 
            rm -rf Resources/Factory
            cp -r "$OPTARG" Resources/Factory
        else
            error_out
        fi
    ;;
    f)
        rm -rf Resources/Factory
        cp -r "/Library/Application Support/sound_ctl/rtstft_ctl/Factory" Resources/Factory
    ;;
    h)
    print_help
    exit 0
    ;;
    ?) 
        error_out
    ;;
    esac
done

echo $"=======  RUNNING CMAKE BUILD  =======\n"
cmake -Bbuild -G "Ninja Multi-Config"
cmake --build build --config Release --target rtstft_ctl_AU
cmake --build build --config Release --target rtstft_ctl_VST3
echo $"======= CREATING BUNDLE FILES =======\n"

mkdir pkg_build
cd pkg_build
mkdir resources

cat > resources/readme.txt << README
Welcome to the rtstft_ctl installer! By default, this \
package will install both the AU and VST versions of rtstft_ctl, \
but feel free to customize the installation and exclude one of these \
in order to save some space on your machine.
README
cp ../LICENSE resources/license.txt

# https://github.com/kurasu/surge/blob/main/installer_mac/make_installer.sh

echo $"===== CREATING DISTRIBUTION XML =====\n"
BUILD_SRC="../build/rtstft_ctl_artefacts/Release"

AU_NAME="rtstft_ctl.component"
AU_SRC=$BUILD_SRC"/AU/"$AU_NAME
AU_DEST=$PLUGIN_DIR"/Components"
AU_ID="com.soundctl.rtstftctl.au"


VST3_NAME="rtstft_ctl.vst3"
VST3_SRC=$BUILD_SRC"/VST3/"$VST3_NAME
VST3_DEST=$PLUGIN_DIR"/VST3"
VST3_ID="com.soundctl.rtstftctl.vst3"

PRESETS_NAME="Presets"
PRESETS_ID="com.soundctl.rtstftctl.presets"

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
        <line choice="${VST3_NAME}"/>
        <line choice="${PRESETS_NAME}"/>
    </choices-outline>
    <choice id="${AU_NAME}" title="Audio Unit" description="rtstft_ctl Audio Unit Installation">
        <pkg-ref id="${AU_ID}"/>
    </choice>
    <choice id="${VST3_NAME}" title="VST3" description="rtstft_ctl VST3 Installation">
        <pkg-ref id="${VST3_ID}"/>
    </choice>
    <choice id="${PRESETS_NAME}" title="Presets" description="Install factory preset library">
        <pkg-ref id="${PRESETS_ID}"/>
    </choice>
    <pkg-ref id="${AU_ID}" version="0.1.0" auth="Root">${AU_NAME}.pkg</pkg-ref>
	<pkg-ref id="${VST3_ID}" version="0.1.0" auth="Root">${VST3_NAME}.pkg</pkg-ref>
    <pkg-ref id="${PRESETS_ID}" version="0.1.0" auth="Root">${PRESETS_NAME}.pkg</pkg-ref>
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
        echo $"Missing parameters in pkg assembly phase\n Check the build script."
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

pkgbuild \
--root ../Resources/Factory \
--version $VERSION \
--identifier "$PRESETS_ID" \
--install-location "$PLUGIN_PRESETS_DIR/Factory" \
--sign "$PKG_SIG" \
$PRESETS_NAME.pkg


echo $"======== FINAL PACKAGE BUILD ========\n"
PKG_NAME_FINAL="OSX_rtstft_ctl_"$VERSION".pkg"
productbuild --sign "$PKG_SIG" \
--distribution distribution.xml  \
--resources resources \
--package-path ./ $PKG_NAME_FINAL # > /dev/null 2>&1

echo $"\n============== CLEANUP ==============\n"
mv $PKG_NAME_FINAL ..
cd ..
rm -rf pkg_build