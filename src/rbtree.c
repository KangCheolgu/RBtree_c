#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void delete_node(rbtree *,node_t *);
void inorder(const rbtree *, key_t *, const size_t, node_t *, size_t *);
rbtree *new_rbtree(void) {

  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t)); 
  p->root = p->nil; 
  p->nil->color = RBTREE_BLACK;
  return p;
}

void delete_rbtree(rbtree *t) {
	
	delete_node(t,t->root);

	free(t);
}

void delete_node(rbtree *t, node_t *curNode){
	if(curNode == t->nil) {
		return;
	}
	
	delete_node(t, curNode->left);
	delete_node(t, curNode->right);

	free(curNode);
}

node_t *rotateLeft(rbtree *t, node_t *node){
	node_t *p_node = node->parent;
	node_t *gp_node = node->parent->parent;
	// p node는 왼쪽 
	if(p_node == gp_node->left){
		
		gp_node->left = node;
		node->parent = gp_node;
		
		p_node->right = node->left;
		node->left->parent = p_node;

		p_node->parent = node;
		node->left = p_node;
		
		return p_node;

	// p node는 오른쪽	
	} else {
		
		p_node->parent = gp_node->parent;

		if (p_node->parent != t->nil){
			if(gp_node->parent->left == gp_node){
				gp_node->parent->left = p_node;
			} else {
				gp_node->parent->right = p_node;
			}
		} else {
			t->root = p_node;
		}
		
		gp_node->right = p_node->left;
		p_node->left->parent = gp_node;

		p_node->left=gp_node ;
		gp_node->parent = p_node;

		return gp_node;
	}

}
node_t *rotateRight(rbtree *t, node_t *node){
	node_t *p_node = node->parent;
	node_t *gp_node = node->parent->parent;
	// pnode 왼쪽
	if(p_node == gp_node->left){
		
		p_node->parent = gp_node->parent;

		if (p_node->parent != t->nil){
			if(gp_node->parent->left == gp_node){
				gp_node->parent->left = p_node;
			} else {
				gp_node->parent->right = p_node;
			}
		} else {
			t->root = p_node;
		}

		gp_node->left = p_node->right;
		p_node->right->parent = gp_node;
		
		p_node->right = gp_node;
		gp_node->parent = p_node;
		
		return gp_node;
	} else {
		gp_node->right= node;
		node->parent = gp_node;
		
		p_node->left= node->right;
		node->right->parent = p_node;

		p_node->parent = node;
		node->right= p_node;

		return p_node;
	}
}

