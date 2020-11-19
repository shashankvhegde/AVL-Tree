
	/*AVL Tree Implementation (Done!)*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int Key_type;
typedef int Data_type;
 
typedef enum{FALSE,TRUE}boolean;

typedef struct Treeentry{
	
	Key_type key;
	Data_type count;
	
}Treeentry;

typedef struct Treenode{
	
	Treeentry entry;
	int balance_factor;
	struct Treenode* left; 
	struct Treenode* right;
	
}Treenode;

int compare(Key_type k1 ,Key_type k2){
	
	/*This function compares the keys*/
	
	if(k1 < k2){
		return -1 ;
	}
	else if(k1 == k2){
		return  0;
	}
	else{
		return 1;
	}
	
}

Treenode* makenode(Treeentry entry){
	
	Treenode *root;
	root = (Treenode*)malloc(sizeof(Treenode));
	if(root == NULL){
		printf("Error\n");
		exit(0);
	}
	else{
		root->entry = entry;
		root->left = root->right = NULL;
		root->balance_factor = 0;
	}
	return root;
}

Treenode* leftrotate(Treenode *root){
	

	Treenode *rs = root->right;
	root->right = rs->left;
	rs->left = root;
	
	root = rs;
//	printf("L\n");
	return root;
}

Treenode* rightrotate(Treenode *root){
	
	Treenode *ls = root->left;
	root->left = ls->right;
	ls->right = root;
	
	root = ls;
//	printf("R\n");
	return root;
}

void traverse(Treenode *root){
	
	/*This function does inorder kind of traversal of the AVL Tree and prints the keys and corresponding values */
	
	if(root != NULL){

		traverse(root->left);
		printf("%d : %d\n",root->entry.key,root->balance_factor);
		traverse(root->right);
		
			
	}

}

Treenode* doublerotateright(Treenode *root){
	
	Treenode *rs = root->right;
	
	root->right = rightrotate(rs);
	root = leftrotate(root);
	
	if(root->balance_factor == 0){

		root->left->balance_factor = 0;
		root->right->balance_factor = 0;
		
	}
	else if(root->balance_factor == 1){

		root->left->balance_factor = 0;
		root->right->balance_factor = -1;
		root->balance_factor = 0;
	}
	else{

		root->left->balance_factor = 1;
		root->right->balance_factor = 0;
		root->balance_factor = 0;		
	}
//	printf("DRR\n");
	return root;
}

Treenode* doublerotateleft(Treenode *root){
	
	Treenode *ls = root->left;
	
	root->left = leftrotate(ls);
	root = rightrotate(root);
	
	if(root->balance_factor == 0){
		root->left->balance_factor = 0;
		root->right->balance_factor = 0;
	
	}
	else if(root->balance_factor == -1){
		root->left->balance_factor = 0;
		root->right->balance_factor = 1;
		root->balance_factor = 0;

	}
	else{
		root->left->balance_factor = -1;
		root->right->balance_factor = 0;
		root->balance_factor = 0;		
	}
//	printf("DLL\n");
	return root;
}

Treenode* Right_balance(Treenode *root,boolean *taller){
	
	Treenode *right = root->right;
	Treenode *oldroot = root;
	
	if(right->balance_factor == 1){
		root = leftrotate(root);
		root->balance_factor = oldroot->balance_factor = 0;
		*taller = FALSE;
	}
	else if(right->balance_factor == 0){
		printf("Error\n");
	}
	else{
		root = doublerotateright(root);
		*taller = FALSE;
		//root->balance_factor = 0;
	}

	return root;
	
}

Treenode* Left_Balance(Treenode *root,boolean *taller){
	
	Treenode *left = root->left;
	Treenode *oldroot = root;
	
	if(left->balance_factor == -1){
		root = rightrotate(root);
		root->balance_factor = oldroot->balance_factor = 0;
		*taller = FALSE;
	}
	else if(left->balance_factor == 0){
		printf("Error\n");
	}
	else{
		root = doublerotateleft(root);
		*taller = FALSE;
		//root->balance_factor = 0;
	}
	
	return root;
}

Treenode* Insert(Treenode *root,Treeentry entry,boolean *taller){
	
	if(!root){
		root = makenode(entry);
		*taller = TRUE;
	}
	else{
		if(compare(entry.key,root->entry.key) == 0){
			root->entry.count += 1;
		}
		else if(compare(entry.key,root->entry.key) > 0){
			root->right = Insert(root->right,entry,taller);
			if(*taller == TRUE){
				if(root->balance_factor == -1){
					root->balance_factor = 0;
					*taller = FALSE;
				}
				else if(root->balance_factor == 0){
					root->balance_factor = 1;
					*taller = TRUE;
				}
				else{
					root = Right_balance(root,taller);

				}
			}
		}
		else{
			root->left = Insert(root->left,entry,taller);
			if(*taller == TRUE){
				if(root->balance_factor == 1){
					root->balance_factor = 0;
					*taller = FALSE;
				}
				else if(root->balance_factor == 0){
					root->balance_factor = -1;
					*taller = TRUE;
				}
				else{
					root = Left_Balance(root,taller);

				}
			}			
		}
	}
	
	return root;	
	
}

