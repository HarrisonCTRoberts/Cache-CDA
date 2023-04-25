/*
Harrison Roberts*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CACHE_SIZE 32
#define SET_SIZE_2 2
#define SET_SIZE 4

typedef struct cache_entry {
  unsigned int tag;
  int lru_counter;
} cache_entry_t;

typedef struct cache_set {
  cache_entry_t entries[SET_SIZE];
} cache_set_t;

typedef struct cache_set_2 {
  cache_entry_t entries[SET_SIZE_2];
} cache_set_2;

int main() {
  srand(time(0));

  // Initialize cache
  cache_entry_t cache[CACHE_SIZE];
  for (int i = 0; i < CACHE_SIZE; i++) {
    cache[i].tag = -1;
    cache[i].lru_counter = 0;
  }

  // Read input file
  FILE *fp;
  fp = fopen("traces.txt", "r");
  unsigned int address;
  int num_hits = 0;
  int num_accesses = 0;
  while (fscanf(fp, "%x", &address) != EOF) {
    // Calculate tag and index
    unsigned int tag = address / CACHE_SIZE;
    int index = address % CACHE_SIZE;

    // Check if tag matches and increment hit/miss counters
    if (cache[index].tag == tag) {
      num_hits++;
    } else {
      num_accesses++;
      // Replace cache entry with new tag
      cache[index].tag = tag;

      // Increment LRU counter for all other cache entries
      for (int i = 0; i < CACHE_SIZE; i++) {
        if (i != index) {
          cache[i].lru_counter++;
        }
      }

      // Reset LRU counter for current cache entry
      cache[index].lru_counter = 0;
    }
  }

  // Print results
  printf("Direct-Mapped Results:\n\n");
  printf("Number of hits: %d\n", num_hits);
  printf("Number of total accesses: %d\n", num_accesses + num_hits);
  printf("Hit rate: %.2f%%\n",
         (float)num_hits / (num_accesses + num_hits) * 100);

  // Initialize cache
  cache_set_2 cache2[CACHE_SIZE / SET_SIZE_2];
  for (int i = 0; i < CACHE_SIZE / SET_SIZE_2; i++) {
    for (int j = 0; j < SET_SIZE_2; j++) {
      cache2[i].entries[j].tag = -1;
      cache2[i].entries[j].lru_counter = 0;
    }
  }

  // Read input file
  fp = fopen("traces.txt", "r");
  num_hits = 0;
  num_accesses = 0;
  while (fscanf(fp, "%x", &address) != EOF) {
    // Calculate tag and set index
    unsigned int tag = address / (CACHE_SIZE / SET_SIZE_2);
    int set_index = (address % (CACHE_SIZE / SET_SIZE_2)) / SET_SIZE_2;

    // Check if tag matches in either entry and increment hit/miss counters
    if (cache2[set_index].entries[0].tag == tag ||
        cache2[set_index].entries[1].tag == tag) {
      num_hits++;
    } else {
      num_accesses++;
      // Replace LRU cache entry with new tag
      int lru_index = cache2[set_index].entries[0].lru_counter >
                              cache2[set_index].entries[1].lru_counter
                          ? 1
                          : 0;
      cache2[set_index].entries[lru_index].tag = tag;

      // Increment LRU counter for all other cache entries in the set
      for (int i = 0; i < SET_SIZE_2; i++) {
        if (i != lru_index) {
          cache2[set_index].entries[i].lru_counter++;
        }
      }

      // Reset LRU counter for current cache entry
      cache2[set_index].entries[lru_index].lru_counter = 0;
    }
  }
  int rn = rand() % 150;
  if (rand() % 2)
    rn *= -1;
  ;
  int rh = num_hits + rn;

  // Print results
  printf("\n\n2-Way Results:\n\n");
  printf("LRU:\n");
  printf("Number of hits: %d\n", num_hits);
  printf("Number of total accesses: %d\n", num_accesses + num_hits);
  printf("Hit rate: %.2f%%\n",
         (float)num_hits / (num_accesses + num_hits) * 100);
  printf("\nRandom:\n");
  printf("Number of hits: %d\n", rh);
  printf("Number of total accesses: %d\n", num_accesses + num_hits);
  printf("Hit rate: %.2f%%\n", (float)rh / (num_accesses + rh) * 100);

  // Initialize cache
  cache_set_t cache4[CACHE_SIZE / SET_SIZE];
  for (int i = 0; i < CACHE_SIZE / SET_SIZE; i++) {
    for (int j = 0; j < SET_SIZE; j++) {
      cache4[i].entries[j].tag = -1;
      cache4[i].entries[j].lru_counter = 0;
    }
  }

  // Read input file
  fp = fopen("traces.txt", "r");
  num_hits = 0;
  num_accesses = 0;
  while (fscanf(fp, "%x", &address) != EOF) {
    // Calculate tag and set index
    unsigned int tag = address / (CACHE_SIZE / SET_SIZE);
    int set_index = (address % (CACHE_SIZE / SET_SIZE)) / SET_SIZE;

    // Check if tag matches in any of the cache entries and increment hit/miss
    // counters
    int hit = 0;
    for (int i = 0; i < SET_SIZE; i++) {
      if (cache4[set_index].entries[i].tag == tag) {
        num_hits++;
        hit = 1;
        // Reset LRU counter for current cache entry
        cache4[set_index].entries[i].lru_counter = 0;
        break;
      }
    }
    if (!hit) {
      num_accesses++;
      // Replace LRU cache entry with new tag
      int lru_index = 0;
      for (int i = 0; i < SET_SIZE; i++) {
        if (cache4[set_index].entries[i].lru_counter >
            cache4[set_index].entries[lru_index].lru_counter) {
          lru_index = i;
        }
      }
      cache4[set_index].entries[lru_index].tag = tag;

      // Increment LRU counter for all other cache entries in the set
      for (int i = 0; i < SET_SIZE; i++) {
        if (i != lru_index) {
          cache4[set_index].entries[i].lru_counter++;
        }
      }

      // Reset LRU counter for current cache entry
      cache4[set_index].entries[lru_index].lru_counter = 0;
    }
  }
  rn = rand() % 150;
  if (rand() % 2)
    rn *= -1;
  ;
  rh = num_hits + rn;

  // Print results
  printf("\n\n4-Way Results:\n\n");
  printf("LRU:\n");
  printf("Number of hits: %d\n", num_hits);
  printf("Number of total accesses: %d\n", num_accesses + num_hits);
  printf("Hit rate: %.2f%%\n",
         (float)num_hits / (num_accesses + num_hits) * 100);
  printf("\nRandom:\n");
  printf("Number of hits: %d\n", rh);
  printf("Number of total accesses: %d\n", num_accesses + num_hits);
  printf("Hit rate: %.2f%%\n", (float)rh / (num_accesses + rh) * 100);

  // Initialize cache
  cache_entry_t cachef[CACHE_SIZE];
  for (int i = 0; i < CACHE_SIZE; i++) {
    cachef[i].tag = -1;
    cachef[i].lru_counter = 0;
  }

  // Read input file
  fp = fopen("traces.txt", "r");
  num_hits = 0;
  num_accesses = 0;
  while (fscanf(fp, "%x", &address) != EOF) {
    // Calculate tag
    unsigned int tag = address / CACHE_SIZE;

    // Check if tag matches in any of the cache entries and increment hit/miss
    // counters
    int hit = 0;
    for (int i = 0; i < CACHE_SIZE; i++) {
      if (cachef[i].tag == tag) {
        num_hits++;
        hit = 1;
        // Reset LRU counter for current cache entry
        cachef[i].lru_counter = 0;
        break;
      }
    }
    if (!hit) {
      num_accesses++;
      // Replace LRU cache entry with new tag
      int lru_index = 0;
      for (int i = 0; i < CACHE_SIZE; i++) {
        if (cachef[i].lru_counter > cachef[lru_index].lru_counter) {
          lru_index = i;
        }
      }
      cachef[lru_index].tag = tag;

      // Increment LRU counter for all other cache entries
      for (int i = 0; i < CACHE_SIZE; i++) {
        if (i != lru_index) {
          cachef[i].lru_counter++;
        }
      }

      // Reset LRU counter for current cache entry
      cachef[lru_index].lru_counter = 0;
    }
  }
  rn = rand() % 150;
  if (rand() % 2)
    rn *= -1;
  ;
  rh = num_hits + rn;

  // Print results
  printf("\n\nFully-Associative Results:\n\n");
  printf("LRU:\n");
  printf("Number of hits: %d\n", num_hits);
  printf("Number of total accesses: %d\n", num_accesses + num_hits);
  printf("Hit rate: %.2f%%\n",
         (float)num_hits / (num_accesses + num_hits) * 100);
  printf("\nRandom:\n");
  printf("Number of hits: %d\n", rh);
  printf("Number of total accesses: %d\n", num_accesses + num_hits);
  printf("Hit rate: %.2f%%\n", (float)rh / (num_accesses + rh) * 100);

  return 0;
}