node_t *change_color(rbtree *t, node_t *node){	
	node_t *gp_node = node->parent->parent;

	gp_node->color = RBTREE_RED;
	gp_node->left->color = RBTREE_BLACK;
	gp_node->right->color = RBTREE_BLACK;

	return gp_node;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 맨 처음 들어갈 때는 root 에 key를 저장, 루트의 색을 확인해야함.
  node_t *new_node =  (node_t *)calloc(1, sizeof(node_t));
  
  new_node->color = RBTREE_RED; 
  new_node->key = key;

  node_t *x = t->root;
  node_t *y = t->nil;
  
  // 만약 t 의 루트 노드가 NULL 
  if ( x == t->nil ) {
    //printf("루트가 널 일때\n");
    
    t->root = new_node;
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->parent = t->nil;

    // 루트노드는 무조건 블랙이어야 한다.
    new_node->color = RBTREE_BLACK; 

	return new_node;
    // 만약 t가 루트 노드가 아니면
  } else {
	//printf("t->root 는 nil이 아닙니다. 현재키 %d\n", new_node->key);
	int rl = -1;
    while(x!=t->nil)  {
		y = x; 
		if (x->key > key) {
			x = x->left;
			rl = 0;
		} else {
			x = x->right;
			rl = 1;
		}
	}

	if(rl == 0){
		// printf("%d 의 left에 %d 삽입\n", y->key, new_node->key);
		y->left = new_node;
	} else {
		// printf("%d 의 right에 %d 삽입\n", y->key, new_node->key);
		y->right = new_node;
	}
	new_node->left = t->nil; 
	new_node->right = t->nil;
	new_node->parent = y;


	}	
	// 삽입이 끝나고 난 뒤 RB 트리의 조건에 맞춰 노드를 변경 해주어야 한다.
	// 루트의 경우에는 리턴 하니 상관 할 필요 없다.
	// 루트는 무조건 블랙이기 때문에 부모의 색깔이 RED라면 무조건 할아버지 노드는 존재할수밖에 없다.
	// 부모의 색깔이 RED인가?
	node_t *cursor = new_node;
	//printf("와일문 들어가기전 커서 확인 : %d\n", cursor->key);
	//printf("와일문 들어가기전 커서의 부모 확인 : %d\n", cursor->parent->key);
	//printf("와일문 들어가기전 커서의 부모의 색  확인 : %d\n", cursor->parent->color);
	while(cursor->parent->color == RBTREE_RED){
		//printf("current cursor key : %d\n", cursor->key);
		node_t *p_node = cursor->parent; 
		node_t *gp_node = cursor->parent->parent;
		
		// CASE1 부모와 삼촌이 색깔이 같은가? 빨간색인가
		if((gp_node->left->color == RBTREE_RED) & (gp_node->right->color == RBTREE_RED)){
			// printf("case 1\n");
			cursor = change_color(t, cursor);
		}
					
		// CASE 2 삼촌은 검정이고 부모와 나의 방향이 같다.
		// 둘다 left
		else if ((gp_node->left == p_node) & (p_node->left == cursor) & (gp_node->right->color == RBTREE_BLACK)){
			// printf("case 2-1\n");
			
			p_node->color = RBTREE_BLACK;
			gp_node->color = RBTREE_RED;

			cursor = rotateRight(t, cursor);
		}

		// 둘다 right
		else if ((gp_node->right == p_node) & (p_node->right == cursor) & (gp_node->left->color == RBTREE_BLACK)){
			// printf("case 2-2\n");
			
			p_node->color = RBTREE_BLACK;
			gp_node->color = RBTREE_RED;

			cursor = rotateLeft(t, cursor);
		}

		// CASE 3 부모와 자식의 방향이 다를경우
		// 부모 left 자식 right
		else if ((gp_node->left == p_node) & (p_node->right == cursor) & (gp_node->right->color == RBTREE_BLACK)){
			// printf("case 3-1\n");
			cursor = rotateLeft(t, cursor);
		}

		// 부모 right 자식 left
		else if ((gp_node->right == p_node) & (p_node->left == cursor) & (gp_node->left->color == RBTREE_BLACK)){
			// printf("case 3-2\n");
			cursor = rotateRight(t, cursor);
		}
		// printf("t->root %d \n",t->root->key);
		t->root->color = RBTREE_BLACK;
	}
	return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    // printf("파인드에 들어왔습니다.:\n");
	node_t *cur_node = t->root;

	while(cur_node != t->nil){
		if( cur_node->key == key ){
			if(cur_node->right->key == key){
				cur_node = cur_node->right;
			} else if (cur_node->left->key == key) {
				cur_node = cur_node->left;	
			}
			return cur_node;
		}
		else if (cur_node->key < key){
			cur_node = cur_node->right;
		}
		else if (cur_node->key > key){
			cur_node = cur_node->left;
		}
	}
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
	node_t *x = t->root;
	while(x != t->nil){
		x = x->left;
		if(x->left == t->nil){
			break;
		}
	}	
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
	node_t *x = t->root;
	while(x != t->nil){
		x = x->right;
		if(x->right == t->nil){
			break;
		}
	}	
  return x;
}