boolean successor(Treenode *root){
	
	Treenode *r = root->right;
	Treenode *l = root->left;
		
	if(r != NULL){
		while(r->left != NULL){
			r= r->left;
		}
		root->entry = r->entry;
		return TRUE;
	}else{
		if(l != NULL){
			root->entry = l->entry;
			root->balance_factor = l->balance_factor;
			root->left = l->left;
			root->right = l->right;
			free(l);
		} 
		return FALSE;
	}
	
}

Treenode* Rcases(Treenode *root,boolean *shorter){

	Treenode *left = root->left;
	Treenode *oldroot = root;
	
	if(root->balance_factor == -1){
		if(left->balance_factor == -1){
			root = rightrotate(root);
			oldroot->balance_factor = root->balance_factor = 0;
			*shorter = TRUE;
		}
		else if(left->balance_factor == 0){
			root = rightrotate(root);
			oldroot->balance_factor = -1;
			root->balance_factor = 1;
			*shorter = FALSE;
		}
		else{
			root = doublerotateleft(root);
			*shorter = TRUE;
		}
	}
	else if(root->balance_factor == 0){
		root->balance_factor = -1;
		*shorter = FALSE;
	}	
	else{
		root->balance_factor = 0;
		*shorter = TRUE;
	}
	return root;
}

Treenode* Lcases(Treenode *root, boolean *shorter){

	Treenode *right = root->right;
	Treenode *oldroot = root;
	
	if(root->balance_factor == 1){
		if(right->balance_factor == 1){
			root = leftrotate(root);
			oldroot->balance_factor = root->balance_factor = 0;
			*shorter = TRUE;
		}
		else if(right->balance_factor == 0){
			root = leftrotate(root);
			oldroot->balance_factor = 1;
			root->balance_factor = -1;
			*shorter = FALSE;
		}
		else{
			root = doublerotateright(root);
			*shorter = TRUE;
		}
	}
	else if(root->balance_factor == 0){
		root->balance_factor = 1;
		*shorter = FALSE;
	}	
	else{
		root->balance_factor = 0;
		*shorter = TRUE;
	}
	return root;
}

boolean isleaf(Treenode *root){
	
	if(root->left == NULL && root->right == NULL){
		return TRUE;
	}
	else {
		return FALSE;
	}
}

Treenode* Delete(Treenode *root, Key_type key, boolean *shorter){
	
	if(root == NULL){
		printf("Key NOT FOUND\n");
		*shorter = FALSE;
	}
	else{
		if(compare(key,root->entry.key) == 0){
			if(isleaf(root) && compare(root->entry.key,key) == 0){
				free(root);
				root = NULL;
				*shorter = TRUE;
			}
			else{

				if(successor(root)){
					root->right = Delete(root->right,root->entry.key,shorter);
					if(*shorter == TRUE){
						root = Rcases(root,shorter);
						//*shorter = FALSE;
					}					
				}
				else{
					*shorter = TRUE;
				}// This will only occur somewhere near the leaf level only because if successor is NULL then in an AVL the other subtree will 
				 // have at most 1 node (not more)
				
			}
		}
		else if(compare(key,root->entry.key) > 0){
			root->right = Delete(root->right,key,shorter);
			if(*shorter == TRUE){
				root = Rcases(root,shorter);
				//*shorter = FALSE;
			}			
		}
		else{
			root->left = Delete(root->left,key,shorter);
			if(*shorter == TRUE){
				root = Lcases(root,shorter);
				//*shorter = FALSE;
			}			
		}
	}
	return root;
}


int main() {
	
	int choice = 0;
	Treeentry n;
	n.count = 0;
	Treenode *root = NULL;
	Key_type k;
	boolean t = FALSE,s = FALSE;

	while(choice != 4){
		
		printf("\nThe choices are:\n");
		printf("1.Insertion\n");
		printf("2.Deletion\n");
		printf("3.Print in sorted order\n");
		printf("4.Exit\n");
		
		printf("\nEnter choice:\n");
		
		scanf("%d",&choice);
		switch(choice){
			case 1:
				printf("Enter a number\n");
				scanf("%d",&(n.key));
				n.count = 1;
				root = Insert(root,n,&t);
				break;	
			case 2:
				printf("Enter a number\n");
				scanf("%d",&k);
				root = Delete(root,k,&s);
				
				break;		
			case 3:
				if(root == NULL){
					printf("The Tree is Empty\n");
				}
				else{
					traverse(root);
				}

				break;			
			case 4:
				printf("EXIT!\n");
				break;
			default:
				printf("Invalid choice\n");
				break;				
			
				
		}
	}
	 	
	return 0;
}
