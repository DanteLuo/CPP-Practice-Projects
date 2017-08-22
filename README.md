# Hunting Project

This is a repo for the first project so called Hunting Project.

Please running cmd ./Hunting at terminal and -h for help and see those options.

The method is either stack or queue based method for searching the route.

The map format should follow:

M
size of a farm
level of a farm
Graphical Map

Here is an example:

M

4 

1 

// farm 0

S..B 

#... 

.#.. 

#... 

S - starting point

B - destination

\# - not able to pass

. - free space

The goal is to start from S and get B finally with a rule based searching method.

You should prepare the map and select correct options. Here is an excution example:
1. ./Hunting -s -o M
2. Input:

   
    M
    
    4
    
    1
    
    // farm 0
    
    S..B 
    
    #... 
    
    .#.. 
    
    #...
3. The results will be shown!