void check_double_black(rbtree *t,node_t *n){	
	printf("into check extra black-----------------\n");
	printf("n's color %d \n -----",n->color);
	if(n == t->root) {
		t->root = t->nil;
		return;
	}
	
	while((n != t->root) & (n->color == RBTREE_BLACK)) {
		node_t *p_node = n->parent;
		printf("와일문 아래 n 확인용 n = %d \n", n->key);
		printf("와일문 아래 n 확인용 n->color = %d \n", n->color);
		printf("parent node of n is %d \n " , p_node->key);

		// n의 위치가 왼쪽	
		if(n == p_node->left){
			node_t *s_node = p_node->right; //형제노드
			printf("n의 위치는 왼쪽 현재 노드와 형제 노드 : %d   %d   \n", n->key, s_node->key);
			// 형제의 색깔이 빨강 CASE 4 
			// 부모와 형제의 색을 바꾸고 왼쪽으로 회전시킨다.
			if(s_node->color == RBTREE_RED){
				printf("형제의 색깔의 빨강 -> CASE 4\n");
				p_node->color = RBTREE_RED;
				s_node->color = RBTREE_BLACK;
				
				rotateLeft(t,s_node->right);
			// 형제의 색깔이 검정 
			// 조카의 색깔이 둘다 검정? 왼쪽만 빨강? 오른쪽만 빨강? 으로 나뉜다.
			} else {
				// 오른쪽 조카의 색깔이 빨강
				if(s_node->right->color == RBTREE_RED) {
					
					printf("형제의 색깔의 검정 -> CASE 1\n");
					s_node->color = p_node->color;
					s_node->right->color = RBTREE_BLACK;
					p_node->color = RBTREE_BLACK;

					rotateLeft(t,s_node->right);
					return;
				// 조카의 색깔이 왼쪽만 빨강
				} else if ((s_node->left->color == RBTREE_RED) & (s_node->right->color == RBTREE_BLACK)) {
					printf("형제의 색깔의 검정 -> CASE 2\n");
					s_node->color = RBTREE_RED;
					s_node->left->color = RBTREE_BLACK;
					rotateRight(t,s_node->left);
					n = p_node->left;
					n->parent = p_node;
				// 조카 둘다 검정
				// 현 부모노드를 새로운 n으로 바꾸고 와일문 다시 돌리기
				} else {
					printf("형제의 색깔의 검정 -> CASE 3\n");
					s_node->color = RBTREE_RED;
					n = p_node;
				}

			}
		// n의 위치가 오른쪽
		} else {
			node_t *s_node = p_node->left; //형제노드
			printf("n의 위치는 오른쪽 현재 노드와 형제 노드 : %d   %d   \n", n->key, s_node->key);

			printf("형제노드 %d \n", s_node->key);
			// 형제의 색깔이 빨강
			// 부모와 형제의 색을 바꾸고 왼쪽으로 회전시킨다.
			if(s_node->color == RBTREE_RED){
				printf("형제의 색깔의 빨강 -> CASE 4\n");
				p_node->color = RBTREE_RED;
				s_node->color = RBTREE_BLACK;
				rotateRight(t,s_node->left);
			// 형제의 색깔이 검정 
			} else {

				// 오른쪽 조카의 색깔이 빨강
				if(s_node->left->color == RBTREE_RED) {
					printf("형제의 색깔의 검정 -> CASE 1\n");
					s_node->color = p_node->color;
					s_node->left->color = RBTREE_BLACK;
					p_node->color = RBTREE_BLACK;
					rotateRight(t,s_node->left);
					return;
				// 조카의 색깔이 오른쪽만 빨강
				} else if ((s_node->right->color == RBTREE_RED) & (s_node->left->color == RBTREE_BLACK)) {
					printf("형제의 색깔의 검정 -> CASE 2\n");
					s_node->color = RBTREE_RED;
					s_node->right->color = RBTREE_BLACK;
					rotateLeft(t,s_node->right);
					n = p_node->right;
					n->parent = p_node;
				// 조카 둘다 검정
				// 현 부모노드를 새로운 n으로 바꾸고 와일문 다시 돌리기
				} else {
					printf("형제의 색깔의 검정 -> CASE 3\n");
					s_node->color = RBTREE_RED;
					n = p_node;
				}
			}
		}
	}
	if (n->color == RBTREE_RED) {
		//printf("확인용 n = %d \n", n->key);
		//printf("확인용 n->color = %d \n", n->color);
		n->color = RBTREE_BLACK;
	}
}

node_t *find_successor(rbtree *t, node_t *p) {
	
	node_t *successor = p->right; 

	while (successor->left != t->nil) {
		successor = successor->left;
	}

	return successor;
}

