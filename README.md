# DataStructuresForArrays
B.Sc. project done at DTU exploring data structures for arrays

## Abstract
This thesis investigates different ways to implement data structures for indexable dynamic sets such as arrays that provide efficient element retrieval, insertion, and removal.
The traditional array has constant time retrieval, but insertion and removal have $O(n)$ time complexities, which can be improved by a constant factor with dynamic arrays.
This paper introduces the tiered vector and analyses and discusses 1- and 2-tiered vectors for a total of seven different implementations.
Insertions and removals in 2-tiered vectors have an amortised time complexity of $O(\sqrt{n})$.
This time complexity can also be deamortised at the cost of extra space consumption. 
Element retrievals run in $O(1)$ time.
It is shown that 2-tiered vectors can achieve very fast running times when implemented cleverly by utilising bit tricks.
Retrieval is slower than in the standard dynamic array implementation in C++ by a factor $1.6$ to $6.5$.
The insertion and removal times in the 2-tiered vectors immediately outperform any other implementation.
Until the 2-tiered vectors contain upwards of a million elements, their insertion and removal times are also notably faster than those of balanced binary search trees whose asymptotic running times are $O(\log(n))$.
