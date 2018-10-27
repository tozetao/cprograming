#include <stdio.h>
#include <stdlib.h>

void base_demo() 
{
    int matrix[3][10] = {
        {1, 2, 3},
        {4, 5, 7},
        {11, 12, 13, 14, 15}
    };

    int *m = &(*(*(matrix + 1) + 1));
    int *s = (*(matrix + 1) + 1);

    printf("m = %d\n", *m);
    printf("s = %d\n", *s);
    
    m--;
    s++;

    printf("m = %d\n", *m);
    printf("s = %d\n", *s);
}

// 多维数组名的测试
void array_name_demo()
{
    printf("example2: \n");

    int matrix[3][10] = {
        {1, 2, 3},
        {4, 5, 7},
        {11, 12, 13, 14, 15}
    };

    int (*mp)[10] = matrix;
    // 数组名指向第一个元素的指针，类型取决于元素类型，因此matrix是一个数组指针

    int *sp = *matrix;
    // 对matrix进行间接引用则获取到的第一个元素的值，它是一个数组，因此表达式的值是一个常量指针

    printf("%d\n", *sp);
    printf("%d\n", *(sp+2));

    printf("%d\n", mp[0][2]);   //输出2
    printf("%d\n", *(*mp + 2));

    printf("%d\n", *mp[2]);     //输出11
    printf("%d\n", **(mp + 2));
}