int rbtree_erase(rbtree *t, node_t *p) {
	// 삭제되는 애가 빨간색이면 변화 없음. 검정색일 경우 조건을 따져야 한다.
	node_t *p_node = p->parent;
	node_t *heir_node = t->nil;
	// 처음 확인 해야 하는 조건. 들어온 노드가 양쪽 다 자식을 가지고 있나?
	// 자식이 둘다 있을 경우 석세서를 찾아서 걔의 키값을 p에 복사하고 석세서를 삭제한다.
	// 석세서를 삭제
	if ((p->left != t->nil) & (p->right != t->nil)) {
		node_t *successor = find_successor(t,p);
		printf("currnet key : %d \n", p->key);
		printf("successor key : %d \n", successor->key);
		// 삭제되는 석세서의 색깔이 검정색인가?
		if (successor->color == RBTREE_BLACK) {
			p->key = successor->key;
			heir_node = successor->right; 
			
			if(successor->parent == p) {
				successor->parent->right = heir_node;
				heir_node->parent = successor->parent;
			} else {
				successor->parent->left= heir_node;
				heir_node->parent = successor->parent;
			}
		
			check_double_black(t,heir_node);

			free(successor);

		// 삭제되는 석세서의 색깔이 빨간색인가?
		} else {
			p->key = successor->key; // 삭제대상 키값을 석세서의 키값으로 변경
				
			if(successor->parent == p) {
				successor->parent->right = t->nil;
			} else {
				successor->parent->left= t->nil;
			}
			free(successor); // 석세서 제거 
		}
		
	// 자식이 둘 중에 하나라도 없으면 p를 삭제하고 대체할 놈을 찾는다.
	// p를 삭제
	} else {
		// p의 색깔이 검정색인가 
		if(p->color == RBTREE_BLACK) {
			// 자식이 없는가
			if((p->left == t->nil) & (p->right == t->nil)){
				// 첵 더블 블랙을 실행하고 free p
				if(p_node == t->nil) {
					check_double_black(t,p);
				} else if(p_node->left == p){
					p_node->left = t->nil;
					p_node->left->parent = p_node;
					check_double_black(t,p_node->left);
				} else {
					p_node->right = t->nil;
					p_node->right->parent = p_node;
					check_double_black(t,p_node->right);
				}

				printf("key parent left right : %d %d %d %d \n", p->key, p_node->key, p->left->key, p->right->key);			
				// printf("%d 삭제\n", p->key);
				free(p);

				// 자식이 한명 있는가
			} else {
				//자식이 오른쪽인가?
				if(p->left == t->nil){
					heir_node = p->right;	
					heir_node->parent = p_node;
					if(heir_node->parent == t->nil){
						t->root = heir_node;
						free(p);
						t->root->color = RBTREE_BLACK;
						return 0;
					}
					//부모는 왼쪽?
					if(p_node->left == p){
						p_node->left = heir_node;
					//부모는 오른쪽?
					} else {
						p_node->right = heir_node;
					}
					
					if(heir_node->color == RBTREE_RED){
						heir_node->color = RBTREE_BLACK;	
					} else {
						check_double_black(t,heir_node);
					}

					free(p);
				// 자식이 왼쪽인가?
				} else {
					heir_node = p->left;
					heir_node->parent = p_node;
					if(heir_node->parent == t->nil){
						t->root = heir_node;
						free(p);
						t->root->color = RBTREE_BLACK;
						return 0;
					}
					if(p_node->left == p){
						p_node->left = heir_node;
					} else {
						p_node->right = heir_node;
					}

					if(heir_node->color == RBTREE_RED){
						heir_node->color = RBTREE_BLACK;	
					} else {
						check_double_black(t,heir_node);
					}

					// printf("%d 삭제\n", p->key);
					free(p);
				}
			}
		// p의 색깔이 빨간색인가
		} else {
			//p의 자식 없음
			//빨간 노드는 자식이 둘다 있거나 둘다 없거나 두가지 종류만 있음
			if((p->right == t->nil) & (p->left == t->nil)){
				if(p->parent->right == p) {
					p->parent->right = t->nil;
					free(p);
				} else {
					p->parent->left = t->nil;
					free(p);
				}
			}

		}
	}
	t->root->color = RBTREE_BLACK;
	return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
	// 밖에선 퀵 소트를 하지만 여기선 중위순회로 뽑으면 되겠다
	
	size_t cnt = 0;

	inorder(t,arr,n,t->root, &cnt);	
	
  return 0;
}

void inorder(const rbtree *t, key_t *arr, const size_t n, node_t *curnode, size_t *cnt) {
	if(curnode == t->nil) return;

	inorder(t,arr,n,curnode->left,cnt);
	if(*cnt < n) {
		arr[(*cnt)] = curnode->key;
		*cnt += 1;
	} else return;
	inorder(t,arr,n,curnode->right,cnt);
}
