#!/bin/sh

VERSION="0.1.0"
VERSION=$(echo $VERSION | tr '.' '_')

PLUGIN_DIR="$HOME/Desktop/test"
PLUGIN_DIR="/Library/Audio/Plug-Ins"
SUPPORT_DIR="/Library/Application Support/sound_ctl"
PLUGIN_SUPPORT_DIR="$SUPPORT_DIR/rtstft_ctl"
PLUGIN_PRESETS_DIR="$PLUGIN_SUPPORT_DIR/presets"
OUTPUT_DIR='..'

SIGNING_CERT="Developer ID Application: Jackson Kaplan (792TGC726K)"
INSTALLER_CERT="Developer ID Installer: Jackson Kaplan (792TGC726K)"

INSTALLER_NAME_FINAL="OSX_rtstft_ctl_"$VERSION

bold=$(tput bold)
norm=$(tput sgr0)

function print_help() {
    cat <<PRINT_HELP

${bold}NAME:${norm}

    build.sh:

${bold}USAGE:${norm}

    ./build.sh [-p [DIR]] [-f]
    
${bold}OPTIONS:${norm}
    
    -p [DIR]:   location of presets dir to copy into the repository before assembling the installation package
    -f:         copy the default presets factory directory (${PLUGIN_PRESETS_DIR}) into the repository before packaging
    -k:         keep the pkg_build dir to inspect for debugging purposes
    -n:         build a dmg and notarize
    -h:         display this dialog

PRINT_HELP
}

function error_out() {
    print_help
    exit 1
}

CLEANUP=TRUE
NOTARIZE=

while getopts 'p:fknh' OPT; do
    case "$OPT" in
    p)
        PRESETS_FACTORY_DIR=$OPTARG
        if [ -d "$OPTARG" ]; then
            rm -rf ./Resources/Factory
            cp -r "$OPTARG" ./Resources/Factory
            chmod -R 777 ./Resources/Factory
        else
            error_out
        fi
        ;;
    f)
        rm -rf ./Resources/Factory
        cp -r "/Library/Application Support/sound_ctl/rtstft_ctl/presets/Factory" ./Resources/Factory
        chmod -R 777 ./Resources/Factory
        ;;
    k)
        CLEANUP=
        ;;
    n)
        NOTARIZE=TRUE
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

cat >resources/readme.txt <<README
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

cat >distribution.xml <<XMLEND
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

echo $"========= WRITING SCRIPTS ==========\n"

mkdir scripts
cat >scripts/postinstall <<CHECK_DIR_END
#!/bin/sh

echo "running postinstall..." > /tmp/rtstft_install.log

mkdir -p  "/Library/Application Support/sound_ctl/rtstft_ctl/presets"
chmod -R 777 "/Library/Application Support/sound_ctl/rtstft_ctl/presets"

if [ "\$?" -ne '0' ]; then
    echo FAILED TO CHMOD PRESET DIRECTORIES!!!
    echo FAILED TO CHMOD PRESET DIRECTORIES!!!
    echo FAILED TO CHMOD PRESET DIRECTORIES!!!
    echo FAILED TO CHMOD PRESET DIRECTORIES!!!
    echo FAILED TO CHMOD PRESET DIRECTORIES!!!
    echo FAILED TO CHMOD PRESET DIRECTORIES!!! >> /tmp/rtstft_install.log
    exit 5
fi

echo "check_directories done." >> /tmp/rtstft_install.log

exit 0

CHECK_DIR_END

chmod 755 scripts/postinstall

echo $"======= CREATING SUB-PACKAGES =======\n"

asssemble_pkg() {
    SRC="$1"
    DEST="$2"
    NAME="$3"
    ID="$4"
    if [ -z $ID ]; then
        echo $"Missing parameters in pkg assembly phase\n Check the build script."
        exit 1
    fi

    cp -r $SRC $NAME
    codesign -fs "$SIGNING_CERT" $NAME

    pkgbuild \
        --component "$NAME" \
        --identifier "$ID" \
        --version $VERSION \
        --install-location "$DEST" \
        --sign "$INSTALLER_CERT" \
        --scripts scripts \
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
    --sign "$INSTALLER_CERT" \
    --scripts scripts \
    $PRESETS_NAME.pkg

echo $"\n======== FINAL PACKAGE BUILD ========\n"

PKG_NAME_FINAL="$INSTALLER_NAME_FINAL.pkg"

productbuild --sign "$INSTALLER_CERT" \
    --distribution distribution.xml \
    --package-path '.' \
    --scripts scripts \
    ./"$PKG_NAME_FINAL" # > /dev/null 2>&1

function notarize() {

    echo $"\n============= GENARATE MANUAL ==============\n"

    DOCS="../RTSTFT/docs"
    PAPER="RTSTFT_CTL_WHITEPAPER.pdf"
    make -C "$DOCS" >/dev/null
    cp "$DOCS/paper.pdf" "./$PAPER"
    make clean -C "$DOCS"

    TMPDIR="./dmg-tmp"
    mkdir -p "$TMPDIR"

    mv "./$PAPER" "./$TMPDIR"

    # https://github.com/surge-synthesizer/surge/blob/main/scripts/installer_mac/make_installer.sh
    echo $"\n============ CREATE DMG ============\n"

    DMG_NAME_FINAL="$INSTALLER_NAME_FINAL.dmg"

    mv "./$PKG_NAME_FINAL" "$TMPDIR"

    if [ -f "$OUTPUT_DIR/$DMG_NAME_FINAL" ]; then
        rm "$OUTPUT_DIR/$DMG_NAME_FINAL"
    fi

    hdiutil create /tmp/tmp.dmg -ov -volname "$INSTALLER_NAME_FINAL" -fs HFS+ -srcfolder "$TMPDIR"
    hdiutil convert /tmp/tmp.dmg -format UDZO -o "$OUTPUT_DIR/$DMG_NAME_FINAL"

    echo $"\n============= NOTARIZE ==============\n"

    if [ -n "$SIGNING_CERT" ]; then
        codesign -fs "$SIGNING_CERT" --timestamp "$OUTPUT_DIR/$DMG_NAME_FINAL"
        codesign -vvv "$OUTPUT_DIR/$DMG_NAME_FINAL"

        if [ "$(xcodebuild -version | egrep -o '[0-9]+' | head -n 1)" -ge 13 ]; then
            xcrun notarytool submit "$OUTPUT_DIR/$DMG_NAME_FINAL" --keychain-profile AC_PASSWORD --wait
        else
            echo "Just use XCode 13 god damnit"
            exit 5
        fi

        if [ "$?" -ne '0' ]; then
            echo "Notarization failed."
            exit 17
        fi

        xcrun stapler staple "$OUTPUT_DIR/$INSTALLER_NAME_FINAL.dmg"
    fi

    rm -rf "$TMPDIR"

}

if [ -n "$NOTARIZE" ]; then
    notarize
else
    mv $PKG_NAME_FINAL ..
fi

cd ..

if [ -n "$CLEANUP" ]; then

    echo $"\n============== CLEANUP ==============\n"
    rm -rf pkg_build

fi
