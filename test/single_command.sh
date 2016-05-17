#tests single commands

echo Testing single commands
ls
ls -a 
mkdir Alex
mkdir Alex #shouldn't work
echo Sentence
echo "Quotation marks shouldn't show up"
echo There "Shouldn't be any quotation marks here either"
    ls                ;
ls         ;
ls;;;;;; #should be an error
echo         ls ls ls l s #ls 
ls;
echo All single commands work
echo Testing Complete
exit
