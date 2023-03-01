#include <cstdio>
#include "list.cpp"
#include "sorter.cpp"


int main()
{
    unsigned int i;
    List<int> l;
    printf("ABTA\n");
    //try {
        l.insert(100, 0);
        l.insert(10, 3);
        l.insert(4, 1);
        for(i = 0; i < l.GetSize(); ++i) {
            printf("%d\n", l[i]);
        }
    //}
    //catch(const char *str) {
    //    printf("exception:%s\n", str);
    //}
    return 0;
}
