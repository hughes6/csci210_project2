#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]) {
    if (pathName == NULL || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64];
    char dirName[64]; 

    struct NODE* parentNode = splitPath(pathName, baseName, dirName);
    if (parentNode == NULL) {
        return;
    }

    struct NODE* child = parentNode->childPtr;
    while (child != NULL) {
        if (strcmp(child->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        child = child->siblingPtr;
    }

    //memory allocate
    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strncpy(newNode->name, baseName, sizeof(newNode->name) - 1); 
    newNode->childPtr = NULL; 
    newNode->siblingPtr = NULL;
    newNode->name[sizeof(newNode->name) - 1] = '\0';
    newNode->fileType = 'D'; 
    newNode->parentPtr = parentNode;
    
    if (parentNode->childPtr == NULL) {
        parentNode->childPtr = newNode; 
    } else {
        //sibling ptr fix
        struct NODE* lastChild = parentNode->childPtr;
        while (lastChild->siblingPtr != NULL) {
            lastChild = lastChild->siblingPtr;
        }
        lastChild->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    char tempPath[128];
    strcpy(tempPath, pathName);

    char *lastSlash = strrchr(tempPath, '/');
    if (lastSlash) {
        *lastSlash = '\0';
        strcpy(baseName, lastSlash + 1);//everything after last slash is baseName
        if (tempPath[0] == '\0') {
            strcpy(dirName, "/");//if baseName was the only thing, dirName is root
        } else {
            strcpy(dirName, tempPath);
        }
    } else {
        strcpy(dirName, "");//no '/', so it's just a filename in cwd
        strcpy(baseName, pathName);
    }

    struct NODE *currentNode = (dirName[0] == '/') ? root : cwd;
    if (strcmp(dirName, "") == 0) {
        return currentNode;
    }

    char *token = strtok(dirName, "/");
    while (token != NULL) {
        struct NODE *child = currentNode->childPtr;
        int found = 0;
        while (child != NULL) {
            if (strcmp(child->name, token) == 0) {
                currentNode = child;
                found = 1;
                break;
            }
            child = child->siblingPtr;
        }
        if (!found) {  
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }   
        token = strtok(NULL, "/");
    }

    return currentNode;
}
