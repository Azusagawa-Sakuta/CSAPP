#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define FILE_LENGTH_MAX 500
#define SPACE_LENGTH 1
#define INSTRUCTION_BEGIN SPACE_LENGTH
#define INSTRUCTION_LENGTH 1
#define ADDRESS_BEGIN INSTRUCTION_BEGIN + INSTRUCTION_LENGTH + SPACE_LENGTH
#define SPACE_MAX 1
typedef long long int64;
typedef unsigned long long uint64;

int hit_cnt, miss_cnt, evic_cnt;

// Output the usage.
void usage(char* pname) {
    printf( "Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n",
            pname );
    printf( "Options:\n"\
            "  -h         Print this help message.\n"\
            "  -v         Optional verbose flag.\n"\
            "  -s <num>   Number of set index bits.\n"\
            "  -E <num>   Number of lines per set.\n"\
            "  -b <num>   Number of block offset bits.\n"\
            "  -t <file>  Trace file.\n"\
            "\n"\
            "Examples:\n"\
            "  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n"\
            "  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n" );
}

// Check if it is valid and can be converted to num.
void checkArgv(int num, char* pname) {
    if (num <= 0) {
        fprintf( stderr,
                 "%s: Missing required command line argument\n",
                 pname );
        usage(pname);
        exit(2);
    }
}

// Warn the user who uses one param twice or more.
void reuseError(char flag, char* pname) {
    fprintf(stderr, "%s: redefinition of flag '%c'\n", pname, flag);
    usage(pname);
    exit(1);
}

// flags of params [-hvsEbt]
typedef enum Flag {
    FLAG_H = 0, FLAG_V, FLAG_S, FLAG_E, FLAG_B, FLAG_T 
} Flag;

// hash table aux func (maybe log_map_size as param)
// and return n & (log_map_size - 1) here.
uint64 hash(uint64 n, uint64 map_size) {
    return n % map_size;
}

typedef struct CacheLine {
    // Hash key for HashTable
    uint64 key;
    // Linked List pointer prev
    struct CacheLine* prev;
    // Linked List pointer next
    struct CacheLine* next; 
    // Hashmap collision pointer hash_next
    struct CacheLine* hash_next;
} CacheLine;

typedef struct CacheSet {
    // Capacity of each set in cache
    int capacity;
    // Size of each set in cache
    int size;
    // MRU of each set in cache
    struct CacheLine* head;
    // LRU of each set in cache
    struct CacheLine* tail;
    // Hashmap for TAG search
    struct CacheLine** hashmap;
} CacheSet;

// log2int(num) will return (int)log(num)
int log2int(int num) {
    int cnt = 0;
    while (num > 1) {
        num /= 2;
        cnt++;
    }
    return cnt;
}

// Allocate space for the number of SIZE of hashtables for sets
CacheLine** HashTableNew(int64 size) {
    CacheLine** table = (CacheLine**) calloc (size, sizeof(CacheLine*));
    for (int64 i = 0; i < size; i++) {
        table[i] = NULL;
    }
    return table;
}

void HashTableFree(CacheLine** table) {
    free(table);
}

// Push a recently used line into certain set
void HashTablePush(CacheSet* set, uint64 key) {
    // A new node with key of RU line
    CacheLine* node = (CacheLine*)malloc(sizeof(*node));
    *node = (CacheLine) {
        .key = key,
        .hash_next = NULL,
        .next = NULL,
        .prev = NULL
    };

    // Push the RU line into the Linked List
    // If no element
    if (!set->head) {
        set->head = set->tail = node;
        node->next = node->prev = NULL;
    }
    // If there is other elements, push it on front.
    else {
        set->head->prev = node;
        node->next = set->head;
        set->head = node;
        node->prev = NULL;
    }

    // Push the TAG into Hashtable
    uint64 hash_key = hash(key, set->capacity);
    CacheLine* cur = set->hashmap[hash_key];
    // If no collision
    if (!cur) {
        set->hashmap[hash_key] = node;
    }
    // If there is collision
    else {
        while (cur->hash_next) {
            cur = cur->hash_next;
        }
        cur->hash_next = node;
    }

    // Increase the size of lines
    set->size++;
}

