gcc -o II -O0 p2_apartado2.c
    gcc -O0 p2_apartado2_block.c -o IIb

echo -n "NC=c(">>pr.r
for i in {0..100}
do
    ./II 250 pr.r>>log1.log
done
echo ")">>pr.r

echo -n "NC=c(">>pr.r
for i in {0..100}
do
    ./IIb 250 pr.r>>log1.log
done
echo ")">>pr.r