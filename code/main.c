#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Node {
	char key[50];
    char meaning[200];
	struct Node *left, *right;
} Node;

typedef struct Node * BST;


// /////////////////////////////////////////////////
// ////////////////////// Prototypes
// /////////////////////////////////////////////////


BST Create_Node(char word[50], char meaning[200]);
BST insert(BST Tree, char key[50], char meaning[200]);
BST leftMost(BST root);
BST delete_node(BST Tree, char key[50]);
void show_menu(BST root);
void cmd_2(BST root);
void cmd_3(BST root);
BST cmd_4(BST root);
BST cmd_7(BST root);
void traverse_inorder(BST root);
void print_word(BST node);
void print_tree(BST root, int level);
void print_err(char msg[50]);
void print_heading(char heading[]);
BST read_file(BST root);


// /////////////////////////////////////////////////
// ////////////////////// BST Functions
// /////////////////////////////////////////////////

// creates new nodes
BST Create_Node(char word[50], char meaning[200])
{
	BST new_node
		= (struct Node*)malloc(sizeof(struct Node));

    strcpy(new_node->key, word);

    strcpy(new_node->meaning, meaning);

	new_node->left = new_node->right = NULL;
	return new_node;
}

// insert node to BST
BST insert(BST Tree, char key[50], char meaning[200])
{
	// If tree is empty (or reached a leaf in recursion)
	if (Tree == NULL){
        return Create_Node(key, meaning);
    }

    int cmp = strcmp(key, Tree -> key);

	if (cmp < 0){
        Tree->left = insert(Tree->left, key, meaning);
    }
	else if (cmp > 0){
		Tree->right = insert(Tree->right, key, meaning);
    }

	return Tree;
}


BST leftMost(BST root)
{
	BST current = root;

	// keep going left until the leaf
	while (current != NULL && current->left != NULL){ // TODO remove first != NULL if unneeded
		current = current->left;
    }

	return current;
}

BST search(BST Tree, char key[50]){

    // If tree is empty (or reached a leaf in recursion)
	if (Tree == NULL){
        return Tree;
    }

    int cmp = strcmp(key, Tree -> key);

	if (cmp < 0){ // key is smaller than current tree root

        return search(Tree->left, key);

    }
	else if (cmp > 0){ // key is greater than current tree root

		return search(Tree->right, key);

    }
    
    return Tree;


}

BST delete_node(BST Tree, char key[50])
{

    // If tree is empty (or reached a leaf in recursion)
	if (Tree == NULL){
        return Tree;
    }

    int cmp = strcmp(key, Tree -> key);

	if (cmp < 0){ // key is smaller than current tree root

        Tree->left = delete_node(Tree->left, key);

    }
	else if (cmp > 0){ // key is greater than current tree root

		Tree->right = delete_node(Tree->right, key);

    } else { // key is the current tree root

		// Node with no child on the right
        if (Tree->right == NULL) {
			BST temp = Tree->left;
			free(Tree);
			return temp;
		}

        // Node with no child on the left
        if (Tree->left == NULL) {
			BST temp = Tree->right;
			free(Tree);
			return temp;
		}


		// Node with two children:
		BST temp = leftMost(Tree->right);
		strcpy(Tree->key, temp->key);
        strcpy(Tree->meaning, temp->meaning);
		Tree->right = delete_node(Tree->right, temp->key);

	}
	return Tree;
}

int main()
{

	BST root = NULL;


    /////// sample testing data

	// root = insert(root, "hh", "H");
    // root = insert(root, "cc", "C");
    // root = insert(root, "dd", "D");
    // root = insert(root, "aa", "A");
    // root = insert(root, "jj", "J");
    // root = insert(root, "kk", "K");
    // root = insert(root, "ii", "I");
    // root = insert(root, "ab", "AB");
    // root = insert(root, "ac", "AC");
    // root = insert(root, "jm", "JM");
    // root = insert(root, "jk", "JK");
    // root = insert(root, "ja", "JA");
    // root = insert(root, "ad", "AD");

    read_file(root);

    show_menu(root);

	return 0;
}

// /////////////////////////////////////////////////
// ////////////////////// File functions
// /////////////////////////////////////////////////

BST read_file(BST root){


    char filename[20] = "dictionary.txt";

    FILE* fptr = fopen(filename, "r");

    char word[50], meaning[200], line[600];

    while(fgets(line, sizeof(char) * 600, fptr)){

        int chars_now;
        int chars_consumed = 0;
    
        int args_read = sscanf( line + chars_consumed, "%*d. %[^:]: %[^\t\n0123456789]%n", word, meaning, &chars_now);

        
     
        while ( args_read > 0 ) {
    
            chars_consumed += chars_now;
            
            printf("inserting word >%s< with meaning >%s<\n", word, meaning);
            printf("  ");
    
            root = insert(root, word, meaning);

            args_read = sscanf( line + chars_consumed, "%*d. %[^:]: %[^\t\n0123456789]%n", word, meaning, &chars_now);
        }

        // if (!fgets(line, sizeof(char) * 600, fptr)){
        //     break;
        // };
    
    }

    fclose(fptr);

    return root;
    
}

