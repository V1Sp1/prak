template <class Item, class T>
class Sorter {
    static void bubble_sort(T& obj);
    static void insertion_sort(T& obj);
    static void selection_sort(T& obj);
};

template <class Item, class T>
void Sorter<Item, T>::bubble_sort(T& obj)
{
    Item c;
    unsigned int i, j;
    unsigned int len = obj.GetSize();
    for(i = 0; i < len; i++){
        for(j = len - 1; j > i; j--){
            if(obj[j-1] > obj[j]){
                c = obj[j-1];
                obj[j-1] = obj[j];
                obj[j] = c;
            }
        }
    }
}

template <class Item, class T>
void Sorter<Item, T>::insertion_sort(T& obj)
{
    Item c;
    unsigned int i, j;
    unsigned int len = obj.GetSize();
    for(i = 1; i < len; i++){
        c = obj[i];
        for(j = i; (j > 0) && (obj[j-1] > c); j--){
            obj[j] = obj[j - 1];
        }
        obj[j] = c;
    }
}

template <class Item, class T>
void Sorter<Item, T>::selection_sort(T& obj)
{
    Item c;
    unsigned int i, j, mini;
    unsigned int len = obj.GetSize();
    for(i = 0; i < len - 1; i++){
        mini = i;
        for(j = i + 1; j < len; j++){
            if(obj[mini] > obj[j]){
                mini = j;
            }
        }
        if(mini != i){
            c = obj[i];
            obj[i] = obj[mini];
            obj[mini] = c;
        }
    }

}
