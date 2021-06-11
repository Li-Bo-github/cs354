////////////////////////////////////////////////////////////////////////////////
// Main File:        myHeap.c
// This File:        myHeap.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Spring 2021
// Instructor:       deppeler
// 
// Author:           Bo Li
// Email:            bli379@wisc.edu
// CS Login:         bol
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2021 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2020, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////

// DEB'S PARTIAL SOLUTION FOR SPRING 2021 DO NOT SHARE

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "myHeap.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

    int size_status;
    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */
int heapspace = 0;
 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 * - If a BEST-FIT block found is NOT found, return NULL
 *   Return NULL unable to find and allocate block for desired size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* myAlloc(int size) {       
    //TODO: Your code goes in here.
	static int called = 0;

	if (called < 1){
		heapspace = heapStart->size_status - 2;
	}
	called += 1;
	
	//Check size
	if (0 > size || heapspace < size){
		return NULL;
	}
	
	//Determine block size
	int padsize = (8- ((size + sizeof(blockHeader)) % 8)) % 8;
    int blocksize = size + padsize + sizeof(blockHeader);
	
	blockHeader *current = heapStart;
	blockHeader *next_current;
	blockHeader *bestHeap = NULL;
	char *t_begin = NULL;
	int t_size;
	int header_size;
	int p_bit;
	int a_bit;
	int pre_best_size;
	
	while (current->size_status != 1) {
        t_begin = (char*)current;
        header_size = current->size_status;
		p_bit = header_size & 2;
		a_bit = header_size & 1;
		t_size = header_size - p_bit - a_bit;
		next_current = (blockHeader*)((char*)current + t_size);
		
		//free block
		if (a_bit == 0){
			//check block size
			//equal size
			if (t_size == blocksize){
				//update a-bit
				current->size_status += 1;
				//update p-bit
				if (next_current->size_status != 1){
					next_current->size_status += 2;
				}
				return t_begin + sizeof(blockHeader);
			}
			//size is larger
			if (t_size > blocksize){
				if (bestHeap == NULL){
					bestHeap = current;
				}else{
					pre_best_size = bestHeap->size_status;
					//update if current size is samller
					if ( (pre_best_size / 8) > (t_size / 8)){
						bestHeap = current;
					}
				}
			} 
		}
		
		
		current = next_current;
	}
	
	//NOT found
	if (bestHeap == NULL){
		return NULL;
	}
	
	//SPLIT the free block into two valid heap blocks
	int total_size = bestHeap->size_status;
	total_size = (total_size / 8 )*8;
	int free_size = total_size - blocksize;
	
	//update alloc block
	bestHeap->size_status -= free_size;
	bestHeap->size_status += 1;
	
	//update free block
	blockHeader *free_block = (blockHeader*)((void*)bestHeap + blocksize);
	//set p-bit
	free_block->size_status = free_size + 2;
	//set footer
	blockHeader *free_footer = (blockHeader*)((void*)bestHeap + (total_size - sizeof(blockHeader)));
	free_footer->size_status = free_size;
	
	
	return (char*)bestHeap + sizeof(blockHeader);
} 
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
int myFree(void *ptr) {    
    //TODO: Your code goes in here.
	char*current = ptr - sizeof(blockHeader);
	blockHeader *header = (blockHeader *)current;
	
	//Return -1 if ptr is NULL.
	if(current == NULL){
		return -1;
	}
	
	//Return -1 if ptr is not a multiple of 8.
	if(((unsigned long int)current - (unsigned long int)(char*)heapStart) % 8 != 0){
		return -1;
	}
	
	//Return -1 if ptr is outside of the heap space.
	if((unsigned long int)current > (unsigned long int)((char*)heapStart + heapspace)){
		return -1;
	}
	
	//Return -1 if ptr block is already freed.
	if(((header->size_status) & 1) == 0){
		return -1;
	}
	
	//free block
	int header_size = header->size_status;
	int p_bit = header_size & 2;
	int a_bit = header_size & 1;
	int size = header_size - p_bit - a_bit;
	
	//set a-bit to 0
	header->size_status -= 1;
	//set footer
	blockHeader *footer = (blockHeader*)(current + size - sizeof(blockHeader));
	footer->size_status = size;
	
	//set next block p-bit to 0
	blockHeader *next = (blockHeader*)(current + size);
	if (next->size_status != 1){
		next->size_status -= 2;
	}
	
    return 0;
} 

/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for delayed coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
    //TODO: Your code goes in here.
	blockHeader *current = heapStart;
	blockHeader *next_current;
	blockHeader *footer;
	
	int adjacent_free = 0;
	
	int header_size;
	int p_bit;
	int a_bit;
	int size;
	
	int next_header_size;
	int next_p_bit;
	int next_a_bit;
	int next_size;
	
	//loop until end
	while (current->size_status != 1){
			
		dispMem();
		
		header_size = current->size_status;
		p_bit = header_size & 2;
		a_bit = header_size & 1;
		size = header_size - p_bit - a_bit;
		next_current = (blockHeader*)((char*)current + size);
		
		//find the first free
		if (a_bit == 0){
			//check if next is end
			while (next_current->size_status != 1){
				//check if the second free
				next_header_size = next_current->size_status;
				next_p_bit = next_header_size & 2;
				next_a_bit = next_header_size & 1;
				next_size = next_header_size - next_p_bit - next_a_bit;
			
				//check if second is free
				if (next_a_bit == 0){				
					//change size of fist
					current->size_status += next_size;
				
					//change footer at second
					footer = (blockHeader*)((char *)next_current + (next_size - sizeof(blockHeader)));
					footer->size_status = size + next_size;
					
					//track num of adjacent_free
					adjacent_free += 1;
					
					//find next block
					header_size = current->size_status;
					size = header_size - p_bit - a_bit;
					next_current = (blockHeader*)((char*)current + size);
				
				}else{
					//next is alloc, end this loop
					break;
				}
			}
			
		}
		//look at next
		current = next_current;
	
	}
	
	//returns 0 if there were no adjacent
	return adjacent_free;
}

 
/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int myInit(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple myInit calls
 
    int pagesize;   // page size
    int padsize;    // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dispMem() {     
 
    int counter;
    char status[6];
    char p_status[6];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, 
	"*********************************** Block List **********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
    fprintf(stdout, 
	"*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
	"*********************************************************************************\n");
    fflush(stdout);

    return;  
} 


// end of myHeap.c (sp 2021)                                         