// Pop the LRU line in certain set
void HashTablePop(CacheSet* set) {
    // A node of LRU line
    CacheLine* dnode = set->tail;

    // Pop the LRU line from Linked List
    // If nothing in Linked List
    if (!dnode) {
        return;
    }
    // If only one element
    else if (!dnode->prev) {
        set->head = set->tail = NULL;
    }
    // If more than one element
    else {
        dnode->prev->next = NULL;
        set->tail = dnode->prev;
    }

    // Pop the LRU line from Hashtable
    CacheLine* prev = NULL;
    uint64 hash_key = hash(dnode->key, set->capacity);
    for ( CacheLine* cur = set->hashmap[hash_key];
          cur;
          cur = cur->hash_next ) {
        if (cur->key == dnode->key) {
            // If the element is head of hashed key linked list.
            // WRONG: If only one element in the index of hashed key
            if (!prev) {
                // Error happened when set->hashmap[hash_key] = NULL;
                set->hashmap[hash_key] = cur->hash_next;
            }
            // If the index of hashed key has collision
            else {
                prev->hash_next = cur->hash_next;
            }
            break;
        }
        prev = cur;
    }

    // Free the space of LRU line
    free(dnode);

    // Decrease the size of lines
    set->size--;
}

// Search line with key
bool HashTableSearch(CacheSet* set, uint64 key) {
    uint64 hash_key = hash(key, set->capacity);
    for ( CacheLine* cur = set->hashmap[hash_key];
          cur;
          cur = cur->hash_next ) {
        if (cur->key == key) {
            return true;
        }
    }
    return false;
}

CacheSet* CacheNew(int set_size, int lines_per_set) {
    CacheSet* cache = (CacheSet*)calloc(set_size, sizeof(CacheSet));
    for (int i = 0; i < set_size; i++) {
        cache[i]      = (CacheSet) {
            .size     = 0,
            .capacity = lines_per_set,
            .head     = NULL,
            .tail     = NULL,
            .hashmap  = HashTableNew(lines_per_set)
        };
    }
    return cache;
}

// Free all cache related memory
void CacheFree(CacheSet* cache, int64 set) {
    for(int i = 0; i < set; i++) {
        CacheLine* next_node = NULL;
        for ( CacheLine* cur = cache[i].head;
              cur != cache[i].tail;
              cur = next_node ) {
            next_node = cur->next;
            free(cur);
        }
        HashTableFree(cache[i].hashmap);
    }
    free(cache);
}

// Make a line (indexed with key) to the head
// Change a certain line into MRU
void CacheMakeHead(CacheSet* set, int64 key) {
    // If the line is already MRU
    if (set->head->key == key) return;
    // Find the line through key
    CacheLine* cur = set->head;
    bool found = false;
    for ( ; cur; cur = cur->next ) {
        if (cur->key == key) {
            found = true;
            break;
        }
    }
    // If not found.
    if (!found) return;

    // If the line is LRU, remember to change the tail pointer
    if (set->tail->key == key) {
        set->tail = set->tail->prev;
    }
    
    // Disconnect this line from Linked List
    if (cur->next) cur->next->prev = cur->prev;
    if (cur->prev) cur->prev->next = cur->next;

    // Insert it to the front
    cur->next = set->head;
    set->head->prev = cur;
    cur->prev = NULL;
    set->head = cur;
}

// Seperate the hex address into tag, set_index, block_offset
// Address: |---t bits---|---s bits---|---b bits---|
//               tag       set index   block offset
void parseAddresss(char* address,
                    int s, int b,
                    uint64* block_ptr, uint64* set_ptr, uint64* tag_ptr ) {
    // Translate the hex address into unsigned long long int
    uint64 add = strtol(address, NULL, 16);
    // Mask the block, set, tag part without shifting.
    uint64 block_raw = add & ((1 << b) - 1);
    uint64 set_raw = add & ((1 << (b + s)) - 1);
    uint64 tag_raw = add;
    // Shift to reveal the true block, set, tag
    uint64 block = block_raw;
    uint64 set = set_raw >> b;
    uint64 tag = tag_raw >> (b + s);
    
    *block_ptr = block;
    *set_ptr = set;
    *tag_ptr = tag;
}