// /////////////////////////////////////////////////
// ////////////////////// Menu
// /////////////////////////////////////////////////

void show_menu(BST root)
{

    while (1)
    {

        printf("\n\n\n\033[1;36m----------------------------------------------------------------------");
        printf("\n----------------------------------------------------------------------\033[0m \n\n\n");

        printf("\nAvailable Commands:\n\n");

        printf("\n\033[0;34m --- Debug --- \033[0m\n");
        printf("\033[0;31m1 \033[0m: Draw Tree\n");

        printf("\n\033[0;34m --- Operations --- \033[0m\n");

        printf("\033[0;31m2 \033[0m: Search for a word and display its meaning\n");

        printf("\033[0;31m3 \033[0m: Update the meaning of an existing word\n");

        printf("\033[0;31m4 \033[0m: Insert a new word with its meaning\n");

        printf("\033[0;31m5 \033[0m: Print all words in the dictionary in an alphabetic order with their associated meanings\n");
        printf("\033[0;31m6 \033[0m: Print all words that start with a specific character in an alphabetic order\n");

        printf("\033[0;31m7 \033[0m: Delete a word from the dictionary \n");
        printf("\033[0;31m8 \033[0m: Delete all words that start with a specific letter \n");

        printf("\033[0;31m9 \033[0m: Save all words back in file dictionary.txt \n");

        printf("\n\033[0;31mq \033[0m: Quit\n");

        printf("\n > \033[0;36mEnter command number to excute : \033[0m");

        int command = 0;
        scanf("%d", &command);

        printf("\n\n");

        switch (command)
        {
        case 1:
            print_heading("Draw Tree");
            print_tree(root, 0);
            break;

        case 2:
            print_heading("Search for a word and display its meaning");

            cmd_2(root);

            break;

        case 3:
            print_heading("Update the meaning of an existing word");

            cmd_3(root);

            break;

        case 4:
            print_heading("Insert a new word with its meaning");

            root = cmd_4(root);

            break;

        case 5:
            print_heading("Print all words in the dictionary in an alphabetic order with their associated meanings");
            traverse_inorder(root);
            break;

        case 6:
            print_heading("Print all words that start with a specific character in an alphabetic order");

            break;

        case 7:
            print_heading("Delete a word from the dictionary");
            
            cmd_7(root);

            break;

        case 8:
            print_heading("Delete all words that start with a specific letter");
            
            break;

        case 9:
            print_heading("Save all words back in file dictionary.txt");
            
            break;

        default:
            printf("\033[1;31minvalid command number, quitting ...\033[0m\n");
            printf("\n\n\n");
            exit(0);
        }
    }
}

// /////////////////////////////////////////////////
// ////////////////////// Menu Functions
// /////////////////////////////////////////////////

void cmd_2(BST root){
    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    BST node = search(root, key);

    if ( node == NULL ){
        print_err("word was not found");
        return;
    }

    print_word(node);
}

void cmd_3(BST root){

    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);



    BST node = search(root, key);

    if ( node == NULL ){
        print_err("word was not found");
        return;
    }

    char trash;
    scanf("%c", &trash);


    printf("Enter the new meaning : ");

    char meaning[200];

    fgets(meaning, sizeof(char) * 200, stdin);
    sscanf(meaning, "%[^\n]", meaning);

    strcpy( node -> meaning,  meaning);

}

BST cmd_4(BST root){

    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    BST node = search(root, key);

    if ( node != NULL ){
        print_err("word already exists !");
        return root;
    }

    char trash;
    scanf("%c", &trash);

    printf("Enter the meaning : ");

    char meaning[200];

    fgets(meaning, sizeof(char) * 200, stdin);
    sscanf(meaning, "%[^\n]", meaning);

    return insert(root, key, meaning);
}

BST cmd_7(BST root){

    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    return delete_node(root, key);
    
}

// /////////////////////////////////////////////////
// ////////////////////// Print Functions
// /////////////////////////////////////////////////

// InOrder traversal of BST
void traverse_inorder(BST root)
{
	if (root != NULL) {
		traverse_inorder(root->left);
		printf("(%s):[%s] \n", root->key, root->meaning);
		traverse_inorder(root->right);
	}
}

void print_word(BST node){

    if (node == NULL){
        return;
    }

    printf("(%s):[%s] \n", node->key, node->meaning);

}

// for debugging Print BST (keys only)
void print_tree(BST root, int level)
{
	    

        for (int i = 0; i < level; i++){
            printf(i == level - 1 ? " |--- " : "      ");
        }

        if (root == NULL){
            printf("- \n\n");
            return;
        }

        // printf("(%s):[%s] \n\n", root->key, root->meaning);
        printf("(%s) \n\n", root->key);



        print_tree(root->left, level + 1);
        print_tree(root->right, level + 1);

}

void print_err(char msg[50]){

    printf("\n\033[0;31m%s\033[0m\n", msg);

}

void print_heading(char heading[])
{
    printf("\n\n-----------------------------------\n> \033[0;31m%s\033[0m \n-----------------------------------\n\n", heading);
}