# Created By Radouane ELFITOURI
echo " Start ......"
ls | while read ligne
do
sed s/
//g $ligne > $ligne.tmp
mv $ligne.tmp $ligne
echo "."$ligne
done
echo "\n Fin ......\n"
