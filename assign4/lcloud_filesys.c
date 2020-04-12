////////////////////////////////////////////////////////////////////////////////
//
//  File           : lcloud_filesys.c
//  Description    : This is the implementation of the Lion Cloud device 
//                   filesystem interfaces.
//
//   Author        : Hunter Schuler
//   Last Modified : 4/11/2020
//

// Include files
#include <stdlib.h>
#include <string.h>
#include <cmpsc311_log.h>

// Project include files
#include <lcloud_filesys.h>
#include <lcloud_controller.h>
#include <lcloud_cache.h>
#include <lcloud_network.h>

// Define max values
#define MAX_NAME_LENGTH 256 // Max allowed name length
#define MAX_DEVICES 16 // Define max allowed devices

// File system interface implementation
// Create a boolean class
typedef enum {false, true} bool;

// Create enums for registers
typedef enum {SEND_B0, RECIEVE} LC_B0;
typedef enum {SEND_B1, SUCCESS} LC_B1;
typedef LcOperationCode LC_C0;
// C1 is device ID
typedef enum{READ, WRITE} LC_C2;
// D0 is sector
// D1 is block

// Create a class for unpacked registers
typedef struct {
    LC_B0 B0;
    LC_B1 B1;
    LC_C0 C0;
    uint8_t C1;
    LC_C2 C2;
    uint16_t D0;
    uint16_t D1;
} LCUnpackedRegisters;

// Create a class for each device
typedef struct {
    bool active; // Determine if device is on
    uint8_t sectors; // Number of sectors on device
    uint8_t blocks; // Number of blocks per sector
    uint32_t totalBlocks; // Total number of blocks on device
} LCDevice;

// Create a class for a file type
struct LCFile {
    char path[MAX_NAME_LENGTH]; // File path
    LcFHandle handle; // File handle
    uint32_t pos; // Cursor position (in bytes)
    uint32_t size; // File size
    bool open; // Boolean to determine if the file is open
    uint16_t blocks[]; // Flexible array member of used blocks (Uses same math as fileCursor)
};

// Declare global variables
bool lc_on = false; // Lioncloud on or off
struct LCFile **files; // Create an array of files to keep track for every file object
LCDevice devices[MAX_DEVICES]; // Create an array of MAX_DEVICES items to keep track of devices
uint16_t maxFiles = 0;
uint16_t fileCursor = 0; // A cursor which tracks last block available to place file, assumes file is at least 1 block

