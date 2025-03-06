/*
This assignment was made using the starting code provided in the course directory.
* author: Hamilton-Wright, Andrew
* name: Memory Manager Starter Program
* year: 2025
* source: Operating Systems Course Directory, School of Computer Science,
	University of Guleph
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "mmanager.h"

#define OVERHEAD (sizeof(int) * 2 + sizeof(long))

int runModel(FILE *outputfp, FILE *inputfp,
		long totalMemorySize, int fitStrategy,
		int verbosity
	)
{
	char *memoryBlock = NULL;
	int nSuccessfulActions = 0;
	mmgr_action action;

	fprintf(outputfp,
			"Running a %s-fit model in %ld (0x%lx) bytes of memory.\n",
			(fitStrategy == STRAT_FIRST ? "first" :
				fitStrategy == STRAT_BEST ? "best" :
					fitStrategy == STRAT_WORST ? "worst" : "unknown"),
			totalMemorySize, totalMemorySize);

	/**
	 * this is the only allocation you should need -- all requests
	 * from your model should come from this allocated block
	 */
	memoryBlock = (char *) malloc(totalMemorySize);
	if (memoryBlock == NULL) {
		perror("allocation failed!");
		return -1;
	}

	/**
	 *	+++ Set up anything else you will need for your memory management
	 */
	// The start of the linked list
	long head = -1;
	while (getAction(&action, inputfp, outputfp, verbosity) > 0)
	{
		if (action.type == ACTION_ALLOCATE)
		{
			// /* +++ do an allocation */
			printf("alloc %d bytes : ", action.size);
			// Check for the signal that memory is empty
			if (head == -1) {
				// List is empty
				if (totalMemorySize < action.size + OVERHEAD) {
					// There is no space in the empty list
					printf("FAIL\n");
					continue;
				}
				// There is space in the empty list
				writeAction(memoryBlock, 0, action);
				writeNext(memoryBlock, 0, -1);
				paintMemory(memoryBlock, OVERHEAD, action.size, action.paint);
				head = 0;
				printf("SUCCESS - return location 0\n");
				continue;
			}
			if (fitStrategy == STRAT_BEST) {
				// Block before the best location
				long bestPrevLocation = -1;
				// Size of the block before the best location
				long bestSize = -1;
				if (head >= action.size + OVERHEAD) {
					// There is room at the start of the list
					bestPrevLocation = -2; // Indicate the new block should be at the head
					bestSize = head;
				}
				long location = head;
				// Loop through the list to find the best location
				while (location != -1) {
					long next = readNext(memoryBlock, location);
					// Calculate the location of the new block
					long newLocation = location + OVERHEAD + readSize(memoryBlock, location);
					if (next == -1) {
						// Search at the end of the list
						if (totalMemorySize - newLocation < action.size + OVERHEAD) {
							// There is no room for the memory allocation at the end of the list
							break;
						}
						if (totalMemorySize - newLocation < bestSize || bestPrevLocation == -1) {
							// There is room at the end of the list and it is the best location
							bestPrevLocation = location;
							bestSize = totalMemorySize - newLocation;
						}
						break;
					}
					if (next - newLocation >= action.size + OVERHEAD && (next - newLocation < bestSize || bestPrevLocation == -1)) {
						// There is enough room in the middle of the list and it is the best location
						bestPrevLocation = location;
						bestSize = next - newLocation;
					}
					location = next;
				}
				if (bestPrevLocation == -1) {
					// There is not enough space in the list
					printf("FAIL\n");
					continue;
				}
				if (bestPrevLocation == -2) {
					// There is space at the start of the list
					writeAction(memoryBlock, 0, action);
					writeNext(memoryBlock, 0, head);
					paintMemory(memoryBlock, OVERHEAD, action.size, action.paint);
					head = 0;
					printf("SUCCESS - return location 0\n");
					continue;
				}
				// There is space in the middle of the list
				long bestLocation = readNext(memoryBlock, bestPrevLocation);
				long newLocation = bestPrevLocation + OVERHEAD + readSize(memoryBlock, bestPrevLocation);
				writeAction(memoryBlock, newLocation, action);
				writeNext(memoryBlock, newLocation, bestLocation);
				writeNext(memoryBlock, bestPrevLocation, newLocation);
				paintMemory(memoryBlock, newLocation + OVERHEAD, action.size, action.paint);
				printf("SUCCESS - return location %ld\n", newLocation);
			} else if (fitStrategy == STRAT_WORST) {
				// Block before the worst location
				long worstPrevLocation = -1;
				// Size of the block before the worst location
				long worstSize = -1;
				if (head >= action.size + OVERHEAD) {
					// There is room at the start of the list
					worstPrevLocation = -2; // Indicate the new block should be at the head
					worstSize = head;
				}
				long location = head;
				// Loop through the list to find the worst location
				while (location != -1) {
					long next = readNext(memoryBlock, location);
					// Calculate the location of the new block
					long newLocation = location + OVERHEAD + readSize(memoryBlock, location);
					if (next == -1) {
						// Search at the end of the list
						if (totalMemorySize - newLocation < action.size + OVERHEAD) {
							// There is no room for the memory allocation at the end of the list
							break;
						}
						if (totalMemorySize - newLocation > worstSize || worstPrevLocation == -1) {
							// There is room at the end of the list and it is the worst location
							worstPrevLocation = location;
							worstSize = totalMemorySize - newLocation;
						}
						break;
					}
					if (next - newLocation >= action.size + OVERHEAD && (next - newLocation > worstSize || worstPrevLocation == -1)) {
						// There is enough room in the middle of the list and it is the worst location
						worstPrevLocation = location;
						worstSize = next - newLocation;
					}
					location = next;
				}
				if (worstPrevLocation == -1) {
					// There is not enough space in the list
					printf("FAIL\n");
					continue;
				}
				if (worstPrevLocation == -2) {
					// There is space at the start of the list
					writeAction(memoryBlock, 0, action);
					writeNext(memoryBlock, 0, head);
					paintMemory(memoryBlock, OVERHEAD, action.size, action.paint);
					head = 0;
					printf("SUCCESS - return location 0\n");
					continue;
				}
				// There is space in the middle of the list
				long worstLocation = readNext(memoryBlock, worstPrevLocation);
				long newLocation = worstPrevLocation + OVERHEAD + readSize(memoryBlock, worstPrevLocation);
				writeAction(memoryBlock, newLocation, action);
				writeNext(memoryBlock, newLocation, worstLocation);
				writeNext(memoryBlock, worstPrevLocation, newLocation);
				paintMemory(memoryBlock, newLocation + OVERHEAD, action.size, action.paint);
				printf("SUCCESS - return location %ld\n", newLocation);
			} else if (fitStrategy == STRAT_FIRST) {
				if (head > action.size + OVERHEAD) {
					// There is room before the first block
					writeAction(memoryBlock, 0, action);
					writeNext(memoryBlock, 0, head);
					paintMemory(memoryBlock, OVERHEAD, action.size, action.paint);
					head = 0;
					printf("SUCCESS - return location 0\n");
					continue;
				}
				long location = head;
				while (1) {
					long next = readNext(memoryBlock, location);
					long newLocation = location + OVERHEAD + readSize(memoryBlock, location);
					if (next == -1) {
						// The end of the list has been reached
						if (totalMemorySize - newLocation < action.size + OVERHEAD) {
							// There is not enough space at the end of the list
							printf("FAIL\n");
							break;
						}
						// There is enough space at the end of the list for an allocation
						writeAction(memoryBlock, newLocation, action);
						writeNext(memoryBlock, newLocation, -1);
						writeNext(memoryBlock, location, newLocation);
						paintMemory(memoryBlock, newLocation + OVERHEAD, action.size, action.paint);
						printf("SUCCESS - return location %ld\n", newLocation);
						break;
					}
					// We are in the middle of the list
					if (next - newLocation >= action.size + OVERHEAD) {
						// There is enough space between this block and the next
						writeAction(memoryBlock, newLocation, action);
						writeNext(memoryBlock, newLocation, next);
						writeNext(memoryBlock, location, newLocation);
						paintMemory(memoryBlock, newLocation + OVERHEAD, action.size, action.paint);
						printf("SUCCESS - return location %ld\n", newLocation);
						break;
					}
					location = next;
				}
			}
		} else
		{
			/* +++ do a release */
			if (head == -1) {
				printf("Could not deallocate id %d\n", action.id);
				continue;
			}
			if (readId(memoryBlock, head) == action.id) {
				// Deallocate the head of the list
				printf("free location %ld\n", head);
				head = readNext(memoryBlock, head);
				continue;
			}
			long current = head;
			while (1) {
				if (readNext(memoryBlock, current) == -1) {
					// Block with the given id could not be found
					// Placed within the loop because there will be no indication after the loop ends wether
					// or not anything was successfully deallocated
					printf("Could not deallocate ID %d\n", action.id);
					break;
				}
				if (readId(memoryBlock, readNext(memoryBlock, current)) == action.id) {
					// Deallocate memory
					printf("free location %ld\n", readNext(memoryBlock, current));
					writeNext(memoryBlock, current, readNext(memoryBlock, readNext(memoryBlock, current)));
					break;
				}
				current = readNext(memoryBlock, current);
			}
		}
		/** increment our count of work we did */
		nSuccessfulActions++;
	}
	// Number of allocations in memory
	int allocations = 0;
	long location = head;
	// Count the number of allocations
	while (location != -1) {
		allocations++;
		location = readNext(memoryBlock, location);
	}
	location = head;
	// Number of chunks (allocated and free)
	int chunks = 0;
	// Number of bytes allocated
	long bytesAllocated = 0;
	// Number of bytes free
	long bytesFree = 0;
	// Count the number of chunks and bytes allocated and free
	if (head > 0) {
		chunks++;
		// Add the free bytes at the start of the list
		bytesFree += head;
	}
	for (int i = 0; i < allocations; i++) {
		chunks++;
		// Add the allocated bytes of the current block
		bytesAllocated += readSize(memoryBlock, location) + OVERHEAD;
		if (readNext(memoryBlock, location) == -1) {
			// End of list
			if (location + OVERHEAD + readSize(memoryBlock, location) < totalMemorySize) {
				chunks++;
				// Add the free bytes between this block and the end of the list
				bytesFree += totalMemorySize - (location + OVERHEAD + readSize(memoryBlock, location));
			} 
		} else if (location + OVERHEAD + readSize(memoryBlock, location) < readNext(memoryBlock, location)) {
			chunks++;
			// Add the free bytes between this block and the next block
			bytesFree += readNext(memoryBlock, location) - (location + OVERHEAD + readSize(memoryBlock, location));
		}
		location = readNext(memoryBlock, location);
	}
	// Print the summary
	printf("SUMMARY\n");
	printf("%ld bytes allocated\n", bytesAllocated);
	printf("%ld bytes free\n", bytesFree);
	printf("%d allocation chunks\n", chunks);
	location = head;
	// Reset to print chunk numbers starting at 0
	chunks = 0;
	// Print chunk information
	if (head > 0) {
		printf("chunk %2d location %4d:%ld bytes - free\n", chunks, 0, head);
		chunks++;
	}
	for (int i = 0; i < allocations; i++) {
		printf("chunk %2d location %4ld:%lu bytes - allocated\n",
			chunks, location, readSize(memoryBlock, location) + OVERHEAD);
		chunks++;
		if (readNext(memoryBlock, location) == -1) {
			if (location + OVERHEAD + readSize(memoryBlock, location) < totalMemorySize) {
				printf("chunk %2d location %4ld:%lu bytes - free\n",
					chunks, location + OVERHEAD + readSize(memoryBlock, location),
					totalMemorySize - (location + OVERHEAD + readSize(memoryBlock, location)));
				chunks++;
			} 
		} else if (location + OVERHEAD + readSize(memoryBlock, location) < readNext(memoryBlock, location)) {
			printf("chunk %2d location %4ld:%lu bytes - free\n",
				chunks, location + OVERHEAD + readSize(memoryBlock, location), 
				readNext(memoryBlock, location) - (location + OVERHEAD + readSize(memoryBlock, location)));
			chunks++;
		}
		location = readNext(memoryBlock, location);
	}
	/** +++ Clean up your memory management */
	free(memoryBlock);
	return nSuccessfulActions;
}

