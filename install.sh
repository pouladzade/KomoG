  #!/bin/bash
  sudo add-apt-repository ppa:beineri/opt-qt58-trusty -y
  sudo apt-get update -qq
    

  sudo apt-get -y install qt58base
  source /opt/qt58/bin/qt58-env.sh

  qmake PREFIX=/usr
  make -j4
  sudo apt-get -y install checkinstall
  sudo checkinstall --pkgname=app --pkgversion="1" --pkgrelease="1" --backup=no --fstrans=no --default --deldoc 
  mkdir appdir ; cd appdir
  dpkg -x ../app_1-1_amd64.deb . ; find .
  cp ./usr/share/applications/*.desktop .
  cp ./usr/share/icons/hicolor/48x48/apps/*.png .
  cd .. 
  wget -c "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage" 
  chmod a+x linuxdeployqt-continuous-x86_64.AppImage
  unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
  ./linuxdeployqt-continuous-x86_64.AppImage ./appdir/usr/bin/* -bundle-non-qt-libs -qmldir=$(readlink -f /opt/qt5*/qml/)
  ./linuxdeployqt-continuous-x86_64.AppImage ./appdir/usr/bin/* -appimage -qmldir=$(readlink -f /opt/qt5*/qml/)
 find ./appdir -executable -type f -exec ldd {} \; | grep " => /usr" | cut -d " " -f 2-3 | sort | uniq