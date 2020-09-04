cp /home/zyc/Github/StickyNotes-build/build-StickyNotes-unknown-Release/StickyNotes /home/zyc/Github/DTKstickyNotes/build/opt/apps/DTKstickyNotes/bin/
VERSION=0.0.3-beta
dpkg -b build/ DTKstickyNotes-$VERSION-amd64.deb
