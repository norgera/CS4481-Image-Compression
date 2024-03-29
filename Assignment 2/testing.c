#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


struct test{
	int x;
	int y;
};

void other_fn(struct test** t){
	printf("\n");
	for(int i = 0; i < 300; i++){
		printf("%d %d\n", t[i]->x, t[i]->y);
	}
}

void test_fn(struct test** t){
	*t = malloc(sizeof(struct test*) * 300);
	for(int i = 0; i < 300; i++){
		t[i] = malloc(sizeof(struct test));
		t[i]->x = 1;
		t[i]->y = 2;
	}

	printf("\n");
	for(int i = 0; i < 300; i++){
		printf("%d %d\n", t[i]->x, t[i]->y);
	}
}

int main(){
	struct test** test_node = malloc(sizeof(struct test**));
	test_fn(test_node);

	printf("\n");
	for(int i = 0; i < 300; i++){
		printf("%d %d\n", test_node[i]->x, test_node[i]->y);
	}

	other_fn(test_node);
	return 0;
}