typedef enum Func {
    LOAD, STORE, MODIFY
} Func;

// Process the input address and make it the MRU line
void processData( int64 block_offset, int64 set_index, int64 tag_key,
                  CacheSet* cache,
                  bool verbose, Func func ) {
    // Pointer s is pointing to &cache[set_index]
    CacheSet* s = &cache[set_index];

    // Search the Hashtable for current tag_key
    if (HashTableSearch(s, tag_key)) {
        // If found, make it the MRU line
        CacheMakeHead(s, tag_key);
        // It's a HIT
        if (verbose) printf("hit ");
        ++hit_cnt;
    }

    // If not found, and there is capacity to push line
    else if (s->size < s->capacity) {
        // Push it into the Hashtable (a set)
        HashTablePush(s, tag_key);
        // It's a MISS
        if (verbose) printf("miss ");
        ++miss_cnt;
    }

    // If no space for another line
    else {
        // Pop the LRU line
        HashTablePop(s);
        // Push the current line
        HashTablePush(s, tag_key);
        // It's a MISS then EVICTION
        if (verbose) printf("miss eviction "); 
        ++miss_cnt, ++evic_cnt;
    }

    // Especially, when we MODIFY an address, we LOAD then STORE it.
    // So, LOAD will make the line being MRU, then STORE will definitely HIT.
    if (func == MODIFY) {
        // It's another HIT
        if (verbose) printf("hit");
        hit_cnt++;
    }
} 

// Parse the input trace file
void parseData(FILE* input,
               int s, int E, int b,
               CacheSet* cache,
               bool verbose ) {
    // Initialise buffer
    char buf[64];
    for (int i = 0; i < 64; i++) buf[i] = '\0';

    while (fgets(buf, sizeof(buf), input)) {
        // Omit instuctions begin with I
        if (buf[0] != ' ') continue;
        // Address begins at 3 and ends at ','.
        if (verbose) {
            int space_count = 0;
            for ( int i = INSTRUCTION_BEGIN;
                  buf[i] != '\n' && buf[i] != '\0';
                  i++ ) {
                // Deal with trailing spaces
                if (buf[i] == ' ') space_count++;
                if (space_count > SPACE_MAX) break;
                // print characters before a newline
                printf("%c", buf[i]);
            }
            printf(" ");
        }

        // Find the string of address
        int add_begin = ADDRESS_BEGIN;
        int add_end = 0;
        // from ADDRESS_BEGIN to a comma
        for (int i = add_begin; buf[i] != '\0'; i++) {
            if (buf[i] == ',') add_end = i;
        }
        if (!add_end) {
            fprintf( stderr,
                     "Invalid trace file." );
            exit(1);
        }

        // Store the string of address
        int add_length = add_end - add_begin;
        char* address = (char*) calloc (add_length, sizeof(char));
        for (int i = 0; i < add_length; i++) {
            address[i] = '\0';
        }
        strncpy(address, buf + add_begin, add_length);

        // Parse address to block, set and tag
        uint64 block_offset, set_index, tag_key;
        parseAddresss( address,
                       s, b,
                       &block_offset, &set_index, &tag_key );

        // Load and Store are same to cache operation
        // Modify is the addition of Load and Store
        switch (buf[1]) {
        case 'L':
            processData( block_offset, set_index, tag_key,
                         cache,
                         verbose, LOAD );
            break;
        case 'S':
            processData( block_offset, set_index, tag_key,
                         cache,
                         verbose, STORE );
            break;
        case 'M':
            processData( block_offset, set_index, tag_key,
                         cache,
                         verbose, MODIFY );
            break;
        default:
            fprintf( stderr,
                     "Invalid trace file." );
            exit(1);
        }

        if (verbose) printf("\n");
        free(address);
    }
}

