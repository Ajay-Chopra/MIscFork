# My First Genetic Algorithm

This project was my first foray into writing a genetic algorithm. The function of the program is to start with a population of random strings
and continually evolve until the target string is reached. While the application is rather trivial, the aim of this program was merely as
a self-teaching exercise.


### Algorithm Overview

The algorithm consists of the following:

1. Generate initial population of N elements

While (Target is not Reached):
  2. Calculate the fitness of each member of the population
  
  3. Put members into a mating pool based on fitness
  
  4. Perform reproduction
  
    4a. Crossover
    
    4b. Mutation
    
  5. Replace old population with next generation
  
  6. Check if Target is Reached
  
 ### Fitness Function and Mating Pool Selection
 
 Fitness was calculated based on how many characters in a given string were equal to the target string. This number was divided by the
 length of the string. Members were then placed in a mating pool based on their fitness score. Members with higher fitness scores had higher
 occurence in the matnig pool and thus, were more likely to be selected for reproduction.
 
 
 ### How to run program
 To run this program, all that is needed is a C++ compiler and an IDE. Upon running the executeable, the process will begin and display
 the current generation, average fitness of the population and the string with the highest fitness. The user can feel free to adjust the
 target string, mutation rate and initial population size to see how they affet performance.
 
 
  ### Acknowledgements
  Huge credit goes to David Shiffman aka The Coding Train for giving me the idea for this project.
 
 
 Author: Ajay Chopra
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
