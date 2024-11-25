TEST_DIR="test/"

runnable_build() {
    ./build.sh --clean
    if [[ $# -eq 0 ]]; then
    	echo "No argument supplied"
    	exit 1
    fi

    if [[ ! -f $1 ]]; then
    	echo $1" does not exist"
    	exit 1
    fi

    if [[ ! ${1##*.} = c ]]; then
    	echo $1" is not a source file"
    	exit 1
    fi    

    ./build.sh --local
    cd test/
    rm -rf build/
    mkdir build/
    cd build
    cmake .. -DTEST_NAME=$(basename $1)
    make
    cd ..
}

if [[ $1 == "--help" ]]; then
    printf "
    
Flags:
    
    --clean:   removes build directories
    --local:   generates a local library and header files
    --system:  generates a system library and header files
    --test:    runs a test in the test folder
    --debug:   uses valgrind to test programs in the test folder

Examples:

    ./build.sh --clean                     (clean build directories)
    ./build.sh --local                     (installs local library)
    ./build.sh --system                    (installs system library)
    ./build.sh --test [test/TEST_NAME.c]   (tests given test name without valgrind)    
    ./build.sh --debug [test/TEST_NAME.c]  (tests given test name without valgrind)    

Notes:

    - Test programs rely on a local installation of ctk via build.sh --local
    - If a local copy of ctk was not generated, build.sh --test will auto generate

";

elif [[ $1 == "--clean" ]]; then
    rm -rf build
    rm -rf out
    rm -f test/vgcore.*
    rm -f vgcore.*
elif [[ $1 == "--local" ]]; then
    ./build.sh --clean
    mkdir build
    mkdir out
    cd build
    cmake .. -DLOCAL_BUILD=ON
    make install
    mv install/* ../out
    cd ..
elif [[ $1 == "--system" ]]; then
    ./build.sh --clean
    mkdir build
    cd build
    cmake ..
    sudo make install
    cd ..
elif [[ $1 == "--test" ]]; then
    runnable_build $2
    ./build/test
    rm build/test
elif [[ $1 == "--debug" ]]; then
    runnable_build $2
    valgrind -s --leak-check=full --track-origins=yes ./build/test
    rm build/test
elif [[ $1 == '--example' ]]; then
    test_build $2
else 
    printf "Must provide an argument! For help, use build.sh --help\n"
fi
