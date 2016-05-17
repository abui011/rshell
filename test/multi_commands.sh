echo Testing multiple commands


echo A; echo B; echo C;
mkdir newfolder && echo B
mkdir newfolder && echo This shouldn't echo
mkdir newfolder || echo This should
echo A && echo B && echo C
echo A && echo B || echo This shouldn't echo
echo A || echo B && echo C
echo A; echo B && echo C
echo A; echo B || echo C
echo A; echo B; echo && #this shouldn't work
ls; ls; ls;
ls || ls || ls
echo A        #;echo B
echo ls ;ls
echo A            && echo B
echo A   &&    echo B

echo All multiple commands tested
echo Testing completed
