gcc -o III -mavx2 p2_apartado3.c
echo -n "NC=c(">>pr.r
for i in {0..100}
do
    ./III 250 pr.r>>log1.log
done
echo ")">>pr.r
