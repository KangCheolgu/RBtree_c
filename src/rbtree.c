#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void postorder_delete_node(rbtree *,node_t *);

rbtree *new_rbtree(void) {
  printf("뉴 알비트리 에 들어왔습니다\n");
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  printf("p 선언 \n");
  
  p->nil = (node_t *)calloc(1, sizeof(node_t)); 
  p->root = p->nil; 
  p->nil->color = RBTREE_BLACK;
  printf("root, nil \n");
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
	printf("딜리트에 들어왔습니다.");
	// postorder_delete_node(t,t->root);
	free(t);
}

void postorder_delete_node(rbtree *t, node_t *curNode){
	
	if (curNode->left != t->nil) postorder_delete_node(t, curNode->left);
	if (curNode->right!= t->nil) postorder_delete_node(t, curNode->right);
	free(curNode);
	printf("free curNode %d", curNode->key);
	
}

node_t *rotateLeft(rbtree *t, node_t *node, int checker){

	node_t *p_node = node->parent;
	node_t *gp_node = node->parent->parent;
	// p node는 왼쪽 
	if(checker > 0){
		
		gp_node->left = node;
		node->parent = gp_node;
		
		p_node->right = node->left;
		node->left->parent = p_node;

		p_node->parent = node;
		node->left = p_node;
		
		return p_node;

	// p node는 오른쪽	
	} else {
		p_node->color = RBTREE_BLACK;
		gp_node->color = RBTREE_RED;
		
		p_node->parent = gp_node->parent;

		if (p_node->parent != t->nil){
			gp_node->parent->right = p_node;
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
node_t *rotateRight(rbtree *t, node_t *node, int checker){
	node_t *p_node = node->parent;
	node_t *gp_node = node->parent->parent;
	// pnode 왼쪽
	if(checker > 0){
		p_node->color = RBTREE_BLACK;
		gp_node->color = RBTREE_RED;
		
		p_node->parent = gp_node->parent;

		if (p_node->parent != t->nil){
			gp_node->parent->left = p_node;
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
  printf("%d가 인서트에 들어왔습니다\n", key);
  // TODO: implement insert
  // 맨 처음 들어갈 때는 root 에 key를 저장, 루트의 색을 확인해야함.
  node_t *new_node =  (node_t *)calloc(1, sizeof(node_t));
  
  new_node->color = RBTREE_RED; 
  new_node->key = key;

  node_t *x = t->root;
  node_t *y = t->nil;
  
  // 만약 t 의 루트 노드가 NULL 
  if ( x == t->nil ) {
    printf("루트가 널 일때\n");
    
    t->root = new_node;
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->parent = t->nil;

    // 루트노드는 무조건 블랙이어야 한다.
    new_node->color = RBTREE_BLACK; 

	return new_node;
    // 만약 t가 루트 노드가 아니면
  } else {
	printf("t->root 는 nil이 아닙니다. 현재키 %d\n", new_node->key);
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
		printf("%d 의 left에 %d 삽입\n", y->key, new_node->key);
		y->left = new_node;
	} else {
		printf("%d 의 right에 %d 삽입\n", y->key, new_node->key);
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
	int cnt = 0;
	printf("와일문 들어가기전 커서 확인 : %d\n", cursor->key);
	printf("와일문 들어가기전 커서의 부모 확인 : %d\n", cursor->parent->key);
	printf("와일문 들어가기전 커서의 부모의 색  확인 : %d\n", cursor->parent->color);
	while(cursor->parent->color == RBTREE_RED){
		printf("current cursor key : %d\n", cursor->key);
		node_t *p_node = cursor->parent; 
		node_t *gp_node = cursor->parent->parent;
		cnt += 1;

		if( cnt == 10 ){
			break;
		}
		
		int checker = gp_node->key - p_node->key;
		int checker2 = p_node->key - cursor->key;

		printf("checker %d, checker %d\n",checker, checker2);
		// CASE1 부모와 삼촌이 색깔이 같은가? 빨간색인가
		if((gp_node->left->color == RBTREE_RED) & (gp_node->right->color == RBTREE_RED)){
			printf("case 1\n");
			cursor = change_color(t, cursor);
		}
					
		// CASE 2 삼촌은 검정이고 부모와 나의 방향이 같다.
		// 둘다 left
		else if ((checker > 0) & (checker2 > 0) & (gp_node->right->color == RBTREE_BLACK)){
			printf("case 2-1\n");
			cursor = rotateRight(t, cursor, checker);
		}

		// 둘다 right
		else if ((checker < 0) & (checker2 < 0) & (gp_node->left->color == RBTREE_BLACK)){
			printf("case 2-2\n");
			cursor = rotateLeft(t, cursor, checker);
		}

		// CASE 3 부모와 자식의 방향이 다를경우
		// 부모 left 자식 right
		else if ((checker > 0) & (checker2 < 0) & (gp_node->right->color == RBTREE_BLACK)){
			printf("case 3-1\n");
			cursor = rotateLeft(t, cursor, checker);
		}

		// 부모 right 자식 left
		else if ((checker < 0) & (checker2 > 0) & (gp_node->left->color == RBTREE_BLACK)){
			printf("case 3-2\n");
			cursor = rotateRight(t, cursor, checker);
		}
		t->root->color = RBTREE_BLACK;
	}
	return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    printf("파인드에 들어왔습니다.:\n");
  // TODO: implement find
	node_t *x = t->root;

	while(x != t->nil){
		if( x->key == key ){
			return x;
		}
		else if (x->key < key){
			x = x->right;
		}
		else {
			x = x->left;
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
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
	// 삭제되는 애가 빨간색이면 변화 없음. 검정색일 경우 조건을 따져야 한다.

	// 처음 확인 해야 하는 조건. 들어온 노드가 양쪽 다 자식을 가지고 있나?
	// 자식이 둘다 있을 경우 석세서를 찾아서 걔의 키값을 p에 복사하고 석세서를 삭제한다.
	// 석세서를 삭제
	if ( p->left != t->nil & p->right != t->nil) {
		node_t *search_successor = p->right;
		node_t *pre_successor = t->nil;
		while (search_successor->left != t->nil) {
			search_successor = search_successor->left;
		}
		if (search_successor->color == RBTREE_BLACK) {

		} else {
			p->key = search_successor->key;
			
		}
		
	// 자식이 둘 중에 하나라도 없으면 p를 삭제하고 대체할 놈을 찾는다.
	// p를 삭제
	} else {
		
	}
	return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

