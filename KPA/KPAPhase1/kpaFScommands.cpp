#include "kpaFScommands.h"
#include <ctime>
#include <queue>
#include <cstring>

//******************************************************************************

// Author: Simeon K

bool parseCreateArgs(string requestArgs, string &name){
    name.clear(); // Remove the name from an earlier request.
    bool valid = true; // start with valid until a check fails

    // Reject a missing file name or one containing only spaces.
    if (requestArgs.empty() || requestArgs.find_first_not_of(' ') == string::npos)
    {
        valid = false; //request failed
    }

    // The directory is passed seperately
    //Reject a slash or a name too long for the directory entry.
    if (requestArgs.find('/') != string::npos || requestArgs.size() >= fnameMaxSize) { 
        valid = false; // request failed
    }

    // Continue only when the earlier checks passed
    if (valid) {
    name = requestArgs; // Copy the  filename to the output
    }

    return valid;
}

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

// check a directory change and build its reply
string my_chdir(string path, string pwd) {
    // Invalid path replies keep the shell's existing pwd
    string reply(1, -3); //Start with existing one-byte invalid-path status
    reply += ' '; // seperate the status from the path
    reply += pwd; // preserve the original pwd if the change fails

    bool valid = !path.empty(); // Reject an empty requested path
    // Absolute paths start at root; relative paths start at pwd
    int current = 0; // start absolute path at the root inode
    string target = "/"; // start the absolute target path at the root

    
    if (valid){ // Check the starting directory only when a path is supplied
        if (path[0] != '/') { //A path without a leading slash is relative
            target = pwd; // start a relative path at the current directory

            while (target.size() > 1 && target.back() == '/') { //remove trailing slashes while preserving root
            target.pop_back(); //remove one trailing slash
            }
            
            current = findDirectoryByPath(target, iNodePool, blockPool); // find the starting directory's inode
        }

        if (current == -1 || iNodePool[current].type != DIR) { // Check the index before accessing the inode
            valid = false; // starting directory is invalid
        }
    }

    if (valid) { // Search the requested path only from a valid directory
        // Split the requested path into directory names
        queue<string> names; // store directory names in their path order
        string::size_type start = 0; // start splitting at the start of the path

        while (start < path.size()) { // continue until the whole path has been split
            string::size_type end = path.find ('/', start); // find the next slash

            if (end == string::npos) { // handle the last name when no slash follows it
                end = path.size(); // end of the string is used as boundary
            }
            if (end > start) { // skip empty pieces between slashes
                names.push(path.substr(start, end - start)); //save this directory
            }
            start = end + 1; // move past the slash to the next name
        }
        
        // Search from the selected starting iNode
        while (!names.empty()) { // stop after the last name or a failed lookup
            string name = names.front(); // get the next directory name
            names.pop(); // remove that name from the queue

            int next = findINodeInDirectory(iNodePool[current], blockPool, name); //search the current directory
            
            if (next == -1 || iNodePool[next].type != DIR) { // Reject a missing name or a regular file
                valid = false; // failure so traverse stops
            } else {
                current = next; // move to the directory that was found
                if (target != "/") { //root supplies the first slash
                target += '/'; // seperate this name from the existing path
                }
                target += name; // append the directory name that was found
            }
        }
    }
    
    if (valid) { // replace the faillure reply only after the whole path succeeds
        reply = string (1, 1); // set the existing one-byte success status
        reply += ' '; // seperate the status from the path
        reply += target; // include the completed directory path
    }

    return reply; //return the completed status and path reply.
}

uint32_t readFileData(iNode &node, uint32_t &position, char buffer[], uint32_t nbytes) { //copy bytes and advance the file position
    // Record when this read was performed
    node.aTime = time(NULL); // record this read, including a zero byte or EOF read
    uint32_t copied = 0; // start with no bytes copied

    if (position < node.size) { // access data blocks only when bytes remain
        uint32_t remaining = node.size - position; // count the bytes remaining before EOF

        // Limit the request to the bytes remaining in the file
        if (nbytes > remaining) { // check wheter request goes beyond EOF
            nbytes = remaining // limit the request to the remaining bytes
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

string my_stat(unsigned int addr) {
/* Need to return every bit of metadata about the file.
The permission bits
The number of references pointing to the file
The name of the file owner
The name of the permissions group
The file size in bytes
The last modified date
The type of the iNode, either directory or file
The name of the file
	*/ 
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
	dirEntry entry = directoryBlock[addr];
	mdata += entry.filename;

	return mdata;
}

string my_rewinddir(string pwd) { //return the parent of the shell's current path
    if (pwd != "/") { // leave root unchanged
        pwd = pwd.substr(0, pwd.find_last_of('/')); // remove the final slash and directory name

        if (pwd.empty()) { //removing a root child's name leaves and empty string
            pwd = "/"; // represent its parent as root
        }
    }

    string reply (1, 1); // start with existing success status byte
    reply += ' '; // Separate the status from the path
    reply += pwd; // Add the parent path to the reply

    return reply; // return the completed reply
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

            

        // Passing fp by reference updates this open file's position
        
