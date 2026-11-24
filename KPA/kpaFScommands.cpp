#include "kpaFScommands.h"
#include <ctime>
#include <queue>
#include <cstring>

//******************************************************************************

// Author: Simeon K
//******************************************************************************

// Author: Simeon K
// requires node.gen to have been initialized or restored.
void setCreationMetadata(iNode &node, fileType type, uint16_t ownerUID, uint16_t ownerGID, uint16_t permissions) {
    time_t now = time(NULL); // Get the current time.
    // Set the iNode's type, owner UID and GID, and permissions.
    node.type = type;
    node.uid = ownerUID;
    node.gid = ownerGID;
    node.perms = permissions;

    // Directories start with two links and files start with one link.
    if (type == DIR) {
        node.nLinks = 2;
    } else {
        node.nLinks = 1;
    }

    // A newly created file has no data, so its size is zero.
    node.size = 0;
    node.mTime = now;
    node.aTime = now;
    node.bTime = now;
    node.cTime = now;

    // increment the generation count.
    node.gen++;
}

//******************************************************************************

// Author: Simeon K
string formatPermissions(uint16_t perms) {
    string result = "---------";

    // Owner permissions
    if ((perms & 0400) != 0) result[0] = 'r';
    if ((perms & 0200) != 0) result [1] = 'w';
    if ((perms & 0100) != 0) result[2] = 'x';

    // Group permissions.
    if ((perms & 0040) != 0) result[3] = 'r';
    if ((perms & 0020) != 0) result[4] = 'w';
    if ((perms & 0010) != 0) result[5] = 'x';

    // Permissions for everyone else
    if ((perms & 0004) != 0) result[6] = 'r';
    if ((perms & 0002) != 0) result[7] = 'w';
    if ((perms & 0001) != 0) result[8] = 'x';

    return result;
}

//******************************************************************************

// Author: Simeon K
int findDirectoryEntry(const dirEntryType entries[], unsigned int entryCount, string name) 
// find a filename with given valid entries
{
    int position = -1; // -1 until a matching filename is found.

    // search valid entries until the first match is found.
    for (unsigned int i = 0; i < entryCount & position == -1; i++) 
    {
        if (name == entries[i].fname) { // compare the requested filename with this entry
            position = i;
        } // finish checking file name
    } // finish searching the entries

    return position; // return position or -1 when no name matched
}

// Search a directory for a filename
int findINodeInDirectory(const iNode &directory, const db blockPool [], string name) {

    int result = -1; // Keep -1 unless the operation finds a different result

    unsigned int entriesLeft = directory.size / sizeof(dirEntryType); // count the valid dir entries from its byte size

    // An empty directory has no entries or data blocks to follow.
    if (entriesLeft > 0) { // Only nonempty directories have data blocks to search  
        const indexType *firstIndex = blockPool[directory.addr].index; // First level of block addresses
        
        for (unsigned int i = 0; i < indexPerBlock && entriesLeft > 0 && result == -1; i++) // Stop when match is found or entires run out
        {
            const indexType *secondIndex = blockPool[firstIndex[i]].index; // Follow the address to. a second index block
            
            for (unsigned int j = 0; j < indexPerBlock && entriesLeft > 0 && result == -1; j++) { // Search data blocks until match is found
        
                const dirEntryType *entries = blockPool[secondIndex[j]].dirEntry; //View this data block as dir entries

                // The final block may contain fewer entries;
                unsigned int count = entriesLeft; // Start with the number of entries still unchecked
                if (count > dirEntryPerBlock) { // Check whether more than one block of entries are left
                count = dirEntryPerBlock; // Limit the search to one block.
                }

                int position = findDirectoryEntry(entries, count, name); // Search the valid entries in the block
                if (position != -1) { // Use the entry only when a matching name is found
                    result = entries[position].iNodeIndex; // Save its inode index so both loops stop
                }
                
                entriesLeft -= count; // remove the entries checked in this block
            }
        }

    }
    return result; // return the matching inode index; or -1 when missing 
}

//****
// Author: Simeon K

