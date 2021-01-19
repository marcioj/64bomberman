#!/bin/bash
set -eu

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

VS_CODE_IMAGE_NAME=$(docker images --format "{{.Repository}}" | grep $(basename "$DIR"))

# build n64
docker run --volume "$DIR:/src" $VS_CODE_IMAGE_NAME bash -c "cd /src && make $@"
