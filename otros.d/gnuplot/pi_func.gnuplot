#
# $Id: pi_func.gnuplot,v 1.1 2002/10/10 13:42:57 jjo Exp $
#
# Muestra la funcio'n  4/(1+x*x) 
# cuya integral entre [0,1] da' PI
#
#set noborder
#set clip
set samples 160
set zeroaxis
set trange [0:10]
plot 4/(1+x*x)
pause -1 "Hit return to continue"
