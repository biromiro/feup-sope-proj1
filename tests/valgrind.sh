if [ $(pwd | grep "test") ]
then
    echo "Run from main folder using: ./tests/valgrind.sh"
    exit 1
fi

mkdir test_files
mkdir test_files/folder1
mkdir test_files/folder2
mkdir test_files/folder1/folder3
echo "Testing file 1" >> test_files/f1
echo "Testing file 2" >> test_files/f2
echo "Testing file 3" >> test_files/folder1/f3
echo "Testing file 4" >> test_files/folder1/f4
echo "Testing file 5" >> test_files/folder1/f5
echo "Testing file 6" >> test_files/folder1/folder3/f6
echo "Testing file 7" >> test_files/folder1/folder3/f7
echo "Testing file 8" >> test_files/folder1/folder3/f8
echo "Testing file 9" >> test_files/folder2/f9
echo "Testing file 10" >> test_files/folder2/f10
echo "Testing file 11" >> test_files/folder2/f11

chmod -R +x test_files

echo > tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R 0888 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -R 0888 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R 0777 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -R 0777 >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R 0777 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -R 0777 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R 8888 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -R 8888 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R 0123 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -R 0888 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R z=x test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -R z=x test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R z=x test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -p g=x test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R 0123 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -R 0123 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R 0456 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -R 0456 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -R 0666 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -R 0666 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cR 0123 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cR 0123 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cR 0456 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cR 0456 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cR 0666 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cR 0666 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cvR 0123 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cvR 0123 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cvR 0456 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cvR 0456 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cvR 0666 test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cvR 0666 test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cvR a=xwr test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cvR a=xwr test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cvR g=xw test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cvR g=xw test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cvR u=rw test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cvR u=rw test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cvR o=x test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cvR o=x test_files  >> tests/valgrind.log

echo "========================================"  >> tests/valgrind.log
echo "========================================"  >> tests/valgrind.log

echo "TEST: valgrind -q --gen-suppressions=yes ./xmod -cvR o=xxwwxxrrww,g-rrwwrrxxxxx,u+x test_files" >> tests/valgrind.log
valgrind -q --gen-suppressions=yes ./xmod -cvR o=xxwwxxrrww,g-rrwwrrxxxxx,u+x test_files  >> tests/valgrind.log

chmod -R 0777 test_files

rm -rf test_files
