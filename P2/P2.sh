gcc -O0 p2_apartado1.c -o I
gcc -O0 p2_apartado2.c -o II
rm ex.r 
rm *.log
echo  -e "N=c(250, 500, 700, 1000, 1500, 2000, 2550, 3000)\n">>ex.r
echo -n "C1=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./I $i ex.r>>log1.log
done

gcc -O0 p2_apartado1.c -o I
echo -n "C1_O=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./I $i ex.r>>log1.log
done

echo -n "C2=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./II $i ex.r>>log2.log
done

echo -e "plot(N,C1,col=2,type=\"l\",main=\"Comparacion entre ejercicio 1 y 2\",lwd=2)\nlines(N,C1_O,col=3,lwd=2)\nlines(N,C2,col=4,lwd=2)\nlegend(\"topright\",legend = c(\"Ej1\",\"Ej1 (Autovectorizacion)\",\"Ej2\"), col = 2:4, lwd = 2, bty = \"n\")">>ex.r