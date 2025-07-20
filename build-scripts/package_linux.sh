cd build/
make DESTDIR=./linux_package install
rm -rf ./linux_package/opt/replayer/include
rm -rf ./linux_package/opt/replayer/lib
cp -r ../external/obs-studio/build-linux/rundir/RelWithDebInfo/lib ./linux_package/opt/replayer/lib
cp -r ../external/obs-studio/build-linux/rundir/RelWithDebInfo/share ./linux_package/opt/replayer/share
cp ../external/obs-studio/build-linux/rundir/RelWithDebInfo/bin/* ./linux_package/opt/replayer/bin
ln -s libobs-opengl.so.30 ./linux_package/opt/replayer/lib/libobs-opengl.so