// Find a directory using an absolute path
int findDirectoryByPath(string path, const iNode iNodePool[], const db blockPool[]) {
   
    int current = -1; // Keep -1 until a valid directory is found

    // splitting the path
    if (!path.empty() && path[0] == '/' && iNodePool[0].type == DIR) { // Requires an absolute path and a directory at root
        // Store directory names in the order they appear in the path.
        queue<string> names;
        string::size_type start = 1;
        
        // locating the end of this name
        while (start < path.size()) { //continue until the whole path has been split
            string::size_type end = path.find('/', start); //find the next slash
            if (end == string::npos) { // handle the last name when no slash follows it
                end = path.size(); // use the end of the path as boundary
            }
           
            if (end > start) { //skip empty pieces between slashes
                names.push(path.substr(start, end - start)); // add this dir name to the queue
            }
            start = end + 1; // move past the slash to the next name
            
        }

        // Every absolute path starts at the inode root.
        current = 0; // begin at the inode root
        while (!names.empty() && current != -1) { //stop after the last name or a failed lookup
            int next = findINodeInDirectory(iNodePool[current], blockPool, names.front()); // look next name

            if (next != -1 && iNodePool[next].type == DIR) { // check the index before accessing the inode
                current = next; // move to the dir that was found
            } else {
                current = -1; // handle a missing name or a regular file and save failure.
            }
            names.pop(); //remove the name just examined            
        }
    }

    return current; // Return the directory's iNode index or -1 on failure.
}

uint32_t readFileData(iNode &node, uint32_t &position, char buffer[], uint32_t nbytes) { //copy bytes and advance the file position
    // Record when this read was performed
    node.aTime = time(NULL); // record this read, including a zero byte or EOF read
    uint32_t copied = 0; // start with no bytes copied

    if (position < node.size) { // access data blocks only when bytes remain
        uint32_t remaining = node.size - position; // count the bytes remaining before EOF

        // Limit the request to the bytes remaining in the file
        if (nbytes > remaining) { // check wheter request goes beyond EOF
            nbytes = remaining; // limit the request to the remaining bytes
        }

        while (copied < nbytes) { // continue until the requested bytes have been copied
            uint32_t fileBlock = position / blockSize; // find the data block number within the file
            uint32_t offset = position % blockSize; // find the byte position within that block

            // Follow the first index block, then the second index block
            indexType secondBlock = blockPool[node.addr].index[fileBlock / indexPerBlock]; //follow the first index level
            indexType dataBlock = blockPool[secondBlock].index[fileBlock % indexPerBlock]; //follow the second index level

            // Copy only the part that fits in this data block
            uint32_t count = blockSize - offset; // count the bytes available in this data block
            if (count > nbytes - copied) { //check whether the bytes are still requested
                count = nbytes - copied; // copy only the requested bytes
            }

            memcpy(buffer + copied, blockPool[dataBlock].data + offset, count); //copy into the next part of the buffer
            position += count; // advance the file position by the copied amount
            copied += count; // add this amount to the total bytes copied
        }
    }

    return copied; // return the copied count, or zero when nothing was read.
}


// Author: Josiah

string my_stat(/* unsigned int addr, string name */) {
	// TODO: need to get the parameters from other functions and globals
	// Also, this needs to verify whether the iNode is used


	if (/* The iNode is actually in use */)
		// Get a copy of the iNode
		iNode file = iNodePool[addr];

		// Now append everything to the string
		string mdata = formatPermissions(file.perms) + ' ';
		mdata += to_string(file.nLinks) + ' ';
		mdata += to_string(file.uid) + ' ';
		mdata += to_string(file.gid) + ' ';
		mdata += to_string(file.size) + ' ';
		mdata += to_string(file.aTime) + ' ';
		mdata += to_string(file.type) + ' ';

		// Need to get the name of the file from directory now
		mdata += name; // Use the name that was looked up in the parent directory
	} else {
		mdata = (char)-4;
	}

	return mdata;
}

