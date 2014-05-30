cmd_Release/obj.target/serial/serial.o := cc '-D_DARWIN_USE_64_BIT_INODE=1' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-DBUILDING_NODE_EXTENSION' -I/Users/macbook/.node-gyp/0.10.18/src -I/Users/macbook/.node-gyp/0.10.18/deps/uv/include -I/Users/macbook/.node-gyp/0.10.18/deps/v8/include  -Os -gdwarf-2 -mmacosx-version-min=10.5 -arch x86_64 -Wall -Wendif-labels -W -Wno-unused-parameter -fno-strict-aliasing -MMD -MF ./Release/.deps/Release/obj.target/serial/serial.o.d.raw  -c -o Release/obj.target/serial/serial.o ../serial.c
Release/obj.target/serial/serial.o: ../serial.c ../serial.h
../serial.c:
../serial.h:
