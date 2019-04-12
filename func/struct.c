#include <stdio.h>
#include <stdlib.h>

struct Part {
    enum DAY {CLASS, DIAMCONDS, HEARTS} DAYOF;
    
    enum DAY d1;

    char name[30];
    int age;
};

struct A;

struct B {
    struct A *a1;
};

struct A {
    struct B b1;
};


int main()
{
    struct Part part1;
    part1.d1 = Part::CLASS;
    printf("%d\n", part1.d1);

//    struct Part part1 = {Part::CLASS, "wangwu", 100}, *part2;
//    part2 = &part1;
//    printf("%s %d %d\n", part2->name, part2->age, part2->d1);

    return 0;
}
