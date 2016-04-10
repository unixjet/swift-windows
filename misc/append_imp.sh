#!/bin/sh

sed -e 's;external global;external dllimport global;' \
	-e 's;@_TWVBo = external dllimport;@_TWVBo = external;' \
	-e 's;@_TWVBo;@__imp__TWVBo;' $1 > $2

