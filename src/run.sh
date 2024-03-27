## script to run an example
## run "chmod +x run.sh" in src folder to give permission

g++ main.cpp cpm.cpp -o run -O3 
if [ $? -eq 0 ]; then
    echo "Compilation successful."
    ./run -f ../example/tiny_example.txt -m "last_ten" -c 11 -t 0.1 -l 0 -g 0
else
    echo "Compilation failed."
fi
