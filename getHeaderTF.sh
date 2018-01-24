
#!/bin/bash -eu
# -*- coding: utf-8 -*-

HEADER_DIR=$HOME/OpenFOAM/tensorflow/lnInclude

if [ ! -e $HEADER_DIR ];
then
    mkdir -p $HEADER_DIR
fi

cd tensorflow

find tensorflow/core -follow -type f -name "*.h" -exec cp --parents {} $HEADER_DIR \;
find tensorflow/cc   -follow -type f -name "*.h" -exec cp --parents {} $HEADER_DIR \;
find tensorflow/c    -follow -type f -name "*.h" -exec cp --parents {} $HEADER_DIR \;

find bazel-genfiles    -follow -type f -name "*.h" -exec cp --parents {} $HEADER_DIR \;
find bazel-tensorflow    -follow -type f -name "*.h" -exec cp --parents {} $HEADER_DIR \;

find third_party/eigen3 -follow -type f -exec cp --parents {} $HEADER_DIR \;

pushd bazel-genfiles
find tensorflow -follow -type f -name "*.h" -exec cp --parents {} $HEADER_DIR \;
popd

pushd bazel-tensorflow/external/protobuf_archive/src
find google -follow -type f -name "*.h" -exec cp --parents {} $HEADER_DIR \;
popd

pushd bazel-tensorflow/external/eigen_archive
find Eigen       -follow -type f -exec cp --parents {} $HEADER_DIR \;
find unsupported -follow -type f -exec cp --parents {} $HEADER_DIR \;
popd

pushd ~/.cache/bazel/_bazel_inabower/30f5cb197d6560046b7ad065e7b2b1e3/external
find eigen_archive -follow -type f -name "*.h" -exec cp --parents {} $HEADER_DIR \;
find protobuf_archive -follow -type f -name "*.h" -exec cp --parents {} $HEADER_DIR \;
popd
