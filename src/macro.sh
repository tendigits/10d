#!/bin/bash
# Setup: make executable with: chmod +x ./macro.sh
# Usage: customize the template and generate files with: 
# ./macro.sh term1 term2 term3

INC='./inc/'
TPL='<h2>Title Goes Here</h2>'

for var in "$@"
do
	echo -e $TPL >> $INC$var'.htm'
done