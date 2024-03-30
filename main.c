#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

struct AVLnode;
typedef struct AVLnode *AVL_T;

struct AVLnode{
    char courseName[40];
    int creditHours;
    char courseCode[9];
    char cousrseDep[20];
    char topics[150];
    AVL_T Left;
    AVL_T Right;
    int Height;
};

void printInFile(AVL_T t, FILE *file);
void printnodes(AVL_T nodes[], int size);
void printTopics(AVL_T tree);
void delete_by_department(AVL_T t, char courseDep[20], int *size);
void delete_by_character(AVL_T t, char courseCh, int *size);
void menu();
char *trimwhitespace(char *str);
int countlines(FILE *filename);
void loadInfo(AVL_T nodes[]);
AVL_T createNode(char courseName[40], int creditHours, char courseCode[8], char courseDep[20], char topics[150]);
int getHeight (AVL_T t);
int MAX (int x, int y);
AVL_T singleRotateLeft(AVL_T x);
AVL_T singleRotateRight(AVL_T y);
AVL_T doubleRotateLeft(AVL_T K3);
AVL_T doubleRotateRight(AVL_T K3);
AVL_T insert(AVL_T tree, AVL_T node);
AVL_T insertAll(AVL_T tree, AVL_T nodes[], int size);
AVL_T insertNewNode(AVL_T tree);
void printInOrder(AVL_T t);
AVL_T deleteNode(AVL_T root, char courseCode[9]);
AVL_T deleteTheNode(AVL_T tree, int *size);
void search_update(AVL_T tree);

int main(){

    AVL_T tree = NULL;
    char ch;
    char courseDep[20];
    //-------------------------------------------------------------------------------------------------
    FILE *info_lines; //this will be used to get it the buses file
    info_lines = fopen("courses.txt", "r");
    if(info_lines == NULL){ //if file not found
        printf("file courses.txt cannot be found");
        exit(1);
    }
    int numOfLines = countlines(info_lines); //if found count the lines of it (as number of buses).
    AVL_T nodes_arr[numOfLines]; //initialing the array of buses
    //-------------------------------------------------------------------------------------------------
    FILE *file;
    file = fopen("offered_courses.txt", "w");
//--------------------------------------------------------
    printf("\t\t\t\t    Welcome to my program\n");
    int selection = 0; //selection to know which thing from the menu it wants to do
    menu(); //printing menu on the screen for the user
    scanf("%d", &selection); //reading the selection

    while(selection != 11){ //while the user didn't choose exit
        switch(selection){
            case 1: //if first choice
                loadInfo(nodes_arr);
                tree = insertAll(tree, nodes_arr, numOfLines);
                break;
            case 2: //if second choice
                tree = insertNewNode(tree);
                break;
            case 3: // if choice 3
                search_update(tree);
                break;
            case 4: //if choice 4
                printInOrder(tree);
                break;
            case 5:
                printTopics(tree);
                break;
            case 6:

                break;
            case 7:
                tree = deleteTheNode(tree, &numOfLines);
                break;
            case 8:
                printf("Please enter course first character:\n");
                scanf("%c", &ch);
                scanf("%c", &ch);
                delete_by_character(tree, ch, &numOfLines);
                printInOrder(tree);
                break;
            case 9:
                printf("Please enter course department:\n");
                gets(courseDep);
                gets(courseDep);
                delete_by_department(tree, courseDep, &numOfLines);
                printInOrder(tree);
                break;
            case 10:
                printInFile(tree, file);
                break;
            default:
                printf("please enter numbers betweem 1-11\n");
        }
        menu(); //putting the menu again on screen
        scanf("%d", &selection); //reading the selection for the while loop
    }

    return 0;
}

void menu(){ //this function is used to print the menu.
    printf("-------------------------------------------------Menu-----------------------------------------------------------\n");
    printf("\t..........................................................................................\n");
    printf("\t:\t1.  Read the file courses.txt file and create the tree.\t\t\t \t:\n");
    printf("\t:\t2.  Insert a new course from the user with all its associated data.\t\t :\n");
    printf("\t:\t3.  Find a course and support updating of its information \t\t\t\t :\n");
    printf("\t:\t4.  List courses in lexicographic order with their associated information \n\t\t(credit hours, IDs, and topics). \t\t\t\t\t :\n");
    printf("\t:\t5.  List all topics associated with a given course. \t\t\t\t :\n");
    printf("\t:\t6.  List all courses in lexicographic order that belong to the same department.   :\n");
    printf("\t:\t7.  Delete a course.\t\t\t\t\t\t\t\t :\n");
    printf("\t:\t8.  Delete all courses that start with a specific letter. \t\t\t :\n");
    printf("\t:\t9.  Delete all courses belong to a given department. \t\t\t\t :\n");
    printf("\t:\t10. Save all words in file offered_courses.txt \t\t\t\t :\n");
    printf("\t:\t11. Exit.\t\t\t\t\t\t\t\t\t :\n");
    printf("\t..........................................................................................\n\n");
    printf("What is your Choice?\n");
}

