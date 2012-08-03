#echo $#
#echo $0
#echo $1
#echo $2
make clean
make
make install
rm -r /am1808/nfs/var/www
rm /am1808/nfs/s2wifi

cp -r  bin/www /am1808/nfs/var/
cp -r  bin/s2wifi /am1808/nfs/s2wifi
#ln -s $1 uImage
#echo "link $1 to uImage ok"
