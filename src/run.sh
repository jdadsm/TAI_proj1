## script to run an example
## run "chmod +x run.sh" in src folder to give permission

g++ main.cpp cpm.cpp -o run -O3 

if [ $? -eq 0 ]; then
    echo "Compilation successful."
    ./run ../example/tiny_example.txt
else
    echo "Compilation failed."
fi
