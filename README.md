# opencv
installation steps::
Set platform target to x64 — Pre-built binaries are built for x64 Windows platforms.
Add to Include Directories — "opencv/build/include "
Add to Library Directories —" opencv/build/x64/vc15/lib "Tell the linker where it can find the lib files for different modules.
Add Additional Dependencies — "opencv_world420d.lib" List .lib files for different modules. Note that we’re only going to list a single all-in-one file named opencv_world.
