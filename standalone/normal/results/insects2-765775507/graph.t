#!/usr/local/bin/blt_wish -f

blt_graph .g -bg white -plotbackground white

set x { 1    2  3  4 5 6 7 9 10 13 30 53 }
set y { 247 62 26 12 4 4 4 2  2  1  1  1 }

.g element create line1 -xdata $x -ydata $y -symbol circle -linewidth 1

.g configure -title "Most Genomes Are Rare"
.g configure -plotrelief flat
.g xaxis configure -min 0 -max 60  -stepsize 10 -subticks 0 -title "Genome Instance Cardinality"
.g yaxis configure -min 0 -max 250 -stepsize 50 -subticks 0 -title "Frequency Of This Cardinality"
.g legend configure -mapped 0

pack .g
wm min . 0 0

.g psconfigure -colormode gray
.g postscript graph.ps -pagewidth 6.5i -pageheight 9i -landscape true