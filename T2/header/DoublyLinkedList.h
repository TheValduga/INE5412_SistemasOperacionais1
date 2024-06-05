#ifndef STRUCTURES_DOUBLY_LINKED_LIST_H
#define STRUCTURES_DOUBLY_LINKED_LIST_H

#include <cstdint>  // std::size_t
#include <stdexcept>  // C++ Exceptions


namespace structures {
//!   Classe DoublyLinkedList
    /*!   Classe generaliza estruturas para armazenar.
    */
template<typename T>
class DoublyLinkedList {
 public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    void clear();
    void push_back(const T& data, const T& endereco, const T& tamanho);  // insere no fim
    void push_front(const T& data, const T& endereco, const T& tamanho);  // insere no início
    void insert(const T& data, const T& endereco, const T& tamanho, std::size_t index);  // insere na posição
    T pop(std::size_t index);  // retira da posição
    T pop_back();  // retira do fim
    T pop_front();  // retira do início
    void remove(const T& data);  // retira específico
    bool empty() const;  // lista vazia
    bool contains(const T& data) const;  // contém
    T& at(std::size_t index);  // acesso a um elemento (checando limites)
    const T& at(std::size_t index) const;  // getter constante a um elemento
    T& at_size(std::size_t index);
    const T& at_size(std::size_t index) const; 
    T& at_end(std::size_t index);
    const T& at_end(std::size_t index) const; 
    std::size_t find(const T& data) const;  // posição de um dado
    std::size_t size() const;  // tamanho
    std::size_t firstfit(const T&  tamanho);
    std::size_t bestfit(const T&  tamanho);
    void print();


 private:
    class Node {
     public:
        explicit Node(const T& data, const T& endereco, const T& tamanho) {
            data_ = data;
            endereco_ = endereco;
            tamanho_ = tamanho;
            next_ = nullptr;
            prev_ = nullptr;
        }

        Node(const T& data, const T& endereco, const T& tamanho, Node* next) {
            data_ = data;
            endereco_ = endereco;
            tamanho_ = tamanho;
            next_ = next;
            prev_ = nullptr;
        }

        Node(const T& data, const T& endereco, const T& tamanho, Node* prev, Node* next) {
            data_ = data;
            endereco_ = endereco;
            tamanho_ = tamanho;
            next_ = next;
            prev_ = prev;
        }

        T& data() {
            return data_;
        }

        const T& data() const {
            return data_;
        }

        T& endereco() {
            return endereco_;
        }

        const T& endereco() const {
            return endereco_;
        }

        T& tamanho() {
            return tamanho_;
        }

        const T& tamanho() const {
            return tamanho_;
        }

        Node* prev() {
            return prev_;
        }

        const Node* prev() const {
            return prev_;
        }

        void prev(Node* node) {
            prev_ = node;
        }

        Node* next() {
            return next_;
        }

        const Node* next() const {
            return next_;
        }

        void next(Node* node) {
            next_ = node;
        }

     private:
        T data_;
        T endereco_;
        T tamanho_;
        Node* prev_;
        Node* next_;
    };

    Node* node(std::size_t index) {
        Node* node = head;
        for (auto i = 0; i < size(); i++) {
            if (i == index) {
                return node;
            }
        node = node->next();
        }
        return node;
    }
    Node* head;
    std::size_t size_;
};

}  // namespace structures

template<typename T>
structures::DoublyLinkedList<T>::DoublyLinkedList() {
    head = nullptr;
    size_ = 0;
}

template<typename T>
structures::DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

template<typename T>
void structures::DoublyLinkedList<T>::clear() {
    while (!empty()) {
        pop_front();
    }
}

template<typename T>
void structures::DoublyLinkedList<T>::push_back(const T& data, const T& endereco, const T& tamanho) {
    insert(data, endereco, tamanho, size_);
}

template<typename T>
void structures::DoublyLinkedList<T>::push_front(const T& data, const T& endereco, const T& tamanho) {
    Node* novo = new Node(data, endereco, tamanho, head);
    if (novo == nullptr) {
        throw(std::out_of_range("Lista cheia"));
    } else {
        if (head != nullptr) {
            head->prev(novo);
        }
        head = novo;
        size_++;
    }
}

template<typename T>
void structures::DoublyLinkedList<T>::insert(const T& data, const T& endereco, const T& tamanho, std::size_t index) {
    Node* novo = new Node(data, endereco, tamanho);
    Node* before;
    if (novo == nullptr) {
        delete novo;
        throw(std::out_of_range("Lista Cheia"));
    } else if (index == 0) {
        delete novo;
        push_front(data, endereco, tamanho);
    } else if (index > size_) {
        delete novo;
        throw(std::out_of_range("Indice incorreto"));
    } else {
        before = node(index-1);
        novo->next(before->next());
        before->next(novo);
        novo->prev(before);
        if (novo->next() != nullptr) {
            novo->next()->prev(novo);
        }
        size_++;
    }
}

