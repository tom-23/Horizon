source scripts/macOS/env.sh

if test -d build/Horizon.app; then
    export PATH=${CMAKE_PREFIX_PATH}/bin:${PATH}
    cd build
    macdeployqt Horizon.app -always-overwrite -verbose=1
    python ../lib/macdeployqtfix/macdeployqtfix.py --quiet Horizon.app/Contents/MacOS/Horizon ${CMAKE_PREFIX_PATH}
    pkgbuild --root "Horizon.app" \
         --install-location "/Applications/Horizon.app" \
         --identifier "com.tombutcher.horizon.pkg" \
         --version ${APP_VERSION} \
         ${APP}.pkg
else
    true
fi

