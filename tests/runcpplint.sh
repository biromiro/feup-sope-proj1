if [ $(pwd | grep "test") ]
then
    echo "Run from main folder using: ./test/runcpplint.sh"
    exit 1
fi

cpplint --filter=-whitespace,-legal/copyright,-readability/check  --recursive src > test/cpplint.log