char *trimwhitespace(char *str){ //this function used to trim all spaces from the words to that strcmp work properly
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

int countlines(FILE *filename){
    // count the number of lines in the file called filename
    int ch=0;
    int lines=1;

    if (filename == NULL){
        return 0;
    }

    while ((ch = fgetc(filename)) != EOF){ //read till the end of file
        if (ch == '\n') //if newline lines++
            lines++;
    }
    fclose(filename);
    return lines;
}

void loadInfo(AVL_T nodes[]){ //this fuction used to load info from file and put it into array of struct(pointer to struct)
    FILE *infofile;
    infofile = fopen("courses.txt", "r"); //the file to read from
    char info[250]; //to store the line in it

    if(infofile == NULL){
        printf("file courses.txt cannot be found");
        exit(1);
    }

    int i=0;//for the array and inputs
    char *token; //to divide the string and store it in it
    char Name[40];
    char *ptrName;
    int creditHours;
    char credit[9];
    char courseCode[9];
    char *ptrCode;
    char courseDep[20];
    char *ptrDep;
    char topics[150];
    char *ptrTopics;

    while(fgets(info, 250, infofile)){ //while not end of file and still reading

        token = strtok(info, ":"); //read the first part of the line

        ptrName = trimwhitespace(token); //to trim it and store it
        strcpy(Name, ptrName);

        token = strtok(NULL, "#"); //read the second part
        strcpy(credit, token);
        creditHours = atoi(credit);

        token = strtok(NULL, "#");
        ptrCode = trimwhitespace(token);
        strcpy(courseCode, ptrCode);

        token = strtok(NULL, "/");
        ptrDep = trimwhitespace(token);
        strcpy(courseDep, ptrDep);

        token = strtok(NULL, "/");
        ptrTopics = trimwhitespace(token);
        strcpy(topics, ptrTopics);

        nodes[i] = createNode(Name, creditHours, courseCode, courseDep, topics); //creating new node of bus and storing it into the array
        i++; //next array element

    }

    fclose(infofile);
}

AVL_T createNode(char courseName[40], int creditHours, char courseCode[9], char courseDep[20], char topics[150]){
    //this function is used to create a new bus node
    AVL_T node = (AVL_T) malloc(sizeof (struct AVLnode)); //initializing space and the node
    strcpy(node->courseName, courseName);
    node->creditHours = creditHours;
    strcpy(node->courseCode, courseCode);
    strcpy(node->topics, topics);
    strcpy(node->cousrseDep, courseDep);
    node->Left = NULL;
    node->Right = NULL;
    node->Height = 0;
    return node;
}

void printnodes(AVL_T nodes[], int size){
    for(int i=0; i < size; i++){
        printf("name: |%s|\nhours: |%d|\ncode: |%s|\ndepartment: |%s|\ntopics: |%s|\n\n", nodes[i]->courseName, nodes[i]->creditHours, nodes[i]->courseCode, nodes[i]->cousrseDep, nodes[i]->topics);
    }
}

int getHeight (AVL_T t){
    if(t==NULL)
        return -1;
    else
        return t->Height;
}

int MAX (int x, int y){
    if(x>y)
        return x;
    else
        return y;
}

AVL_T singleRotateLeft(AVL_T x) {
    AVL_T y;
    y         = x->Right;
    x->Right  = y->Left;
    y->Left = x;

    x->Height = 1 + MAX(getHeight(x->Left) , getHeight(x->Right));
    y->Height = 1 + MAX(getHeight(x) , getHeight(y->Right));

    return y;
}

AVL_T singleRotateRight(AVL_T y) {
    AVL_T x;
    x        = y->Left;
    y->Left  = x->Right;
    x->Right = y;

    y->Height = 1 + MAX(getHeight(y->Right) , getHeight(y->Left));
    x->Height = 1 + MAX(getHeight(y) , getHeight(x->Left));

    return x;
}

AVL_T doubleRotateLeft(AVL_T K3) {
    K3->Left  = singleRotateLeft(K3->Left);
    return singleRotateRight(K3);
}

AVL_T doubleRotateRight(AVL_T K3) {
    K3->Right  = singleRotateRight(K3->Right);
    return singleRotateLeft(K3);
}

AVL_T insert(AVL_T tree, AVL_T node){
    if(node == NULL){
        printf("Node is empty!!\n");
        exit(1);
    }
    if(tree == NULL){
        tree = node;
    }
    else if(strcmp(node->courseCode, tree->courseCode) < 0){
        tree->Left=insert(tree->Left, node);
        if(getHeight(tree->Left) - getHeight(tree->Right) >= 2){

            if(strcmp(node->courseCode, tree->Left->courseCode) > 0)
                tree=singleRotateRight(tree);
            else
                tree=doubleRotateLeft(tree);
        }
    }
    else if(strcmp(node->courseCode, tree->courseCode) > 0){
        tree->Right = insert(tree->Right, node);
        if(getHeight(tree->Right) - getHeight(tree->Left) >= 2){
            if(strcmp(node->courseCode, tree->Left->courseCode) < 0)
                tree=singleRotateLeft(tree);
            else
                tree=doubleRotateRight(tree);
        }
    }
    tree->Height = 1 + MAX(getHeight(tree->Right) , getHeight(tree->Left));
    return tree;
}

AVL_T insertAll(AVL_T tree, AVL_T nodes[], int size){
    for(int i=0; i < size; i++){
        tree=insert(tree, nodes[i]);
    }
    return tree;
}

void printInOrder(AVL_T t){
    if(t!= NULL){
        printInOrder(t->Left);
        printf("name: |%s|\nhours: |%d|\ncode: |%s|\ndepartment: |%s|\ntopics: |%s|\n\n", t->courseName, t->creditHours, t->courseCode, t->cousrseDep, t->topics);
        printInOrder(t->Right);
    }
}

AVL_T insertNewNode(AVL_T tree){
    char courseName[40];
    int creditHours;
    char courseCode[9];
    char cousrseDep[20];
    char topics[150];

    printf("\nPlease enter the course name:\n");
    scanf("%s", courseName);

    printf("Please enter the credit hours:\n");
    scanf("%d", &creditHours);

    printf("Please enter course code:\n");
    scanf("%s", courseCode);

    printf("Please enter course department:\n");
    scanf("%s", cousrseDep);

    printf("Please enter course topics as (topic1, topic2, ...., topic n:\n");
    scanf("%s", topics);

    AVL_T node = createNode(courseName, creditHours, courseCode, cousrseDep, topics);
    insert(tree, node);
}

AVL_T search(AVL_T ptr, char courseCode[9]){
    if (ptr != NULL) {
        if (strcmp(courseCode, ptr->courseCode) < 0){
            ptr = search(ptr->Left, courseCode);
        }else if (strcmp(courseCode, ptr->courseCode) > 0){
            ptr = search(ptr->Right, courseCode);
        }
    }
    return ptr;
}

void search_update(AVL_T tree){
    char courseCode[9];

    printf("Please enter course code:\n");
    scanf("%s", courseCode);

    char yesName[5];
    char yesDepartment[5];
    char yesCredit[5];
    char yesTopics[5];
    char courseName[40];
    int creditHours;
    char cousrseDep[20];
    char topics[150];

    AVL_T searchOut = search(tree, courseCode);

    if(searchOut == NULL){
        printf("There is no node with such a code\n");
        return NULL;
    }else{
        printf("\nDo you want to change the course name? (yes or no)\n");
        scanf("%s", yesName);
        if(strcmp(yesName, "yes") == 0){
            printf("Please enter the new course name:\n");
            gets(courseName);
            gets(courseName);
            strcpy(searchOut->courseName, courseName);
        }
        printf("\nDo you want to change the course credit hours? (yes or no)\n");
        scanf("%s", yesCredit);
        if(strcmp(yesCredit, "yes") == 0){
            printf("Please enter the new course credit hours:\n");
            scanf("%d", &creditHours);
            searchOut->creditHours = creditHours;
        }
        printf("\nDo you want to change the course department? (yes or no)\n");
        scanf("%s", yesDepartment);
        if(strcmp(yesDepartment, "yes") == 0){
            printf("Please enter the new course department:\n");
            gets(cousrseDep);
            gets(cousrseDep);
            strcpy(searchOut->cousrseDep, cousrseDep);
        }
        printf("\nDo you want to change the course topics? (yes or no)\n");
        scanf("%s", yesTopics);
        if(strcmp(yesTopics, "yes") == 0){
            printf("Please enter the new course topics (topic 1, topic 2, ..., topic N):\n");
            gets(topics);
            gets(topics);
            strcpy(searchOut->topics, topics);
        }
    }
}

void printTopics(AVL_T tree){
    char courseCode[9];

    printf("Please enter course code:\n");
    scanf("%s", courseCode);

    AVL_T searchOut = search(tree, courseCode);

    if(searchOut == NULL){
        printf("There is no node with such a code\n");
        return NULL;
    }else{
        printf("Topics for %s are:\n%s.\n", courseCode, searchOut->topics);
    }
}

AVL_T minValueNode(AVL_T node){
    AVL_T current = node;

    while (current->Left != NULL) // loop down to find the leftmost leaf
        current = current->Left;

    return current;
}

int getBalanceFactor(AVL_T N) {
    if (N == NULL)
        return 0;
    return getHeight(N->Left) - getHeight(N->Right);
}

AVL_T deleteTheNode(AVL_T tree, int *size){

    AVL_T t = tree;
    char courseCode[9];

    printf("Please enter course code:\n");
    scanf("%s", courseCode);

    AVL_T srch = search(tree, courseCode);
    if(srch == NULL){
        printf("This node does not exist!\n");
        return NULL;
    }else{
        *size = *size -1;
        t = deleteNode(t, courseCode);
        return t;
    }
}

AVL_T deleteNode(AVL_T root, char code[9]) {    //function to delete specfic node according to the code

    if (root == NULL)
        return root;

    if (strcmp(code, root->courseCode) < 0)
        root->Left = deleteNode(root->Left, code);
    else if (strcmp(code, root->courseCode) > 0) {
        root->Right = deleteNode(root->Right, code);
    } else {
        if ((root->Left == NULL) || (root->Right == NULL)) {
            AVL_T temp = root->Left ? root->Left : root->Right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            AVL_T temp = minValueNode(root -> Right);
            copy(root, temp);
            root->Right = deleteNode(root->Right, temp->courseCode);

        }
        if (root == NULL)
            return root;

    }
    root->Height = MAX(getHeight(root->Left), getHeight(root));
    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(root->Left) >= 0)
        return singleRotateRight(root);
    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->Left) < 0) {
        root->Left = singleRotateLeft(root->Left);
        return singleRotateRight(root);
    }
    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->Right) <= 0)
        return singleRotateLeft(root);
    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->Right) > 0) {
        root->Right = singleRotateRight(root->Right);
        return singleRotateLeft(root);
    }
    return root;
}

