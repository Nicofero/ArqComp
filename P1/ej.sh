rm Ejecuciones/*
echo -e "L=c(256,768,2048,3072,8192,16384,32768)\n">Ejecuciones/cache.r

#EJECUCION PROGRAMA PARA DISTINTOS Ds ALMACENANDO EL RESULTADO EN SU ARCHIVO .log CORRESPONDIENTE
echo "Ejecución con D = " $1
./program $1 Ejecuciones/cache.r>Ejecuciones/ej_$1.log

echo "Ejecución con D = " $2
./program $2 Ejecuciones/cache.r>Ejecuciones/ej_$2.log

echo "Ejecución con D = " $3
./program $3 Ejecuciones/cache.r>Ejecuciones/ej_$3.log

echo "Ejecución con D = " $4
./program $4 Ejecuciones/cache.r>Ejecuciones/ej_$4.log

echo "Ejecución con D = " $5
./program $5 Ejecuciones/cache.r>Ejecuciones/ej_$5.log

#IMPRESIÓN .R

echo -e "\nplot(L,C_$1, col = 1, lwd= 3, ylim = c(0,70), xlim = c(0,32768),type = \"l\",main = \"Grafico\", ylab = \"Coste en ciclos de cada acceso a memoria\" , xlab = \"Numero total de lineas cache diferentes del vector A que se referencian\")
lines(L,C_$2, col = 2, lwd= 3,type = \"l\")
lines(L,C_$3, col = 3, lwd= 3,type = \"l\")
lines(L,C_$4, col = 4, lwd= 3,type = \"l\")
lines(L,C_$5, col = 5, lwd= 3,type = \"l\")
legend(\"topright\",legend = c($1,$2,$3,$4,$5), col = 1:5, lwd = 2, bty = \"n\")
abline(v=L, col=1, lty=4)">>Ejecuciones/cache.r
