gcc -O0 p2_apartado1.c -o I
gcc -O0 p2_apartado2.c -o II
gcc -o III -mavx2 p2_apartado3.c
gcc -o IV -fopenmp -O0 p2_apartado4.c
rm ex.r 
rm *.log
echo  -e "N=c(250, 500, 700, 1000, 1500, 2000, 2550, 3000)\n">>ex.r
echo -n "C1=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./I $i ex.r>>log1.log
done

gcc -O2 p2_apartado1.c -o I
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

echo -n "C3=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./III $i ex.r>>log3.log
done

echo -n "C4=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./IV $i ex.r>>log4.log 4
done

gcc -o IV -fopenmp -O2 p2_apartado4.c

echo -n "C4_2=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./IV $i ex.r>>log4.log 4
done

echo -e "plot(N,C1,col=2,type=\"l\",main=\"Comparacion entre todos los ejercicios\",lwd=2)
lines(N,C1_O,col=3,lwd=2)
lines(N,C2,col=4,lwd=2)
lines(N,C3,col=5,lwd=2)
lines(N,C4,col=6,lwd=2)
lines(N,C4_2,col=7,lwd=2)
legend(\"topright\",legend = c(\"Ej1\",\"Ej1 (Autovectorizacion)\",\"Ej2\",\"Ej3\",\"Ej4\",\"Ej4 (Autovectorizacion)\"), col = 2:7, lwd = 2, bty = \"n\")">>ex.r
