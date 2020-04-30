////////////////////////////////////////////////////////////////////////////////
//
//  File           : lcloud_cache.c
//  Description    : This is the cache implementation for the LionCloud
//                   assignment for CMPSC311.
//
//   Author        : Hunter Schuler
//   Last Modified : 4/30/2020
//

// Includes 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmpsc311_log.h>
#include <lcloud_cache.h>

//
// Global variables
uint32_t cacheBlocks = 0;
uint32_t cacheHits = 0;
uint32_t cacheMisses = 0;
uint32_t oldestBlock = 0;
struct cacheBlock **cache;
struct cacheBlock{
    LcDeviceId deviceId;
    uint16_t sectorId;
    uint16_t blockId;
    char data[LC_DEVICE_BLOCK_SIZE];
};

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcloud_getcache
// Description  : Search the cache for a block 
//
// Inputs       : did - device number of block to find
//                sec - sector number of block to find
//                blk - block number of block to find
// Outputs      : cache block if found (pointer), NULL if not or failure
char *lcloud_getcache(LcDeviceId did, uint16_t sec, uint16_t blk) {
    for (int i = 0; i < cacheBlocks; i++) {
        // Enables early truncation in linear search
        if (cache[i]->deviceId == (LcDeviceId)-1) {
            cacheMisses += 1;
            return(NULL);
        }
        // File checks
        // Utilize short circuit comparison for more early truncation
        if (cache[i]->deviceId == did && cache[i]->sectorId == sec && cache[i]->blockId == blk) {
            cacheHits += 1;
            return(cache[i]->data);
        }
    }
    cacheMisses += 1;
    return(NULL);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcloud_putcache
// Description  : Put a value in the cache 
//
// Inputs       : did - device number of block to insert
//                sec - sector number of block to insert
//                blk - block number of block to insert
// Outputs      : 0 if succesfully inserted, -1 if failure
int lcloud_putcache(LcDeviceId did, uint16_t sec, uint16_t blk, char *block) {
    // Check if the block exists in the cache
    for (int i = 0; i < cacheBlocks; i++) {
        // Enables early truncation in linear search
        if (cache[i]->deviceId == (LcDeviceId)-1) {
            break;
        }
        // File checks
        // Utilize short circuit comparison for more early truncation
        if (cache[i]->deviceId == did && cache[i]->sectorId == sec && cache[i]->blockId == blk) {
            memcpy(cache[i]->data, block, LC_DEVICE_BLOCK_SIZE);
            return(0);
        }
    }
    // If the file doesn't exist, update the oldest
    cache[oldestBlock]->deviceId = did;
    cache[oldestBlock]->sectorId = sec;
    cache[oldestBlock]->blockId = blk;
    memcpy(cache[oldestBlock++]->data, block, LC_DEVICE_BLOCK_SIZE);
    oldestBlock %= cacheBlocks;
    return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcloud_initcache
// Description  : Initialze the cache by setting up metadata a cache elements.
//
// Inputs       : maxblocks - the max number number of blocks 
// Outputs      : 0 if successful, -1 if failure
int lcloud_initcache(int maxBlocks) {
    cacheBlocks = maxBlocks;
    cache = malloc(cacheBlocks * (sizeof(struct cacheBlock)));
    for (int i = 0; i < cacheBlocks; i++){
        struct cacheBlock *block = malloc(sizeof(struct cacheBlock));
        block->deviceId = -1;
        cache[i] = block;
    }
    return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcloud_closecache
// Description  : Clean up the cache when program is closing
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure
int lcloud_closecache(void) {
    logMessage(LOG_INFO_LEVEL, "Cache Hits: %i", cacheHits);
    logMessage(LOG_INFO_LEVEL, "Cache Misses: %i", cacheMisses);
    logMessage(LOG_INFO_LEVEL, "Cache Hit Ratio: %f", (float)cacheHits/(cacheHits+cacheMisses));
    free(cache);
    return(0);
}