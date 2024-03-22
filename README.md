# To run a predefined example, use the commands below in the src folder

- First, give permission to execute the script:
```bash
chmod +x run.sh
``` 

- Then, run the script:
```bash
./run.sh
```

- To run an example of mutate, first, give permission to execute the script:
```bash
chmod +x run_mutate.sh
``` 

- Then, run the script:
```bash
./run_mutate.sh
```

# To run the program while choosing the parameters, use the commands below in the src folder

- First, compile the program:
```bash
c++ main.cpp cpm.cpp -o run
```

- Then, run the compiled program while specifying the parameters:
```bash
./run <filename> [chunkSize] [threshold] [logs]
```

- To do the same for mutate, first compile the program:
```bash
c++ mutate_main.cpp mutate.cpp -o mutate
```

- Then, run the compiled program while specifying the parameters:
```bash
./mutate <filename> [mutateChance]
```