void copy(AVL_T tree, AVL_T temp) {
    strcpy(tree->courseName, temp->courseName);
    tree->creditHours = temp->creditHours;
    strcpy( tree->courseCode, temp->courseCode);
    strcpy( tree->cousrseDep, temp->cousrseDep);
    strcpy(tree->topics, temp->topics);
}

void delete_by_department(AVL_T t, char courseDep[20], int *size){

    if(t!= NULL){
       if(strcmp(t->cousrseDep, courseDep) == 0){
           deleteNode(t, t->courseCode);
           *size = *size -1;
       }
       delete_by_department(t->Left, courseDep, size);
       delete_by_department(t->Right, courseDep, size);
    }
}

void delete_by_character(AVL_T t, char courseCh, int *size){
    if(t!= NULL){
        if(t->courseCode[0] == courseCh){
            deleteNode(t, t->courseCode);
            *size = *size -1;
        }
        delete_by_character(t->Left, courseCh, size);
        delete_by_character(t->Right, courseCh, size);
    }
}

void printInFile(AVL_T t, FILE *file){
    if(t!= NULL){
        printInFile(t->Left, file);
        fprintf(file,"name: |%s|\nhours: |%d|\ncode: |%s|\ndepartment: |%s|\ntopics: |%s|\n\n", t->courseName, t->creditHours, t->courseCode, t->cousrseDep, t->topics);
        printInFile(t->Right, file);
    }
    fclose(file);
}
