gcc -O0 p2_apartado1.c -o I
gcc -O0 p2_apartado2.c -o II
rm ex.log 

for i in 250 500 700 1000 1500 2000 2550 3000
do
    echo -e "\nIteración con N = $i">>ex.log
    echo "Apartado I">>ex.log
    ./I $i>>ex.log
    echo "Apartado II">>ex.log
    ./II $i>>ex.log
done
