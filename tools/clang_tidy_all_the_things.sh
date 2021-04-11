#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace
set -o pipefail

declare -a bazel_build_flags

bazel_build_flags+=(
  # Useful to determine how long individual steps are taking in CI.
  '--show_timestamps'
)

bazel_build_flags+=(
    '--noshow_progress'
    '--noshow_loading_progress'
)

# Compilation database should be stored in the Bazel execution root.
readonly BAZEL_ROOT=$(bazel info execution_root)

# Generate compilation database.
CC=clang bazel build "${bazel_build_flags[@]}" -- //...

#./tools/generate_compilation_database.sh

# Run clang-tidy.
# tests have a lot of issues with importing libraries
mapfile -t SOURCE_FILES < <(find "." -name '*.cc' -not -name '*_test.cc')
clang-tidy -checks='google-*' -p "$BAZEL_ROOT" -header-filter='./.*' "${SOURCE_FILES[@]}"
