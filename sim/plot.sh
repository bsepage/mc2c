#!/bin/bash
#gnuplot -e "set terminal dumb; plot 'sim/measures.dat' using 1:2" -persist
gnuplot -e "plot 'sim/measures.dat' using 1:2" -persist
