## script to run an example
## run "chmod +x run_mutate.sh" in src folder to give permission

g++ mutate_main.cpp mutate.cpp -o mutate -O3
if [ $? -eq 0 ]; then
    echo "Compilation successful."
    ./mutate -f ../example/tiny_example.txt -m 0.5
else
    echo "Compilation failed."
fi