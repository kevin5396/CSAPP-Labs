/*
 * studentID:   5140219295
 * studentName: 凌康伟
 */
#include "cachelab.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************************
 *
 * structs for cache simulation
 *
 **********************************/
typedef unsigned long long Address;
typedef struct _cache_line
{
    Address tag;
    struct _cache_line *prev;
    struct _cache_line *next;
}CacheLine, *PCacheLine;

typedef struct _cache
{
    int n_sets;
    int associativity;
    int index_mask;
    int index_offset;
    int tag_offset;
    PCacheLine *set_head;
    PCacheLine *set_tail;
    int* n_entries;
}Cache, *PCache;

typedef struct _logger {
    int verbose;
    int hits;
    int misses;
    int evictions;
}Logger;

typedef struct _log {
    int hit;
    int miss;
    int evic;
}Log;

/*********************************
 *
 * cacheline_lru: simulate LRU process, choose the least recently used (at the end of the list), change its tag to the newly loaded(eviction),
 *                then insert into the head.
 * cacheline_new: insert a new line at the start of the set list.
 * cacheline_visit: under hit circumstance, update the newly visited cache line (move it to the start of the set list).
 *
 **********************************/
void cacheline_lru(Address tag, PCacheLine head, PCacheLine tail);
void cacheline_new(Address tag, PCacheLine head);
void cacheline_visit(PCacheLine line, PCacheLine head);

/*********************************
 *
 * Simulation Functions Declarations
 *
 **********************************/
void usage(void);
PCache cache_init(int n_idx, int n_blk, int n_assoc);
void cache_sim(PCache pcache, Address address, Log* log);
void simulate(char *const filename, PCache pcache, Logger* logger);
void cache_deinit(PCache pcache);

// visual status
void printStatus(PCache pcache);


int main(int argc, char *argv[])
{

    char* opts = "hvs:E:b:t:";
    int opt, n_idx=0, n_blk=0, n_assoc=0, vbs_flag = 0;
    char* filename;
    while ((opt = getopt(argc, argv, opts)) != -1) {
        switch (opt) {
        case 'h':
            usage();
            exit(0);
        case 'v':
            vbs_flag = 1;
            break;
        case 's':
            n_idx = atoi(optarg);
            break;
        case 'E':
            n_assoc = atoi(optarg);
            break;
        case 'b':
            n_blk = atoi(optarg);
            break;
        case 't':
            filename = optarg;
        default:
            break;
        }
    }
    if (n_idx == 0 || n_blk == 0 || n_assoc == 0) {
        usage();
        exit(0);
    }

    PCache pcache = cache_init(n_idx, n_blk, n_assoc);
    Logger logger = {vbs_flag, 0, 0, 0};
    simulate(filename, pcache, &logger);

    printSummary(logger.hits, logger.misses, logger.evictions);
    cache_deinit(pcache);
    return 0;
}