int main(int argc, char* argv[])
{
    int nextarg = 1;
    bool flags[6];
    
    bool verbose = false;
    int s, E, b;

    int filelength;
    char filename[512];
    char resultname[] = ".csim_results";
    FILE* tracein;
    FILE* traceout;

    // Initialise flags of parameters.
    for (int i = 0; i < 6; i++) {
        flags[i] = false;
    }

    // If too short.
    if (argc < 2) {
        usage(argv[0]);
        exit(2);
    }

    // Parse params.
    while (nextarg < argc) {
        if (argv[nextarg][0] != '-') {
            usage(argv[0]);
            return 1;
        }
        char flag = argv[nextarg][1];
        switch (flag) {
        case 'h':   // help
            if (flags[FLAG_H]) reuseError(flag, argv[0]);
            flags[FLAG_H] = true;
            usage(argv[0]);
            return 0;
        case 'v':   // verbose?
            if (flags[FLAG_V]) reuseError(flag, argv[0]);
            flags[FLAG_V] = true;
            verbose = true;
            break;
        case 's':   // -s # for num of sets
            if (flags[FLAG_S]) reuseError(flag, argv[0]);
            flags[FLAG_S] = true;
            ++nextarg;
            if (nextarg >= argc) checkArgv(0, argv[0]);
            s = atoi(argv[nextarg]);
            checkArgv(s, argv[0]);
            break;
        case 'E':   // -E # for num of lines per set
            if (flags[FLAG_E]) reuseError(flag, argv[0]);
            flags[FLAG_E] = true;
            ++nextarg;
            if (nextarg >= argc) checkArgv(0, argv[0]);
            E = atoi(argv[nextarg]);
            checkArgv(E, argv[0]);
            break;
        case 'b':   // -b # for size of block
            if (flags[FLAG_B]) reuseError(flag, argv[0]);
            flags[FLAG_B] = true;
            ++nextarg;
            if (nextarg >= argc) checkArgv(0, argv[0]);
            b = atoi(argv[nextarg]);
            checkArgv(b, argv[0]);
            break;
        case 't':   // -t <file> for name of trace file
            if (flags[FLAG_T]) reuseError(flag, argv[0]);
            flags[FLAG_T] = true;
            ++nextarg;
            if (nextarg >= argc) checkArgv(0, argv[0]);
            filelength = strlen(argv[nextarg]) - 6;

            // If too long
            if (filelength > FILE_LENGTH_MAX) {
                fprintf( stderr,
                         "%s: file name too long\n",
                         argv[0] );
                usage(argv[0]);
                exit(1);
            }

            // If not end with ".trace"
            if (strcmp(argv[nextarg] + filelength, ".trace")) {
                fprintf( stderr,
                         "%s: invalid format of file\n",
                         argv[0] );
                usage(argv[0]);
                exit(1);
            }

            strncpy(filename, argv[nextarg], filelength);
            strcpy(filename + filelength, ".trace");

            tracein = fopen(filename, "r");
            // If cannot open
            if (!tracein) {
                fprintf( stderr,
                         "%s: can't open input file '%s'\n",
                         argv[0], filename );
                usage(argv[0]);
                exit(1);
            }
            break;
        default:
            fprintf( stderr,
                     "%s: invalid option -- '%c'\n",
                     argv[0], flag );
            usage(argv[0]);
            exit(1);
        }
        ++nextarg;
    }

    // Check if all params are filled.
    for (int i = FLAG_S; i <= FLAG_T; i++) {
        checkArgv((int)flags[i], argv[0]);
    }

    // Check if results can be written into disk.
    traceout = fopen(resultname, "w");
    if (!traceout) {
        fprintf( stderr,
                 "%s: can't open output file '%s'\n",
                 argv[0], resultname );
        exit(1);
    }

    // Allocate space for a new cache.
    int S = (int)pow(2, s);
    CacheSet* cache = CacheNew(S, E);

    // Deal with data.
    parseData( tracein,
               s, E, b,
               cache,
               verbose );

    // Print and write.
    printSummary(hit_cnt, miss_cnt, evic_cnt);
    fprintf(traceout, "%d %d %d", hit_cnt, miss_cnt, evic_cnt);

    // Free all memory.
    CacheFree(cache, S);
    fclose(tracein);
    fclose(traceout);

    return 0;
}
