gcc -o II -O0 p2_apartado2.c
    gcc -O0 p2_apartado2_block.c -o IIb

echo -n "C2_b=c(">>ex.r
for i in 250 500 700 1000 1500 2000 2550 3000
do
    ./IIb $i ex.r>>log2.log
done