// Write the id and size of an action
void writeAction(char *memoryBlock, long location, mmgr_action action) {
	// Pointer to the id
	char *idPointer = (char *)&(action.id);
	// Pointer to size
	char *sizePointer = (char *)&(action.size);
	for (int i = 0; i < sizeof(int); i++) {
		// Extract the bytes of the pointers and write them to memory
		memoryBlock[location + i] = idPointer[i];
		memoryBlock[location + sizeof(int) + i] = sizePointer[i];
	}
}

// Write the pointer to the next chunk of allocated memory
void writeNext(char *memoryBlock, long location, long next) {
	// Move the location past id and size
	location += sizeof(int) * 2;
	// Pointer to next
	char *nextPointer = (char *)&next;
	for (int i = 0; i < sizeof(long); i++) {
		// Extract the bytes write them to memory
		memoryBlock[location + i] = nextPointer[i];
	}
}

// Read the id of a chunk
int readId(char *memoryBlock, long location) {
	int id = 0;
	// Pointer to the id to be read
	char *idPointer = (char *)&id;
	for (int i = 0; i < sizeof(int); i++) {
		// Extract the bytes from memory and store them in the variable
		idPointer[i] = memoryBlock[location + i];
	}
	return id;
}

// Read the size of a chunk
int readSize(char *memoryBlock, long location) {
	// Move the location past id
	location += sizeof(int);
	int size = 0;
	// Pointer to the size to be read
	char *sizePointer = (char *)&size;
	for (int i = 0; i < sizeof(int); i++) {
		// Extract the bytes from memory and store them in the variable
		sizePointer[i] = memoryBlock[location + i];
	}
	return size;
}

// Read the next pointer
long readNext(char *memoryBlock, long location) {
	long next = 0;
	// Move the location past id and size
	location += sizeof(int) * 2;
	// Pointer to next
	char *nextPointer = (char *)&next;
	for (int i = 0; i < sizeof(long); i++) {
		// Extract the bytes from memory and store them in the variable
		nextPointer[i] = memoryBlock[location + i];
	}
	return next;
}

// Paint a specific chunk of memory with a character
void paintMemory(char *memoryBlock, long location, int size, char paintCharacter) {
	for (int i = 0; i < size; i++) {
		memoryBlock[location + i] = paintCharacter;
	}
}