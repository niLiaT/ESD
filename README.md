# Fog Computing Server Deployment

## Environments
* Ubuntu 18.04
* C++ 17
* gcc 7.4.0

## File Description
* bin  
  |\_\_sefsd.out: object file of the proposed mtethod  
  |\_\_dmga.out: object file of discrete monkey genetic algorithm  
  |\_\_bat.out: object file of binary bat algorithm  
  |\_\_ga.out: object file of genetic algorithm  
  |\_\_tf.out: object file of top first  
  |\_\_s_g_compare.out: object file of the comparison experiment of searcher and goods  
  |\_\_generator.out: object file of dataset generator  
* dataset  
  |\_\_generator.cc: c++ file of dataset generator  
  |\_\_taipei_cartesian.txt: dataset of base station in taipei in forms of cartesian coordinates  
  |\_\_taipei_gps.txt: dataset of base station in taipei in forms of geographic coordinate system
  * dataset naming rule: gateway_fog_edge_user_x_y.txt
* result  
  |\_\_fog_edge_user_x_y: experimental results of different dataset  
  |\_\_gamma: experimantal results of different gamma value  
  |\_\_graph: gnuplot file of experiments  
  |\_\_parameter: experimental results of different parameters  
  |\_\_s_g_compare: experimental results of the comparison experiment of searchers and goods  
  |\_\_taipei: experimental results of taipei base station dataset  
* script  
  |\_\_cal_statistics.sh: shell script to calculate average value of a experiment  
  |\_\_others: shell script of experiments
* source  
  |\_\_connection.cc: connect the devices and calculate objective value  
  |\_\_se.cc: search economics  
  |\_\_main.cc: driver  

## Execute
* SE
```
./bin/($algorithm).out ($dataset).txt $iteration $solution_length $regions $searchers $goods $players
```
* DMGA
```
./bin/dmga.out ($dataset).txt $iteration $solution_length $population
```
* BAT
```
./bin/bat.out ($dataset).txt $iteration $solution_length $polpulation
```
* GA
```
./bin/ga.out ($dataset).txt $iteration $solution_length $population $players $crossover_rate $mutation_rate
```
* TF
```
./bin/tf.out ($dataset).txt $solution_length
```

## Compile
g++ src/main.cc src/connection.cc src/($algo).cc -O3 -o ($algo).out