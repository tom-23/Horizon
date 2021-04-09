export QTDIR="C:\Qt\5.15\msvc2019_64"

"$QTDIR/bin/windeployqt" --help
"$QTDIR/bin/windeployqt" \
    --no-system-d3d-compiler \
    --no-quick \
    "$Executable"

choco install -y InnoSetup
cmd " /c C:/ProgramData/chocolatey/bin/ISCC.exe /O$APPVEYOR_BUILD_FOLDER /DAppVersion=$APP_VERSION /DRoot=$Destination /DSource=$Source $Source/Shared/horizon.iss"
