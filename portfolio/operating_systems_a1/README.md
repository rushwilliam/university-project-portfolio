### CIS\*3110-W25-A1
### William Rush
### January 27 2025

This assignment was made using the starting code provided in the course directory.
* author: Hamilton-Wright, Andrew
* name: Memory Manager Starter Program
* year: 2025
* source: Operating Systems Course Directory, School of Computer Science,
	University of Guleph

**Which algorithm manages memory in the least overall number of chunks?**

Since the number of allocated chunks should be the same (assuming that all allocations are successful), then in order to reduce the number of overall chunks, the number of free chunks must be reduced. This can be done by allocating memory in a free chunk that is exactly the right size. If the chunk size is random, then all three algorithms should perform about the same, since finding chunks of the exact right size would be a coincidence. It could be said that best-fit will be the most likely to find these chunks of exact size, however it also tends to create many small free chunks which are difficult to fill. In the case that chunks of the same size are frequently allocated and deallocated, then the best-fit model should perform the best, since it will consistently allocate new chunks in free spaces of exactly the right size. For example:

**test1.txt, 4000 bytes**
```
A:0:1000:A
A:1:100:B
A:2:100:C
A:3:100:D
A:4:100:E
A:5:100:F
F:0
F:2
F:4
A:6:100:G
A:7:100:H
```
First-fit:
```
SUMMARY
580 bytes allocated
3420 bytes free
9 allocation chunks
chunk  0 location    0:116 bytes - allocated
chunk  1 location  116:116 bytes - allocated
chunk  2 location  232:784 bytes - free
chunk  3 location 1016:116 bytes - allocated
chunk  4 location 1132:116 bytes - free
chunk  5 location 1248:116 bytes - allocated
chunk  6 location 1364:116 bytes - free
chunk  7 location 1480:116 bytes - allocated
chunk  8 location 1596:2404 bytes - free
```
Best-fit:
```
SUMMARY
580 bytes allocated
3420 bytes free
7 allocation chunks
chunk  0 location    0:1016 bytes - free
chunk  1 location 1016:116 bytes - allocated
chunk  2 location 1132:116 bytes - allocated
chunk  3 location 1248:116 bytes - allocated
chunk  4 location 1364:116 bytes - allocated
chunk  5 location 1480:116 bytes - allocated
chunk  6 location 1596:2404 bytes - free
```
Worst-fit:
```
SUMMARY
580 bytes allocated
3420 bytes free
9 allocation chunks
chunk  0 location    0:1016 bytes - free
chunk  1 location 1016:116 bytes - allocated
chunk  2 location 1132:116 bytes - free
chunk  3 location 1248:116 bytes - allocated
chunk  4 location 1364:116 bytes - free
chunk  5 location 1480:116 bytes - allocated
chunk  6 location 1596:116 bytes - allocated
chunk  7 location 1712:116 bytes - allocated
chunk  8 location 1828:2172 bytes - free
```

In this case, first-fit uses the large block of memory at the start, and worst-fit uses the large block of memory at the end. Only best-fit is able to find the chunks of exact size in the middle. This scenario is fairly common, for example if a linked list of structures or objects regularly grows and shrinks.

**Which algorithm manages memory with the greatest number of successful allocations?**

For similar reasons to the first question, the greatest number of successful allocations can be often left to chance, with each algorithm performing better in very specific scenarios. For example, best-fit can leave larger chunks for large memory allocations, and worst-fit can leave many medium sized chunks. In the case described above where many identically sized structures are allocated and freed, best-fit will manage memory efficiently, leaving large free chunks. For example:

**test2.txt, 1000 bytes**
```
A:0:100:A
A:1:100:B
A:2:100:C
A:3:100:D
A:4:100:E
F:0
F:1
F:3
A:5:100:F
A:6:100:G
A:7:400:H
```
First-fit:
```
SUMMARY
880 bytes allocated
120 bytes free
7 allocation chunks
chunk  0 location    0:116 bytes - allocated
chunk  1 location  116:116 bytes - allocated
chunk  2 location  232:116 bytes - allocated
chunk  3 location  348:116 bytes - free
chunk  4 location  464:116 bytes - allocated
chunk  5 location  580:416 bytes - allocated
chunk  6 location  996:4 bytes - free

```
Best-fit:
```
SUMMARY
880 bytes allocated
120 bytes free
7 allocation chunks
chunk  0 location    0:116 bytes - allocated
chunk  1 location  116:116 bytes - free
chunk  2 location  232:116 bytes - allocated
chunk  3 location  348:116 bytes - allocated
chunk  4 location  464:116 bytes - allocated
chunk  5 location  580:416 bytes - allocated
chunk  6 location  996:4 bytes - free

```
Worst-fit:
```
SUMMARY
464 bytes allocated
536 bytes free
7 allocation chunks
chunk  0 location    0:232 bytes - free
chunk  1 location  232:116 bytes - allocated
chunk  2 location  348:116 bytes - free
chunk  3 location  464:116 bytes - allocated
chunk  4 location  580:116 bytes - allocated
chunk  5 location  696:116 bytes - allocated
chunk  6 location  812:188 bytes - free
```

In this test worst-fit fails the final allocation, because it does not leave a large enough space open. First-fit succeeds here, but it could be that the desired chunk in memory is at the start, in which case only best-fit would succeed.