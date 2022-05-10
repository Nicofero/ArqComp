N=c(250, 500, 700, 1000, 1500, 2000, 2550, 3000)

C1=c(31172228.000000,118299392.000000,240294942.000000,484388025.000000,1003624595.000000,1688310277.000000,2697123006.000000,3713986739.000000)
C1_O=c(5889747.000000,25413272.000000,47468662.000000,77891888.000000,203133834.000000,368728919.000000,571469339.000000,775618293.000000)
C1_O3=c(4128270.000000,12283912.000000,15612554.000000,36167115.000000,82155805.000000,151350577.000000,243445774.000000,331622681.000000)
C2=c(4293724.000000,14601306.000000,28268168.000000,58481834.000000,129356951.000000,243598268.000000,464674266.000000,514602471.000000)
C3=c(2701110.000000,11349595.000000,23774008.000000,48812051.000000,110752839.000000,200305659.000000,318527699.000000,447503842.000000)
C4=c(974797.000000,4012251.000000,7854092.000000,16773678.000000,36504223.000000,65361007.000000,113934983.000000,149383220.000000)
C4_1=c(4064827.000000,14956909.000000,29022239.000000,56104418.000000,124880216.000000,221400969.000000,358926324.000000,507532035.000000)
C4_2=c(1755838.000000,6958286.000000,14710580.000000,30572062.000000,64290928.000000,118222074.000000,188777633.000000,262200937.000000)
C4_4=c(1193581.000000,4095185.000000,8037239.000000,16401439.000000,36963453.000000,69240419.000000,108485916.000000,146709507.000000)
C4_O2=c(251539.000000,890530.000000,1730526.000000,3527868.000000,8094732.000000,14440298.000000,23340629.000000,32555567.000000)
plot(N,C1,col=2,type="l",main="Comparacion entre todos los ejercicios",lwd=2, ylab = "Ciclos de reloj")
lines(N,C1_O,col=3,lwd=2)
lines(N,C2,col=4,lwd=2)
lines(N,C3,col=5,lwd=2)
lines(N,C4,col=6,lwd=2)
lines(N,C4_O2,col=7,lwd=2)
legend("topright",legend = c("Ej1","Ej1 (-O2)","Ej2","Ej3","Ej4","Ej4 (-O2)"), col = 2:7, lwd = 2, bty = "n")
a=C1/C2
med=mean(a)

#Grafica speedup Ej1 y Ej2
plot(N,a,col=2,type="l",main="Speedup entre Ej1 y Ej2",ylab="Speedup")
abline(med,0,lty=2)
legend("topright",legend=c("Speedup", "Media"),col=2:1, lty = 1:2,lwd=2,bty="n")

#Grafica speedup Ej3 y Ej4 con respecto a Ej2
a=C2/C3
b=C2/C4
plot(N,a,col=3,type="l",lwd=2,ylim=c(1,5),main="Speedup entre sec. opt. y AVX y OpenMP",ylab="Speedup")
lines(N,b,col=4,lwd=2)
legend("topright",legend=c("Ej3","Ej4"),col=3:4,lwd=2,bty="n")

#Grafica speedup 2,3,4 con respecto a 1 con -O3
a=C1_O3/C2
b=C1_O3/C3
c=C1_O3/C4
plot(N,a,col=5,type="l",lwd=2,main="Speedup entre -O3 y el resto de versiones",ylab="Speedup", ylim = c(0,5))
lines(N,b,col=6,lwd=2)
lines(N,c,col=7,lwd=2)
legend("topright",legend=c("Ej2","Ej3","Ej4"),col=5:7,lwd=2,bty="n")

#Grafica cores
plot(N,C2/C4_4,main="Comparacion OpenMP con n hilos distintos",col=4,lwd=2, type = "l", ylab = "Speedup con respecto a sec. opt.",ylim=c(1,4.5))
lines(N,C2/C4_2,col=3,lwd=2)
lines(N,C2/C4_1,col=2,lwd=2)
lines(N,C2/C4,col=5,lwd=2)
legend("topright",legend=c(1,2,4,8),col=2:5,lwd=2,bty="n")

#Grafica N mas grande

plot(c(1,2,4,8),c(C2[8]/C4_1[8],C2[8]/C4_2[8],C2[8]/C4_4[8],C2[8]/C4[8]),type="l",pch=19,xlab="Numero de hilos",ylab="Speedup con respecto a sec. opt.",lwd=2,main="Comparacion de n cores con N=3000")
points(c(1,2,4,8),c(C2[8]/C4_1[8],C2[8]/C4_2[8],C2[8]/C4_4[8],C2[8]/C4[8]),col=2:5,lwd=5)
