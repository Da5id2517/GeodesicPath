#!/bin/bash

WORKSPACE=$(bazel info workspace)
bazel run --run_under="cd $WORKSPACE && " @com_grail_bazel_compdb//:generate
