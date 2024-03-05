## script to run an example
## run "chmod +x run.sh" in src folder to give permission

g++ main.cpp cpm.cpp -o run

if [ $? -eq 0 ]; then
    echo "Compilation successful."
    ./run ../example/tiny_example.txt 4
else
    echo "Compilation failed."
fi
