if [ $(pwd | grep "test") ]
then
    echo "Run from main folder using: ./tests/runcpplint.sh"
    exit 1
fi

cpplint --filter=-whitespace,-legal/copyright,-readability/check,-readability/casting  --recursive . > tests/cpplint.log