template<typename T>
T structures::DoublyLinkedList<T>::pop(std::size_t index) {
    Node *sai, *before, *after;
    T dado;
    if (empty()) {
        throw(std::out_of_range("Lista vazia"));
    } else if (index == 0) {
        dado = pop_front();
    } else if (index > size_-1) {
        throw(std::out_of_range("Indice incorreto"));
    } else {
       sai = node(index);
       after = sai->next();
       before = sai->prev();
      if (before != nullptr) {
          before->next(after);
      }
      if (after != nullptr) {
        after->prev(before);
      }
      dado = sai->data();
      delete sai;
      size_--;
    }
    return dado;
}

template<typename T>
T structures::DoublyLinkedList<T>::pop_back() {
    return pop(size_-1);
}

template<typename T>
T structures::DoublyLinkedList<T>::pop_front() {
    Node *sai;
    if (empty()) {
        throw(std::out_of_range("Erro lista vazia"));
    } else {
      sai = head;
      head = sai->next();
      T dado = sai->data();
      delete sai;
      size_--;
      return dado;
      }
}

template<typename T>
void structures::DoublyLinkedList<T>::remove(const T& data) {
    if (empty()) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else {
        std::size_t index = find(data);
        pop(index);
    }
}

template<typename T>
bool structures::DoublyLinkedList<T>::empty() const {  
    return size_ == 0;
}

template<typename T>
bool structures::DoublyLinkedList<T>::contains(const T& data) const {
    if (empty()) {
        throw("lista vazia");
    } else {
        Node *verifica = head;
    for (auto i = 0; i != size_; i++) {
        if (verifica->data() == data) {
            return true;
        }
            verifica = verifica->next();
        }
    return false;
    }
}

template<typename T>
T& structures::DoublyLinkedList<T>::at(std::size_t index) {
    if (index > size_) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
        Node* nodee = node(index);
        return nodee->data();
    }
}

template<typename T>
const T& structures::DoublyLinkedList<T>::at(std::size_t index) const {
    if (index > size_) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
        Node* node = node(index);
        return node->data();
    }
}

template<typename T>
std::size_t structures::DoublyLinkedList<T>::find(const T& data) const {
           if (empty()) {
        throw("lista vazia");
    } else {
        Node *verifica = head;
    for (auto i = 0; i != size_; i++) {
        if (verifica->data() == data) {
            return i;
        }
            verifica = verifica->next();
        }
    }
    return size_;
}

template<typename T>
std::size_t structures::DoublyLinkedList<T>::size() const {  
    return size_;
}

template<typename T>
T& structures::DoublyLinkedList<T>::at_size(std::size_t index) {
    if (index > size_) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
        Node* nodee = node(index);
        return nodee->tamanho();
    }
}

template<typename T>
const T& structures::DoublyLinkedList<T>::at_size(std::size_t index) const {
    if (index > size_) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
        Node* node = node(index);
        return node->tamanho();
    }
}

template<typename T>
T& structures::DoublyLinkedList<T>::at_end(std::size_t index) {
    if (index > size_) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
        Node* nodee = node(index);
        return nodee->endereco();
    }
}

template<typename T>
const T& structures::DoublyLinkedList<T>::at_end(std::size_t index) const {
    if (index > size_) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
        Node* node = node(index);
        return node->endereco();
    }
}

template<typename T>
std::size_t structures::DoublyLinkedList<T>::firstfit(const T&  tamanho) {
    if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
        Node* aux = head;
        int index = 0;
        for (int i = 0; i < size(); i++) {
            if (aux -> data() == -1) {
                if (aux -> tamanho() >= tamanho) {
                    index = i;
                    break;
                }
            }
            aux = aux -> next();
        }
        return index;
    }
}

template<typename T>
std::size_t structures::DoublyLinkedList<T>::bestfit(const T&  tamanho) {
    if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
        Node* aux = head;
        auto index = 0;
        int t = -1;
        int i = 0;
        for (i; i < size(); i++) {
            if (at(i) == -1) {
                if (at_size(i) >= tamanho and t == -1) {
                    index = i;
                    t = at_size(i);
                } else if (at_size(i) >= tamanho and t > 0) {
                    if (at_size(i) < t) {
                        index = i;
                        t = at_size(i);
                    }
                }
            }
            aux = aux -> next();
        }
        if (t == -1) {
            return -1;
        } else {
            return index;
        }
    }
}

template<typename T>
void structures::DoublyLinkedList<T>::print() {
    Node *verifica = head;
    for (auto i = 0; i != size_; i++) {
        cout << "Id: " << verifica->data() << ", endereço: " << verifica->endereco() << ", tamanho:" << verifica->tamanho() << endl;
        verifica = verifica->next();
    }
}


#endif
