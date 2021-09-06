#!/bin/bash
# START SPEED TEST
#res1=$(date +%s.%N)
# Lint
# clang-format -i main.c

# Cleanup
rm -f ./main
rm -rf ../site
mkdir ../site

# Linux(debug)
# cc -std=c89 -DDEBUG -Wall -Wno-unknown-pragmas -Wpedantic -Wshadow -Wuninitialized -Wextra -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Wvla -g -Og -fsanitize=address -fsanitize=undefined main.c -o main

# Linux(fast)
 cc main.c -std=c89 -Os -DNDEBUG -g0 -Wall -Wno-unknown-pragmas -o main

# RPi
# tcc -Wall main.c -o main

# Plan9
# pcc main.c -o main

# Valgrind
# gcc -std=c89 -DDEBUG -Wall -Wpedantic -Wshadow -Wuninitialized -Wextra -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Wvla -g -Og main.c -o main
# valgrind ./main

# Build Size
# echo "$(du -b ./main | cut -f1) bytes written"

# Run
./main

# Cleanup
rm -f ./main
# FINISH SPEED TEST
#res2=$(date +%s.%N)
#dt=$(echo "$res2 - $res1" | bc)
#dd=$(echo "$dt/86400" | bc)
#dt2=$(echo "$dt-86400*$dd" | bc)
#dh=$(echo "$dt2/3600" | bc)
#dt3=$(echo "$dt2-3600*$dh" | bc)
#dm=$(echo "$dt3/60" | bc)
#ds=$(echo "$dt3-60*$dm" | bc)

#LC_NUMERIC=C printf "Total runtime: %d:%02d:%02d:%02.4f\n" $dd $dh $dm $ds
