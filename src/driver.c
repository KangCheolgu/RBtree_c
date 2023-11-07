#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void print_rbtree_node(const node_t *, const node_t *);
void print_rbtree(const rbtree *);

int main(int argc, char *argv[])
{
    // 새로운 레드-블랙 트리 생성
    rbtree *tree = new_rbtree();

    // 커맨드 라인에서 제공된 각 키를 트리에 삽입
    for (int i = 1; i < argc; ++i)
    {
        key_t key = atoi(argv[i]); // 문자열을 숫자로 변환
        rbtree_insert(tree, key);
    }

    // 메인 루프
    int choice, key;
    while (1)
    {
		printf("\n\n");
        printf("Select\n1. Insert\n2. Erase\n3. Print\n-1. Quit\n");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: // 삽입
				while(1){
					printf("Input_Key_Value: ");
					printf(">> ");
					scanf("%d", &key);
					if (key == -1){
						break;
					}
					rbtree_insert(tree, key);
				}
                break;

            case 2: // 삭제
    			printf(">>\n");
                printf("Delete_Key_Value: ");
                scanf("%d", &key);
                node_t *f_node = rbtree_find(tree, key);
                if (f_node != tree->nil) 
                    rbtree_erase(tree, f_node);
				else 
				{
    				printf(">>\n");
                    printf("Key가 트리에 없습니다.\n");
                }
                break;

            case 3: // 출력
                print_rbtree(tree);
                break;

            case -1: // 종료
                goto exit_loop;

            default:
    			printf(">>\n");
                printf("잘못된 선택입니다.\n");
        }
    }

exit_loop:
    // 레드-블랙 트리 삭제 및 메모리 정리
    delete_rbtree(tree);
    return 0;
}

void print_rbtree_node(const node_t *node, const node_t *nil)
{
    if (node == nil)
        return;

    // 왼쪽 서브트리 출력
    print_rbtree_node(node->left, nil);
    // 현재 노드 출력
    printf("%d (%s)\n", node->key, node->color == RBTREE_RED ? "RED" : "BLACK");
    // 오른쪽 서브트리 출력
    print_rbtree_node(node->right, nil);
}

void print_rbtree(const rbtree *t)
{
	printf("\n");
    printf("==============================================\n");
	printf("In_Order_Print\n");
    // 루트 노드부터 시작하여 트리 출력
    print_rbtree_node(t->root, t->nil);
    printf("==============================================\n");
	printf("\n");
}
