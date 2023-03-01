template <class T>
class List {
    T **p;
    unsigned int cursize, allocsize;
    void Resize(unsigned int required_index);
    List(unsigned int init_size);
    List(const List<T> &ref);
public:
    List() : p(0), cursize(0), allocsize(0) { }
    unsigned int GetSize() const;
    void insert(const T& value, unsigned int index);
    void remove(unsigned int index);
    T& operator[](unsigned int index) const;
    List<T>& operator+(const List<T>& op2) const;
    void operator=(const List<T>& ref);
    ~List();
};


template <class T> List<T>::List(unsigned int init_size)
{
    cursize = 0;
    allocsize = init_size;
    p = new T*[allocsize];
}

template <class T> List<T>::List(const List<T> &ref)
{
    cursize = ref.cursize;
    allocsize = ref.allcosize;
    for(unsigned int i = 0; i < allocsize; ++i) {
        p[i] = ref.p[i];
    }
}

template <class T> void List<T>::Resize(unsigned int required_index)
{
    unsigned int new_size = 10, i;
    if(required_index < allocsize) {
        return;
    }
    while(new_size <= required_index) {
        new_size *= 2;
    }
    T **np = new T*[new_size];
    if(p) {
        for(i = 0; i < cursize; ++i) {
            np[i] = p[i];
        }
        delete [] p;
    }
    p = np;
    allocsize = new_size;
}

template <class T> unsigned int List<T>::GetSize() const {
    return cursize;
}

template <class T> void List<T>::insert(const T& value, unsigned int index) {
    if(index >= cursize) {
        index = cursize;
    }
    ++cursize;
    Resize(cursize);
    for(unsigned int i = cursize - 1; i > index; --i) {
        p[i] = p[i-1];
    }
    p[index] = new T;
    *p[index] = value;
}

template <class T> void List<T>::remove(unsigned int index) {
    unsigned int i;
    if(index >= cursize) {
        throw "";
    }
    delete p[index];
    for(i = index; i + 1 < cursize; ++i) {
        p[i] = p[i+1];
    }
    p[i] = 0;
}

template <class T> T& List<T>::operator[](unsigned int index) const
{
    if(index >= cursize) {
        throw "going outside list";
    }
    if(p[index]) {
        return *p[index];
    } else {
        throw "there is no elemenet with such index";
    }
}

template <class T>
List<T>& List<T>::operator+(const List<T>& op2) const
{
    unsigned int i;
    List new_List(allocsize + op2.allocsize);
    for(i = 0; i < cursize; ++i) {
        new_List.p[i] = p[i];
    }
    for(i = 0; i < op2.cursize; ++i) {
        new_List.p[i+cursize] = op2.p[i];
    }
    return new_List;
}

template <class T>
void List<T>::operator=(const List& ref)
{
    if(p) {
        delete [] p;
    }
    allocsize = ref.allocsize;
    for(unsigned int i = 0; i < cursize; ++i) {
        p[i] = ref.p[i];
    }
}

template <class T>
List<T>::~List()
{
    if(p) {
        for(unsigned int i = 0; i < cursize; ++i) {
            delete p[i];
        }
        delete [] p;
    }
}