void usage(void)
{
    printf("Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
           "Options:\n"
           "  -h     Print this help message.\n"
           "  -v         Optional verbose flag.\n"
           "  -s <num>   Number of set index bits.\n"
           "  -E <num>   Number of lines per set.\n"
           "  -b <num>   Number of block offset bits.\n"
           "  -t <file>  Trace file.\n"
           "Examples:\n"
           "  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n"
           "  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

PCache cache_init(int n_idx, int n_blk, int n_assoc)
{
    PCache pcache = (PCache)malloc(sizeof(Cache));
    pcache->associativity = n_assoc;
    pcache->index_offset = n_blk;
    pcache->index_mask = ((1 << n_idx) - 1) << n_blk;
    pcache->n_sets = 1 << n_idx;
    pcache->tag_offset = n_blk + n_idx;

    pcache->set_head = (PCacheLine*)malloc(sizeof(PCacheLine) * pcache->n_sets);
    pcache->set_tail = (PCacheLine*)malloc(sizeof(PCacheLine) * pcache->n_sets);
    pcache->n_entries = (int*)malloc(sizeof(int) * pcache->n_sets);
    memset(pcache->n_entries, 0, pcache->n_sets);

    for (int i = 0; i < pcache->n_sets; ++i) {
        pcache->set_head[i] = (PCacheLine)malloc(sizeof(CacheLine));
        pcache->set_tail[i] = (PCacheLine)malloc(sizeof(CacheLine));

        pcache->set_head[i]->next = pcache->set_tail[i];
        pcache->set_tail[i]->prev = pcache->set_head[i];
    }
    return pcache;
}
void cache_deinit(PCache pcache)
{
    PCacheLine head, cur, tail, tmp;
    for (int i = 0; i < pcache->n_sets; ++i) {
        head = pcache->set_head[i];
        tail = pcache->set_tail[i];
        for (cur = head->next; cur != tail;) {
            tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        free(head);
        free(tail);
    }
    free(pcache->set_head);
    free(pcache->set_tail);
    free(pcache->n_entries);

    free(pcache);
}

void simulate(char *const filename, PCache pcache, Logger *pLogger)
{
    FILE* trace = fopen(filename, "r");
    if (!trace) {
        printf("trace %s opening failed\n", filename);
        exit(1);
    }

    char ins;
    Address addr;
    unsigned size;
    Log log = {0, 0, 0};
    while (fscanf(trace, " %c %llx, %u", &ins, &addr, &size) == 3) {
        log.hit = 0;
        log.miss = 0;
        log.evic = 0;
        if (ins == 'I') continue;

        cache_sim(pcache, addr, &log);
        switch (ins) {
        case 'L':
            break;
        case 'S':
            break;
        case 'M':
            log.hit++; // Modify operation always hits at the end.
            break;
        default:
            break;
        }
        pLogger->hits += log.hit;
        pLogger->misses += log.miss;
        pLogger->evictions += log.evic;
        if (pLogger->verbose) {
            printf("%c %llx,%u ", ins, addr, size);
            if (log.miss)  printf("miss ");
            if (log.evic)  printf("eviction ");
            if (log.hit>1)   printf("hit ");
            if (log.hit) printf("hit");
            printf("\n");
        }
        //printStatus(pcache);
    }
    fclose(trace);
}

void cache_sim(PCache pcache, Address addr, Log* log)
{
    unsigned idx = (addr & pcache->index_mask) >> pcache->index_offset;
    PCacheLine head = pcache->set_head[idx];
    PCacheLine tail = pcache->set_tail[idx];
    int count = pcache->n_entries[idx], found = 0;
    unsigned long long tag = addr >> (pcache->tag_offset);
    PCacheLine current = head;
    // search in cache set
    for (int i = 0; i < count && current != tail; ++i) {
        current = current->next;
        if (current->tag == tag) {
            found = 1;
            break;
        }
    }
    if (found) {
        log->hit = 1;
        cacheline_visit(current, head);
    } else {
        log->miss = 1;
        if (count < pcache->associativity) {
            cacheline_new(tag, head);
            pcache->n_entries[idx]++;
        } else {
            cacheline_lru(tag, head, tail);
            log->evic = 1;
        }
    }
}

void cacheline_lru(Address tag, PCacheLine head, PCacheLine tail)
{
    PCacheLine reuse_line = tail->prev;

    // remove line from the end of the list
    tail->prev = reuse_line->prev;
    reuse_line->prev->next = tail;

    // insert into the start of the list
    PCacheLine next = head->next;
    reuse_line->next = next;
    next->prev = reuse_line;
    head->next = reuse_line;
    reuse_line->prev = head;

    // set tag for the newly loaded line
    reuse_line->tag = tag;
}
void cacheline_new(Address tag, PCacheLine head)
{
    PCacheLine newline = (PCacheLine)malloc(sizeof(CacheLine));
    newline->tag = tag;

    head->next->prev = newline;
    newline->next = head->next;

    head->next = newline;
    newline->prev = head;
}

void cacheline_visit(PCacheLine line, PCacheLine head)
{
    // remove line from current pos
    line->prev->next = line->next;
    line->next->prev = line->prev;

    // insert into the start pos
    line->next = head->next;
    line->prev = head;
    head->next->prev = line;
    head->next = line;
}

void printStatus(PCache pcache)
{
    printf("\n");
    for (int i = 0; i < pcache->n_sets; ++i) {
        printf("set %d: ", i);
        PCacheLine line = pcache->set_head[i];
        for (int j = 0; j < pcache->n_entries[i]; ++j) {
            line = line->next;
            printf("%llx -> ", line->tag);
        }
        printf("\n");
    }
}
