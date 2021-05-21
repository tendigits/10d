#!/bin/bash
# Setup: make executable with: chmod +x ./macro.sh
# Usage: customize the template and generate files with: 
# ./macro.sh term1 term2 term3

INC='./inc/'
TPL1='<header>\r\t<p class="pretitle"></p>\r\t<h1>'
TPL2='</h1>\r\t<p class="posttitle"></p>\r</header>\r<main>'
TPL3='<figure>\r\t<a href="#imghigh" class="h">•</a><a href="#imglow" class="l">•</a>\r\t<img class="high" src="../assets/img/" width="" height="" alt="" loading="lazy">\r\t<img class="low" src="../assets/img/" width="" height="" alt="" loading="lazy">\r\t<figcaption></figcaption>\r</figure>'
TPL4='\r</main>'

for var in "$@"
do
	echo -e $TPL1$var$TPL2$TPL3$TPL4 >> $INC$var'.htm'
done