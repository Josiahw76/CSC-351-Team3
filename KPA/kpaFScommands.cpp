#include "kpaFScommands.h"
#include <ctime>


//******************************************************************************

// Author: Simeon K

bool parseCreateArgs(string requestArgs, string &directory, string &name) {
    // Clear results left over from an earlier request
    directory.clear();
    name.clear();

    // Locate the space seperating the directory and filename.
    string::size_type split = requestArgs.find(' ');

    // Return false if no space exists or split is zero.
    if (split == string::npos || split == 0) {
        return false;
    }

    // Put everything after the space into a local string filename.
    string filename = requestArgs.substr(split + 1);

    //Return false if filename is empty or contains only spaces.
    if (filename.empty() || filename.find_first_not_of(' ') == string::npos || filename.size() >= fnameMaxSize)
    {
        return false;
    }

    // Copy the directory portion and filename into the outputs.
    directory = requestArgs.substr(0, split);
    name = filename;

    return true;
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
int findDirectoryEntry(const dirEntryType entries[], unsigned int entryCount, string name) {
    
    // Check each filename in the supplied directory entries
    for (unsigned int i = 0; i < entryCount; i++) {
        if (name == entries[i].fname) {
            return i;
        }
    }

    return -1; // No matching filename was found
}