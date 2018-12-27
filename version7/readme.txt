requirements£º
Linux system
Install libdmtx
Install OpenCV

compile command :


g++ `pkg-config --cflags opencv libdmtx` -o test main.o recog.o filterproc.o guidedfilter.o ZbarBinary.o ZXingBW.o YangBW.o `pkg-config --libs opencv libdmtx`
