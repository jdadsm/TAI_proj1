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
g++ main.cpp cpm.cpp -o run -O3 
```

- Then, run the compiled program while specifying the parameters:
```bash
./run -f [filename] -m [method] -c [chunkSize] -t [threshold] -l [logs] -g [grid_search]

filename - The name of the origin file.
method - There are two methods: last ten where it only used the last ten characters in consideration for the probability distribution and the default method that uses all the characters before in that iteration.
chunkSize - The size of the kmer.
threshold - The percentage defined to choose when the model moves on to the next kmer. It represents the number of misses over the number of tries.
logs - This value is set to 1 to write the logs into the results.txt file and set to 0 if no logs are wanted.
grid_search - This value is set to 1 if we want to iterate over many values for every parameter. The results of this grid search will be written into the grid_search_iterations.txt file. If the value is set to 0 it will execute the program with either the default values or the values set with the other flags.
```

- To do the same for mutate, first compile the program:
```bash
g++ mutate_main.cpp mutate.cpp -o mutate -O3
```

- Then, run the compiled program while specifying the parameters:
```bash
./mutate -f [filename] -m [mutateChance]

filename - The name of the origin file.
mutateChance - The probability of a character being mutated.
```
