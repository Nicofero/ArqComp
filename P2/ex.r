N=c(250, 500, 700, 1000, 1500, 2000, 2550, 3000)

C1=c(31206950.000000,123484818.000000,235016811.000000,501521044.000000,1100262444.000000,1716609811.000000,2686465750.000000,3694390509.000000)
C1_O=c(5793226.000000,22676235.000000,41771862.000000,77804589.000000,202119709.000000,376011765.000000,560247185.000000,784335393.000000)
C1_O3=c(2028156.000000,7851498.000000,16013626.000000,32596788.000000,79150254.000000,147416981.000000,253068377.000000,341363968.000000)
C2=c(4325364.000000,14703179.000000,28417819.000000,67598846.000000,136150079.000000,247695580.000000,381841199.000000,522832996.000000)
C3=c(2565135.000000,12022544.000000,23313458.000000,46046908.000000,109739179.000000,200336778.000000,305565077.000000,424147177.000000)
C4=c(1000985.000000,4022614.000000,7865196.000000,17128816.000000,36163451.000000,65217641.000000,107168473.000000,162297385.000000)
C4_1=c(4283574.000000,17169277.000000,29427548.000000,57594750.000000,127959097.000000,228538506.000000,369541859.000000,517898941.000000)
C4_2=c(1731963.000000,7017500.000000,14054257.000000,28717824.000000,65467062.000000,116671080.000000,192833579.000000,275360893.000000)
C4_4=c(1191664.000000,4097528.000000,8002498.000000,16359737.000000,37084183.000000,66964151.000000,107207693.000000,147807212.000000)
C4_O2=c(249813.000000,899498.000000,1746845.000000,3660853.000000,8183812.000000,14582207.000000,23452836.000000,32412893.000000)
plot(N,C1,col=2,type="l",main="Comparacion entre todos los ejercicios",lwd=2, ylab = "Ciclos de reloj")
lines(N,C1_O,col=3,lwd=2,type="l")
lines(N,C2,col=4,lwd=2)
lines(N,C3,col=5,lwd=2)
lines(N,C4_4,col=6,lwd=2)
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
b=C2/C4_4
plot(N,a,col=3,type="l",lwd=2,ylim=c(1,5),main="Speedup entre sec. opt. y AVX y OpenMP",ylab="Speedup")
lines(N,b,col=4,lwd=2)
legend("topright",legend=c("Ej3","Ej4"),col=3:4,lwd=2,bty="n")

#Grafica speedup 2,3,4 con respecto a 1 con -O3
a=C1_O3/C2
b=C1_O3/C3
c=C1_O3/C4_4
d=C1_O3/C4_O2
plot(N,a,col=5,type="l",lwd=2,main="Speedup entre -O3 y el resto de versiones",ylab="Speedup", ylim = c(0,3))
lines(N,b,col=6,lwd=2)
lines(N,c,col=7,lwd=2)
legend("topright",legend=c("Ej2","Ej3","Ej4"),col=5:7,lwd=2,bty="n")

plot(N, c, col = 5, type = "l", lwd = 2, main = "Speedup entre OpenMP compilado con y sin -O2", ylab = "Speedup", ylim = c(0,12))
lines(N,d, col = 6, lwd =2)
legend("topright", legend = c("Ej4", "Ej4 (O2)"), col = 5:6, lwd = 2, bty = "n")

#Grafica cores
plot(N,C2/C4_4,main="Comparacion OpenMP con n hilos distintos",col=4,lwd=2, type = "l", ylab = "Speedup con respecto a sec. opt.",ylim=c(1,4.5))
lines(N,C2/C4_2,col=3,lwd=2)
lines(N,C2/C4_1,col=2,lwd=2)
lines(N,C2/C4,col=5,lwd=2)
legend("topright",legend=c(1,2,4,8),col=2:5,lwd=2,bty="n")

#Grafica N mas grande

plot(c(1,2,4,8),c(C2[8]/C4_1[8],C2[8]/C4_2[8],C2[8]/C4_4[8],C2[8]/C4[8]),type="l",pch=19,xlab="Numero de hilos",ylab="Speedup con respecto a sec. opt.",lwd=2,main="Comparacion de n cores con N=3000")
points(c(1,2,4,8),c(C2[8]/C4_1[8],C2[8]/C4_2[8],C2[8]/C4_4[8],C2[8]/C4[8]),col=2:5,lwd=5)

