#include <iostream>

#include "header/read_file.h"
#include "source/read_file.cpp"



#include "header/DoublyLinkedList.h"
class Bitmap {
public:
    int* bitmap;
    int memoriaSize;
    int blocksize;
    int algoritmo;

    struct allocation {
        int id;
        int start;
        int size;
    };
    
    DoublyLinkedList<Bitmap::allocation> currentAllocs; // Conferir se está importando direito.

    Bitmap(int memSize, int blkSize) {
        memoriaSize = memSize;
        bitmap = new int[memSize/blkSize];
        blocksize = blkSize;
    }

    ~Bitmap() {
        free(bitmap);
    }

    void alocar(int bytes, int id) {
        int sizeAlloc = bytes/blocksize;
        
        int sizeLeft = sizeAlloc;
        int init;

        bool free = false;
        for (int i = 0; i < memoriaSize; i++) {
            if (free) {
                if (bitmap[i] == 0) {
                    sizeLeft -= 1;
                    if (sizeLeft == 0) {
                        break;
                    }
                } else {
                    free = false;
                    sizeLeft = sizeAlloc;
                }
            } else {
                if (bitmap[i] == 0) {
                    sizeLeft -= 1;
                    init = i;
                    if (sizeLeft == 0) {
                        break;
                    }
                    free = true;
                }
            }
        }

        Bitmap::allocation newalloc;
        newalloc.id = id;
        newalloc.start = init;
        newalloc.size = sizeAlloc;
        currentAllocs.push_front(newalloc);
        for (int i = init; i < (init+sizeAlloc); i++) {
            bitmap[i] = 1;
        }
    }

    void Desalocar(int id) {
        Bitmap::allocation pos = currentAllocs.find(id); // Fazer um find de struct
        for (int i = pos.start; i < pos.start+pos.size; i++) {
            bitmap[i] = 0;
        }
        currentAllocs.remove(pos);
    }

    void printMem() {
        for (int i = 0; i < memoriaSize; i++) {
            cout << itoa(bitmap[i]); // checar conversion
        }
    }
};



class LinkedList {
public:
    int memoriaSize;
    int blocksize;
    int algoritmo;

    struct allocation {
        int id;
        int start;
        int size;
    };

    DoublyLinkedList<Bitmap::allocation> segments; // Conferir se está importando direito.

    LinkedList(int memSize, int blkSize) {
        memoriaSize = memSize/blkSize;
        blocksize = blkSize;
        allocation init;
        init.id = 0;
        init.start = 0;
        init.size = memSize;
        segments.push_front(init);
    }

    ~LinkedList() {
    }

    bool alocar(int bytes, int id) {
        int sizeSearch = bytes/blocksize;
        
        // First fit
        allocation *slot = segments.head; // pegar root
        int i = 0;
        while (true) {
            if (slot.data().id == 0 && slot.data().size >= sizeSearch) {
                break;
            }
            slot = slot.next();
            i += 1;
            if (slot == nullptr) {
                return false;
            }
        }

        // Add new allocation
        allocation *newSlot;
        newSlot.data().id = id;
        newSlot.data().start = slot.data().start;
        newSlot.data().size = sizeSearch;

        slot.data().start += sizeSearch;
        slot.data().size -= sizeSearch;

        segments.insert(newSlot, i);

        return true;
    }

    void desalocar(int id)  {
        allocation *slot = segments.head; // pegar root
        int i = 0;
        while (true) {
            if (slot.data().id == id) {
                break;
            }
            slot = slot.next();
            i += 1;
        }

        // remove allocation by case
        allocation *prev = slot.prev();
        allocation *next = slot.next();
        if (prev.data().id == 0) {
            if (next.data().id == 0) {
                prev.data().size += slot.data().size + next.data().size;
                segments.pop(i+1);
                segments.pop(i);
            } else {
                prev.data().size += slot.data().size;
                segments.pop(i);
            }
        } else if (next.data().id == 0) {
            next.data().size += slot.data().size;
            next.data().start = slot.data().start;
            segments.pop(i);
        } else {
            slot.data().id = 0;
        }
    }
}





int main() {
    readFile f;
    f.read_file();
    entrada mydata = f.getMydata();

    //use os prints pra te ajudar a entender como esta a entrada
    /* a entrada esta colocada na forma de 2 vetores, um para os primeiros 4 dados para setar o algoritmo
       e outro que possui os comando ja separados em forma de uma struct (olhe o readfile.h, teremos que
       retirar essas structs em algum momento)*/

    // checagem dos valores de entrada, colocados em mydata.dados em ordem de entrada, que nem no arquivo
    for (int i = 0; i < 4; i++) printf("%d\n", mydata.dados.at(i));
    printf("\n");

    // checagem dos valores dos comandos
    for (int i = 0; i < mydata.comando.size(); i++) {
        printf("%c\n", mydata.comando.at(i).tipo);
        if (mydata.comando.at(i).tipo == 'A') {
            printf("%d\n", mydata.comando.at(i).tamanho);
        }
        printf("%c\n", mydata.comando.at(i).id);
        printf("\n");
    }

    return 0;
}