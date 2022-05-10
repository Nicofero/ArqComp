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

gcc -O3 p2_apartado1.c -o I
echo -n "C1_O3=c(">>ex.r
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
    ./IV $i ex.r>>log4.log 8
done

echo -n "C4_1=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./IV $i ex.r>>log4.log 1
done

echo -n "C4_2=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./IV $i ex.r>>log4.log 2
done

echo -n "C4_4=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./IV $i ex.r>>log4.log 4
done

gcc -o IV -fopenmp -O2 p2_apartado4.c

echo -n "C4_O2=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./IV $i ex.r>>log4.log 8
done

echo -e "plot(N,C1,col=2,type=\"l\",main=\"Comparacion entre todos los ejercicios\",lwd=2)
lines(N,C1_O,col=3,lwd=2)
lines(N,C2,col=4,lwd=2)
lines(N,C3,col=5,lwd=2)
lines(N,C4,col=6,lwd=2)
lines(N,C4_O2,col=7,lwd=2)
legend(\"topright\",legend = c(\"Ej1\",\"Ej1 (-O2)\",\"Ej2\",\"Ej3\",\"Ej4\",\"Ej4 (-O2)\"), col = 2:7, lwd = 2, bty = \"n\")
a=C1/C2
med=mean(a)

#Grafica speedup Ej1 y Ej2
plot(N,a,col=2,type=\"l\",main=\"Speedup entre Ej1 y Ej2\",ylab=\"Speedup\")
abline(med,0,lty=2)
legend(\"topright\",legend=\"Speedup\",col=2,lwd=2,bty=\"n\")

#Grafica speedup Ej3 y Ej4 con respecto a Ej2
a=C2/C3
b=C2/C4
plot(N,a,col=3,type=\"l\",lwd=2,ylim=c(1,5),main=\"Speedup entre sec. opt. y AVX y OpenMP\",ylab=\"Speedup\")
lines(N,b,col=4,lwd=2)
legend(\"topright\",legend=c(\"Ej3\",\"Ej4\"),col=3:4,lwd=2,bty=\"n\")

#Grafica speedup 2,3,4 con respecto a 1 con -O3
a=C1_O3/C2
b=C1_O3/C3
c=C1_O3/C4
plot(N,a,col=5,type=\"l\",lwd=2,main=\"Speedup entre -O3 y el resto de versiones\",ylab=\"Speedup\", ylim = c(0,3))
lines(N,b,col=6)
lines(N,c,col=7)
legend(\"topright\",legend=c(\"Ej2\",\"Ej3\",\"Ej4\"),col=5:7,lwd=2,bty=\"n\")

#Grafica cores
plot(N,C4_1,main=\"Comparacion OpenMP con n_hilos distintos\",col=2,lwd=2)
lines(N,C4_2,col=3)
lines(N,C4_4,col=4)
lines(N,C4,col=5)
legend(\"topright\",legend=c(1,2,4,8),col=2:5,lwd=2,bty=\"n\")">>ex.r