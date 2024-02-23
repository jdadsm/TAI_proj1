## script to run an example
## run "chmod +x run.sh" in src folder to give permission

g++ -o cpm cpm.cpp

if [ $? -eq 0 ]; then
    echo "Compilation successful."
    ./cpm ../example/tiny_example.txt 4
else
    echo "Compilation failed."
fi
