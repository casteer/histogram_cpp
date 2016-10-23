# histogram_cpp

I needed a short c++ program that calculates the histogram of a bunch of numbers on the command-line. 

## Dependencies

Aside from standard C++ (vectors, This program uses the really nice Boost::Accumulators and Boost::Program_Options so you'll need these. 
 
## Compilation 

So to compile this, please use: 

g++ histogram.cpp -o histogram -lboost_program_options

Then to get help for the calling parameters etc, do 

histogram --help 

...and follow the instructions. 

## Use cases

The first is to use it simply with a data file: 

./histogram -i data.file --min 0.0 --max 100.0 --bins 101 

Then it will write the histogram to the terminal. 


