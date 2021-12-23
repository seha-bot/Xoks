//NEC stands for Non-Euclidean Container
//That is because no matter how many things you store inside, you can always put in more
//This tool sucks and you should use vectors
//WARNING: this tool is experimental and good results are not guaranteed, use vectors instead

#include<stdlib.h>

template<typename T>
class nec
{
private:
    unsigned int superSize = 0;
    unsigned int superOffset = 1;
public:
    T* array;
    unsigned int size = 0;
    
    void Add(T value)
    {
        if(++size >= superSize)
        {
            superSize += superOffset;
            array = (T*)realloc(array, superSize * sizeof(T));
        }
        //std::cout << superSize << "\n";
        array[size - 1] = value;
    }

    void RemoveAt(int index)
    {
        if(index >= 0 && index < size)
        {
            // T *newArray = (T*)malloc(sizeof(T) * --size);
            T *newArray = (T*)malloc(sizeof(T) * superSize);
            size--;

            int offset = 0;
            for(int i = 0; i <= size; i++)
            {
                if(i != index) newArray[i - offset] = array[i];
                else offset++;
            }

            free(array);
            array = newArray;
            // superSize = size;

            if(size < superSize)
            {
                if(superSize - size > superOffset)
                {
                    // superSize = size;
                    superSize -= superOffset;
                    array = (T*)realloc(array, superSize * sizeof(T));
                }
            }
        }
    }

    void Remove(T value)
    {
        for(int i = 0; i < size; i++)
            if(array[i] == value) RemoveAt(i);
    }

    void RemoveAll()
    {
        if(size > 0)
        {
            size = superSize = 0;
            free(array);
            array = (T*)calloc(1, sizeof(T));
        }
    }

    void Resize(unsigned int size)
    {
        array = (T*)calloc(superSize = size, sizeof(T));
    }

    bool Contains(T value)
    {
        for(int i = 0; i < size; i++)
            if(array[i] == value) return true;
        return false;
    }
    
    nec(int speed = 10) { superOffset = speed; array = (T*)calloc(speed, sizeof(T)); }
    ~nec() { free(array); }

    T& operator[](unsigned int index) { return array[index]; }
    nec& operator<<(T value) { Add(value); return *this; }
    nec& operator=(const nec& other)
    {
        array = other.array;
        size = other.size;
        superSize = other.superSize;
        superOffset = other.superOffset;
        return *this;
    }
};