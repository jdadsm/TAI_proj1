# To run a predefined example, use the commands below in the src folder

- First, give permissions:
```bash
chmod +x run.sh
``` 

- Then, run the script:
```bash
./run.sh
```

# To run the program while choosing the parameters, use the commands below in the src folder

- First, compile the program:
```bash
c++ main.cpp cpm.cpp generator.cpp -o run
```

- Then, run the compiled program while specifying the parameters:
```bash
./run <filename> [chunkSize]
```