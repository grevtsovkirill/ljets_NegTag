#!/bin/bash

MYCOMMAND="$@"
echo $MYCOMMAND
for arg; do shift; done

source ../setup.sh

pwd
which root
echo "$MYCOMMAND"
$MYCOMMAND