////////////////////////////////////////////////////////////////////////////////
//
// Function     : generateHandle
// Description  : Function to generate a new unique file handle
//
// Inputs       : None
// Outputs      : 1 + the max handle found in the list
LcFHandle generateHandle(void) {
    LcFHandle maxHandle = 0; // All files will have handles of 1 or greater
    for (int i = 0; i < maxFiles; i++) {
        if (files[i]->handle > maxHandle) {
            maxHandle = files[i]->handle;
        }
    }
    return (++maxHandle);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : retrieveFileH
// Description  : Returns a file type found based on its file handle
//
// Inputs       : handle - The file handle provided
// Outputs      : A file; has a size of -1 if there was an error
struct LCFile *retrieveFileH(LcFHandle handle) {
    for (int i = 0; i < maxFiles; i++) {
        if (files[i]->handle == handle) {
            return files[i];
        }
    }
    struct LCFile *errFile = malloc(sizeof(struct LCFile));
    errFile->size = -1;
    return errFile;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : retrieveFileP
// Description  : Returns a file type found based on path
//
// Inputs       : *path - A pointer to a string array for the path
// Outputs      : A file; has a size of -1 if there was an error
struct LCFile *retrieveFileP(const char *path) {
    for (int i = 0; i < maxFiles; i++) {
        if (files[i]->path == path) {
            return files[i];
        }
    }
    struct LCFile *errFile = malloc(sizeof(struct LCFile));
    errFile->size = -1;
    return errFile;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : saveFile
// Description  : Function to save file to list
//
// Inputs       : file - The file to be saved
// Outputs      : 0 if successful (file found) or -1 if the file ouldn't be found
int saveFile(struct LCFile *file) {
    for (int i = 0; i < maxFiles; i++) {
        if (files[i]->handle == file->handle) {
            files[i] = file;
            return 0;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : findEmptyFile
// Description  : A function which loops through all existing and non-existing
//                files and closes them
//
// Inputs       : None
// Outputs      : None
void closeFiles(void) {
    for (int i = 0; i < maxFiles; i++) {
        files[i]->open = false;
        free(files[i]);
    }
    free(files);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : findEmptyFile
// Description  : A function to find the first empty file in the files list array
//
// Inputs       : None
// Outputs      : the index of the first empty file, or -1 if there is no more available
int findEmptyFile(void) {
    for (int i = 0; i < maxFiles; i++) {
        if (files[i]->handle < 0) {
            return i;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : calcDevice
// Description  : A function to calculate device from an absolute position
//
// Inputs       : absBlock - The absolute block
// Outputs      : Device number - Device which stores absBlock
uint8_t calcDevice(int absBlock) {
    for (int i = 0; i < MAX_DEVICES; i++) {
        absBlock -= devices[i].totalBlocks;
        if (absBlock < 0) {
            return i;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : calcSector
// Description  : A function to calculate sector relative to device from an absolute position
//
// Inputs       : absBlock - The absolute block
// Outputs      : Sector number - The sector on the device
uint16_t calcSector(int absBlock) {
    // Find calculate device, absblock becomes block on device
    for (int i = 0; i < MAX_DEVICES; i++) {
        absBlock -= devices[i].totalBlocks;
        if (absBlock < 0) {
            absBlock += devices[i].totalBlocks;
            // Calculate sector on device
            return absBlock / devices[i].blocks;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : calcBlock
// Description  : A function to calculate block relative to sector from an absolute position
//
// Inputs       : absBlock - The absolute block
// Outputs      : Block number - Block in sector on device
uint16_t calcBlock(int absBlock) {
    // Find calculate device, absblock becomes block on device
    for (int i = 0; i < MAX_DEVICES; i++) {
        absBlock -= devices[i].totalBlocks;
        if (absBlock < 0) {
            absBlock += devices[i].totalBlocks;
            // Calculate block on device
            return absBlock % devices[i].blocks;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : packRegisters
// Description  : A function to build a registerFrame for the IO Bus
//
// Inputs       : B0 - First 4 bits of register (0 for send, 1 for recieve)
//                B1 - Second 4 bits of regsiter (0 for send, 1 for success, other for failure)
//                C0 - First 8 bits of register (Operation code)
//                C1 - Second 8 bits of register (Device ID)
//                C2 - Third 8 bits of register (Determines Read or Write)
//                D0 - First 16 bits of register (Sector number - relative to device)
//                D1 - Second 16 bits of register (Block number - relative to sector)
// Outputs      : registers - An packed register set to send to the IO
LCloudRegisterFrame packRegisters(LC_B0 B0, LC_B1 B1, LC_C0 C0, uint8_t C1, LC_C2 C2, uint16_t D0, uint16_t D1) {
    uint64_t registers = 0;
    // Begin packing values
    registers |= (B0 & 0xF);
    registers <<= 4;
    registers |= (B1 & 0xF);
    registers <<= 8;
    registers |= (C0 & 0xFF);
    registers <<= 8;
    registers |= (C1 & 0xFF);
    registers <<= 8;
    registers |= (C2 & 0xFF);
    registers <<= 16;
    registers |= (D0 & 0xFFFF);
    registers <<= 16;
    registers |= (D1 & 0xFFFF);

    // Return packed registers
    return registers;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : unpackRegisters
// Description  : A function to unpack a response from the IO Bus
//
// Inputs       : registerFrame - The response frame from the bus
// Outputs      : registers - An unpacked version of the response with type LCUnpackedRegisters
LCUnpackedRegisters unpackRegisters(LCloudRegisterFrame registerFrame) {
    // Copy values from registerFrame in reverse order for easy evaluation
    LCUnpackedRegisters registers;
    registers.D1 = registerFrame & 0xFFFF;
    registerFrame >>= 16;
    registers.D0 = registerFrame & 0xFFFF;
    registerFrame >>= 16;
    registers.C2 = registerFrame & 0xFF;
    registerFrame >>= 8;
    registers.C1 = registerFrame & 0xFF;
    registerFrame >>= 8;
    registers.C0 = registerFrame & 0xFF;
    registerFrame >>= 8;
    registers.B1 = registerFrame & 0xF;
    registerFrame >>= 4;
    registers.B0 = registerFrame & 0xF;
    return registers;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : writeDataBlock
// Description  : A function to write a block of data to a device sector from the provided buffer
//
// Inputs       : dev - the integer value of the device to write to
//                sec - the integer value of the sector to write to (relative to the device)
//                block - the integer value of the block to write to (relative to the sector)
// Outputs      : 0 if successful, -1 if failure
int writeDataBlock(uint8_t dev, uint16_t sec, uint16_t block, char *buf) {
    LCUnpackedRegisters resp = unpackRegisters(client_lcloud_bus_request(packRegisters(SEND_B0, SEND_B1, LC_BLOCK_XFER, dev, LC_XFER_WRITE, sec, block), buf));
    // If they don't return success, throw an error
    if (resp.B0 != RECIEVE || resp.B1 != SUCCESS || resp.C0 != LC_BLOCK_XFER) {
        return -1;
    }
    // Put the newly written block in to the cache
    if (lcloud_putcache(dev, sec, block, buf) != 0) {
        return -1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : readDataBlock
// Description  : A function to read a block of data from a device sector to the provided buffer
//
// Inputs       : dev - the integer value of the device to read from
//                sec - the integer value of the sector to read from (relative to the device)
//                block - the integer value of the block to read from (relative to the sector)
//                buf - the character buffer the device will write to
//                writeToCache - a boolean to determine if the result should be written to the cache
// Outputs      : 0 if successful, -1 if failure
int readDataBlock(uint8_t dev, uint16_t sec, uint16_t block, char *buf, bool writeToCache) {
    char *tempBuf = lcloud_getcache(dev, sec, block);
    if (tempBuf == NULL) {
        LCUnpackedRegisters resp = unpackRegisters(client_lcloud_bus_request(packRegisters(SEND_B0, SEND_B1, LC_BLOCK_XFER, dev, LC_XFER_READ, sec, block), buf));
        // If they don't return success, throw an error
        if (resp.B0 != RECIEVE || resp.B1 != SUCCESS || resp.C0 != LC_BLOCK_XFER) {
            return -1;
        }
        if (writeToCache && lcloud_putcache(dev, sec, block, buf) != 0) {
            return -1;
        }
    } else {
        memcpy(buf, tempBuf, LC_DEVICE_BLOCK_SIZE);
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : selectNextDevice
// Description  : Function to select next available LC Device
//
// Inputs       : lastDevice - the last device available to write to
// Outputs      : next available lion cloud device if successful, -1 if failure
int selectNextDevice(int lastDevice) {
    for (int i = lastDevice; i < MAX_DEVICES; i++) {
        if (devices[i].active != false) {
            return i;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : powerOn
// Description  : Function to turn on LionCloud devices
//
// Inputs       : None
// Outputs      : 0 if successful, -1 if failure
int powerOn(void) {
    // Send a power on signal (0) to devices
    LCUnpackedRegisters resp = unpackRegisters(client_lcloud_bus_request(0, NULL));

    // If they don't return success, throw an error
    if (resp.B0 != RECIEVE || resp.B1 != SUCCESS || resp.C0 != LC_POWER_ON) {
        return -1;
    }

    // Probe for available devices
    resp = unpackRegisters(client_lcloud_bus_request(packRegisters(SEND_B0, SEND_B1, LC_DEVPROBE, 0, 0, 0, 0), NULL));

    // If they don't return success, throw an error
    if (resp.B0 != RECIEVE || resp.B1 != SUCCESS || resp.C0 != LC_DEVPROBE || resp.D0 == 0) {
        return -1;
    }

    // Decode available devices
    for (int i = 0; i < MAX_DEVICES; i++) {
        if ((resp.D0 & 1) > 0) {
            // Initialize the available device
            LCUnpackedRegisters respInit = unpackRegisters(client_lcloud_bus_request(packRegisters(SEND_B0, SEND_B1, LC_DEVINIT, i, 0, 0, 0), NULL));

            // If they don't return success, throw an error
            if (respInit.B0 != RECIEVE || respInit.B1 != SUCCESS || respInit.C0 != LC_DEVINIT || respInit.D0 == 0) {
                return -1;
            }

            // Define device
            devices[i].active = true;
            devices[i].sectors = respInit.D0;
            devices[i].blocks = respInit.D1;
            devices[i].totalBlocks = respInit.D0 * respInit.D1;

            // Increment max files
            maxFiles += devices[i].totalBlocks;
        } else {
            devices[i].active = false;
        }
        resp.D0 >>= 1;
    }

    files = malloc(maxFiles*(sizeof(struct LCFile) + sizeof(uint16_t)*maxFiles));
    for (int i = 0; i < maxFiles; i++) {
        struct LCFile *file = malloc(sizeof(struct LCFile) + sizeof(uint16_t)*maxFiles);
        // Create a simple dummy file for each of the possible files
        file->handle = -1;
        file->size = 0;
        file->open = false;
        files[i] = file;
        // This loop shouldn't be necessary but for some reason it is
        if (i == 0) {
            for (int j = 0; j < maxFiles; j++) {
                files[i]->blocks[j] = 0;
            }
        }
    }
    fileCursor = 0;

    // Initialize lcloud cache
    if (lcloud_initcache(LC_CACHE_MAXBLOCKS) != 0) {
        return -1;
    }

    // Mark devices as turned on
    lc_on = true;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcopen
// Description  : Open the file for for reading and writing
//
// Inputs       : path - the path/filename of the file to be read
// Outputs      : file handle if successful test, -1 if failure
LcFHandle lcopen(const char *path) {
    // If devices haven't been turned on, initialize them
    if (!lc_on) {
        if (powerOn() == -1) {
            return -1;
        }
    }

    // Check if file exists
    struct LCFile *file = retrieveFileP(path);

    // If not, create it
    int handle = generateHandle();
    if (file->size == -1) {
        // Free the temporary storage used in the retrieve function, since it doesn't account for max array size
        free(file);
        int index = findEmptyFile();
        for (int i = 0; i < strlen(path); i++) {
            files[index]->path[i] = path[i];
        }
        files[index]->handle = handle;
        files[index]->pos = 0;
        files[index]->size = 0;
        files[index]->open = true;
        files[index]->blocks[0] = fileCursor++;
    }
    return(handle);
} 

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcread
// Description  : Read data from the file 
//
// Inputs       : fh - file handle for the file to read from
//                buf - place to put the data
//                len - the length of the read
// Outputs      : number of bytes read, -1 if failure
int lcread(LcFHandle fh, char *buf, size_t len) {
    // If devices haven't been turned on, initialize them
    if (!lc_on) {
        if (powerOn() == -1) {
            return -1;
        }
    }

    // Check to make sure operation is valid size
    if (len > LC_MAX_OPERATION_SIZE) {
        return -1;
    }

    // Retrieve file to read and test if it exists and is open
    struct LCFile *readFile = retrieveFileH(fh);
    if (readFile->open == false || readFile->size == -1) {
        free(readFile);
        return -1;
    }

    // Check if file pointer is at the end of the file
    if (readFile->pos == readFile->size) {
        return len;
    }

    // Provide buffer to read from pos to end of block (or len)
    char readBuffer[LC_DEVICE_BLOCK_SIZE];

    // Retrieve the current block data and calculate the offset with pos
    // block index = pos / LC_DEVICE_BLOCK_SIZE
    // calculate device, sector, and block using block pointer math
    uint16_t blockIndex = readFile->pos / LC_DEVICE_BLOCK_SIZE;
    uint16_t readBlock = calcBlock(readFile->blocks[blockIndex]);
    uint16_t readSector = calcSector(readFile->blocks[blockIndex]);
    uint8_t readDevice = calcDevice(readFile->blocks[blockIndex]);
    if (readBlock == (uint16_t)-1 || readSector == (uint16_t)-1 || readDevice == (uint8_t)-1) {
        return -1;
    }

    // offset index from returned buffer = pos % LC_DEVICE_BLOCK_SIZE
    uint8_t offsetByteFromBlock = readFile->pos % LC_DEVICE_BLOCK_SIZE;

    // Check if offset+len is still within a block
    if (offsetByteFromBlock+len <= LC_DEVICE_BLOCK_SIZE) {
        if (readDataBlock(readDevice, readSector, readBlock, readBuffer, true) != 0) {
            return -1;
        }
        // Check if this will read to the end of the file
        if (readFile->pos+len > readFile->size) {
            // Read the remainder of the file
            memcpy(&buf[0], &readBuffer[offsetByteFromBlock], readFile->size-readFile->pos);
            // Set the cursor to the end of the file
            readFile->pos = readFile->size;
            // Save the file and return the length
            if (saveFile(readFile) != 0) {
                return -1;
            }
            return len;
        }
        memcpy(&buf[0], &readBuffer[offsetByteFromBlock], len);
    } else {
        uint16_t bufOffset = 0;
        // Determine if length ends on the current block
        while (bufOffset+(LC_DEVICE_BLOCK_SIZE-offsetByteFromBlock) < len) {
            if (readDataBlock(readDevice, readSector, readBlock, readBuffer, true) != 0) {
                return -1;
            }

            // Reading the last block of the data file
            if (readFile->blocks[blockIndex+1] == 0) {
                // Read the remainder of the file
                if (len - bufOffset > (readFile->size % LC_DEVICE_BLOCK_SIZE)) {
                    // If it is greater than the remainder of the file, read to the end
                    memcpy(&buf[bufOffset], &readBuffer[offsetByteFromBlock], readFile->size % LC_DEVICE_BLOCK_SIZE);
                    // Set the cursor to the end of the file
                    readFile->pos = readFile->size;
                } else {
                    // Doesn't read to the end of the file
                    memcpy(&buf[bufOffset], &readBuffer[offsetByteFromBlock], len - bufOffset);
                    // Set the cursor to the end of the read
                    readFile->pos += len;
                }
                // Save the file and return the length
                if (saveFile(readFile) != 0) {
                    return -1;
                }
                return len;
            }

            memcpy(&buf[bufOffset], &readBuffer[offsetByteFromBlock], LC_DEVICE_BLOCK_SIZE-offsetByteFromBlock);

            // Increase the offset from the original buffer by one block
            bufOffset += (LC_DEVICE_BLOCK_SIZE - offsetByteFromBlock);
            offsetByteFromBlock = 0;

            // Use logic to increment block and sector to next available block
            readBlock = calcBlock(readFile->blocks[++blockIndex]);
            readSector = calcSector(readFile->blocks[blockIndex]);
            readDevice = calcDevice(readFile->blocks[blockIndex]);
            if (readBlock == (uint16_t)-1 || readSector == (uint16_t)-1 || readDevice == (uint8_t)-1) {
                return -1;
            }
        }
        // Read the rest of the data
        if (readDataBlock(readDevice, readSector, readBlock, readBuffer, true) != 0) {
            return -1;
        }
        memcpy(&buf[bufOffset], &readBuffer[offsetByteFromBlock], len-bufOffset);
    }


    // Save file back to list
    readFile->pos+=len;
    if (saveFile(readFile) != 0){
        return -1;
    }
    return len;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcwrite
// Description  : write data to the file
//
// Inputs       : fh - file handle for the file to write to
//                buf - pointer to data to write
//                len - the length of the write
// Outputs      : number of bytes written if successful test, -1 if failure
int lcwrite(LcFHandle fh, char *buf, size_t len) {
    // If devices haven't been turned on, initialize them
    if (!lc_on) {
        if (powerOn() == -1) {
            return -1;
        }
    }

    // Check to make sure operation is valid size
    if (len > LC_MAX_OPERATION_SIZE) {
        return -1;
    }

    // Retrieve file to write and test if it exists and is open
    struct LCFile *writeFile = retrieveFileH(fh);
    if (writeFile->open == false || writeFile->size == -1) {
        free(writeFile);
        return -1;
    }

    // Provide buffer to read from beginning of block to end of file
    char writeBuffer[LC_DEVICE_BLOCK_SIZE];

    // Retrieve the current block data and calculate the offset with pos
    // calculate device, sector, and block using block pointer math
    uint16_t blockIndex = writeFile->pos / LC_DEVICE_BLOCK_SIZE;
    uint16_t writeBlock = calcBlock(writeFile->blocks[blockIndex]);
    uint16_t writeSector = calcSector(writeFile->blocks[blockIndex]);
    uint8_t writeDevice = calcDevice(writeFile->blocks[blockIndex]);
    if (writeBlock == (uint16_t)-1 || writeSector == (uint16_t)-1 || writeDevice == (uint8_t)-1) {
        return -1;
    }

    // offset index from returned buffer = pos % LC_DEVICE_BLOCK_SIZE
    uint8_t offsetByteFromBlock = writeFile->pos % LC_DEVICE_BLOCK_SIZE;

    // Read the first block to write to
    if (readDataBlock(writeDevice, writeSector, writeBlock, writeBuffer, false) != 0) {
        return -1;
    }

    // If it's greater than a block size, we'll need to write to the next block
    if (len+offsetByteFromBlock <= LC_DEVICE_BLOCK_SIZE) {
        memcpy(&writeBuffer[offsetByteFromBlock], &buf[0], len);
        if (writeDataBlock(writeDevice, writeSector, writeBlock, writeBuffer) != 0) {
            return -1;
        }
    } else {
        uint16_t bufOffset = 0;
        // Determine if length ends on the current block
        while (bufOffset+(LC_DEVICE_BLOCK_SIZE-offsetByteFromBlock) < len) {
            memcpy(&writeBuffer[offsetByteFromBlock], &buf[bufOffset], LC_DEVICE_BLOCK_SIZE-offsetByteFromBlock);
            // Write block to device and update cache
            if (writeDataBlock(writeDevice, writeSector, writeBlock, writeBuffer) != 0) {
                return -1;
            }

            // Increase the offset from the original buffer by one block
            bufOffset += (LC_DEVICE_BLOCK_SIZE - offsetByteFromBlock);
            offsetByteFromBlock = 0;

            // Use logic to increment block and sector to next available block
            if (!writeFile->blocks[++blockIndex]) {
                writeBlock = calcBlock(fileCursor);
                writeSector = calcSector(fileCursor);
                writeDevice = calcDevice(fileCursor);
                writeFile->blocks[blockIndex] = fileCursor++;
            } else {
                writeBlock = calcBlock(writeFile->blocks[blockIndex]);
                writeSector = calcSector(writeFile->blocks[blockIndex]);
                writeDevice = calcDevice(writeFile->blocks[blockIndex]);
            }
            if (writeBlock == (uint16_t)-1 || writeSector == (uint16_t)-1 || writeDevice == (uint8_t)-1) {
                return -1;
            }
            if (devices[writeDevice].active == false) {
                writeDevice = selectNextDevice(writeDevice);
                // No more devices to write to
                if (writeDevice == -1) {
                    return -1;
                }
                // Adjust pointer to beginning of device
                fileCursor = 0;
                for (int i = 0; i < writeDevice; i++) {
                    fileCursor += devices[i].totalBlocks;
                }
            }

            // Reset writeBuffer
            if (readDataBlock(writeDevice, writeSector, writeBlock, writeBuffer, false) != 0) {
                return -1;
            }
        }
        // Write the remainder of the data to the last block, update cache
        memcpy(&writeBuffer[offsetByteFromBlock], &buf[bufOffset], len-bufOffset);
        if (writeDataBlock(writeDevice, writeSector, writeBlock, writeBuffer) != 0) {
            return -1;
        }
    }

    // Increment file size and pos
    writeFile->pos += len;
    if (writeFile->pos > writeFile->size) {
        writeFile->size = writeFile->pos;
    }

    // If the block index increases
    if ((writeFile->pos / LC_DEVICE_BLOCK_SIZE) > blockIndex && writeFile->blocks[writeFile->pos / LC_DEVICE_BLOCK_SIZE] == 0) {
        writeFile->blocks[writeFile->pos / LC_DEVICE_BLOCK_SIZE] = fileCursor++;
    }

    if (saveFile(writeFile) != 0) {
        return -1;
    }
    return(len);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcseek
// Description  : Seek to a specific place in the file
//
// Inputs       : fh - the file handle of the file to seek in
//                off - offset within the file to seek to
// Outputs      : 0 if successful test, -1 if failure
int lcseek(LcFHandle fh, size_t off) {
    // If devices haven't been turned on, initialize them
    if (!lc_on) {
        if (powerOn() == -1) {
            return -1;
        }
    }

    // Retrieve file to write and test if it exists and is open
    struct LCFile *file = retrieveFileH(fh);
    if (file->open == false || file->size == -1 || off > file->size) {
        free(file);
        return -1;
    }

    file->pos = off;
    if (saveFile(file) != 0) {
        return -1;
    }
    return off;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcclose
// Description  : Close the file
//
// Inputs       : fh - the file handle of the file to close
// Outputs      : 0 if successful test, -1 if failure
int lcclose(LcFHandle fh) {
    if (!lc_on) {
        if (powerOn() == -1) {
            return -1;
        }
    }

    // Retrieve file and test if it is open
    struct LCFile *closeFile = retrieveFileH(fh);
    if (closeFile->open == false || closeFile->size == -1) {
        free(closeFile);
        return -1;
    }

    // Close the file
    closeFile->open = false;

    // If for some reason, the file doesn't save, return an error
    if (saveFile(closeFile) != 0) {
        return -1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : lcshutdown
// Description  : Shut down the filesystem
//
// Inputs       : none
// Outputs      : 0 if successful test, -1 if failure
int lcshutdown(void) {
    // If devices are on, turn them off
    if (lc_on) {
        // Close all open files
        closeFiles();

        // Turn off devices
        LCUnpackedRegisters resp = unpackRegisters(client_lcloud_bus_request(packRegisters(SEND_B0, SEND_B1, LC_POWER_OFF, 0, 0, 0, 0), NULL));

        // Report any error
        if (resp.B1 != SUCCESS || resp.B0 != RECIEVE || resp.C0 != LC_POWER_OFF) {
            return -1;
        }

        // Set devices to off
        lc_on = false;

        // Dump cache
        if (lcloud_closecache() != 0) {
            return -1;
        }
    }

    return 0;
}