int my_read(fdTable &table, unsigned int fd, char buffer[], int nbytes) { // read using an open file descriptor

    int result = -1; // keep the invalid input result unless the read succeeds

    // Check the descriptor before indexing the table
    if (fd < table.tableCapacity && nbytes >= -1) { // check the descriptor and requested count before proceeding
        Entry &file = table.openFiles[fd]; // refer to the actual descriptor entry
        
        if (file.fmode != 'n') { // continue only when this desciptor is ope
            if (file.fmode != 'r') { // reject a desciptor that is not open for reading
                result = -2; // save the existing permission failure result
            } else {
                iNode &node = iNodePool[file.fileAddress]; // access the files inode

                if(node.type == FILE_TYPE) { // read file data only from a regular file
                    uint32_t count = node.size; //use the inode size when nbytes was omitted
                    
                    
                    if (nbytes >= 0) {// check whether a specific count was supplied
                        count = nbytes; // use the requested count instead
                    }

                    result = readFileData(node, file.fp, buffer, count); // read the data and update this descriptors position

                }
            }
        }
    }


    return result; //return the byte count or the saved error result
}

// Author: Simeon K

// Close an open file descriptor
int my_close(fdTable &table, unsigned int fd) {
    int result = -1; // Invalid or already closed desciprtor

    // Check the index before accessing its entry.
    if (fd < table.tableCapacity && table.openFiles[fd].fmode != 'n') {
        // Release this descriptor slot for reuse.
        table.closef(fd);
        result = 1; // success
    }

    return result; // return result of the close attempt.
}

int my_write(fdTable &table, unsigned int fd, const char buffer[],
             int nbytes, blocks &dataBlocks) {
    // TODO: Check the descriptor and write mode.
    // TODO: Write bytes, allocate blocks as needed, and update position/metadata.
    return -1; // Not implemented.
}

int my_mkdir(string path, string pwd, uint16_t userUID, uint16_t userGID,
             uint16_t mask, blocks &inodeBlocks, blocks &dataBlocks) {
    // TODO: Find the parent and check the requested directory name.
    // TODO: Initialize the new directory and add its name to the parent.
    return -1; // Not implemented.
}

int my_link(string oldPath, string newPath, string pwd, blocks &dataBlocks) {
    // TODO: Find the existing file and the new name's parent directory.
    // TODO: Add the new name and update the inode's link count and metadata.
    return -1; // Not implemented.
}

int my_unlink(fdTable &table, string path, string pwd, unsigned int userUID,
              unsigned int userGID, blocks &inodeBlocks, blocks &dataBlocks) {
    // TODO: Find the file and check the required permissions.
    // TODO: Agree on handling an open file's final link.
    // TODO: Remove the name, update links, and release storage when appropriate.
    return -1; // Not implemented.
}

int my_rmdir(fdTable &table, string path, string pwd, unsigned int userUID,
             unsigned int userGID, blocks &inodeBlocks, blocks &dataBlocks) {
    // TODO: Find the directory, protect root, and check permissions.
    // TODO: Remove descendants, then remove this directory and release storage.
    return -1; // Not implemented.
}

int my_opendir(string path, string pwd, dirStream &stream) {
    // TODO: Agree on the directory-stream fields and lifetime rules.
    // TODO: Find the directory and initialize its reading position.
    return -1; // Not implemented.
}

int my_readdir(dirStream &stream, dirEntryType &entry) {
    // TODO: Check the stream and copy the next directory entry.
    // TODO: Advance its position and distinguish an entry, EOF, and failure.
    return -1; // Not implemented.
}

int my_closedir(dirStream &stream) {
    // TODO: Check the stream and release its open state.
    return -1; // Not implemented.
}

int my_kill(blocks &inodeBlocks, blocks &dataBlocks, fdTable &table,
            string pwd, FILE *store, bool &running) {
    // TODO: Agree on a save format that matches the startup restore code.
    // TODO: Save the pools, descriptor table, and current directory.
    // TODO: Stop the request loop only after saving succeeds.
    return -1; // Not implemented; leave running unchanged.
}

// TODO: add the following:
// my_rm() {}
// my_chmod() {}
// my_chown() {}

// END FS CALL TEMPLATEEE
