#ifndef STRUCTURES_LINKED_LIST_H
#define STRUCTURES_LINKED_LIST_H

#include <cstdint>
#include <stdexcept>


namespace structures {
//! Class LinkedList
    /* Classe emplementa a função de cadeia de ponteiros a lista
    */
template<typename T>
class LinkedList {
 public:
    LinkedList();  //  construtor padrão
    ~LinkedList();  //  destrutor
    void clear();  //  limpar lista
    void push_back(const T& data, const T& endereco, const T& tamanho);  //  inserir no fim
    void push_front(const T& data, const T& endereco, const T& tamanho);  //  inserir no início
    void insert(const T& data, const T& endereco, const T& tamanho, std::size_t index);  //  inserir na posição
    T& at(std::size_t index);  //  acessar um elemento na posição index
    T& at_end(std::size_t index);
    T& at_size(std::size_t index);
    T pop(std::size_t index);  //  retirar da posição
    T pop_back();  //  retirar do fim
    T pop_front();  //  retirar do início
    void remove(const T& data);  //  remover específico
    bool empty() const;  //  lista vazia
    bool contains(const T& data) const;  //  contém
    std::size_t find(const T& data) const;  //  posição do dado
    std::size_t size() const;  //  tamanho da lista
    void print();

 private:
    class Node {  //  Elemento
     public:
        explicit Node(const T& data, const T& endereco, const T&tamanho):
            data_{data},
            endereco_{endereco},
            tamanho_{tamanho}
        {}

        Node(const T& data, Node* next):
            data_{data},
            endereco_{endereco},
            tamanho_{tamanho},
            next_{next}
        {}

        T& data() {  //  getter: dado
            return data_;
        }

        const T& data() const {  //  getter const: dado
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

        Node* next() {  //  getter: próximo
            return next_;
        }

        const Node* next() const {  //  getter const: próximo
            return next_;
        }

        void next(Node* node) {  //  setter: próximo
            next_ = node;
        }

     private:
        T data_;
        T endereco_;
        T tamanho_;
        Node* next_{nullptr};
    };

    Node* end() {  //  último nodo da lista
        auto it = head;
        for (auto i = 1u; i < size(); ++i) {
            it = it->next();
        }
        return it;
    }

    Node* current(std::size_t index) {  // retorna node com idice
        auto it = head;
        if (index != 0) {
            for (auto i = 1; i < size(); ++i) {
            if (i == index)
                return it;
            it = it->next();
            }
        }
        return it;
    }

    Node* head{nullptr};
    std::size_t size_{0u};
};

}  // namespace structures

template<typename T>
structures::LinkedList<T>::LinkedList() {
}

template<typename T>
structures::LinkedList<T>::~LinkedList() {
    clear();
}

template<typename T>
void structures::LinkedList<T>::clear() {
    while (!empty()) {
        pop_front();
    }
}

template<typename T>
void structures::LinkedList<T>::push_back(const T& data, const T& endereco, const T& tamanho) {
    insert(data, endereco, tamanho, size_);
}

template<typename T>
void structures::LinkedList<T>::push_front(const T& data, const T& endereco, const T& tamanho) {
    Node* novo = new Node(data, endereco, tamanho);
    if ( novo == nullptr ) {
        delete novo;
        throw(std::out_of_range("Erro lista cheia"));
    } else {
        novo->next(head);
        head = novo;
        size_++;
    }
}

template<typename T>
void structures::LinkedList<T>::insert(const T& data, const T& endereco, const T& tamanho, std::size_t index) {
    Node* novo = new Node(data, endereco, tamanho);
    Node* before;
    if ( novo == nullptr ) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (index > size_) {
        delete novo;
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (index == 0) {
        delete novo;
        push_front(data, endereco, tamanho);
    } else {
        before = current(index);
        novo->next(before->next());
        before->next(novo);
        size_++;
    }
}

template<typename T>
T& structures::LinkedList<T>::at(std::size_t index) {
    if (index > size_) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
          Node* node = head;
          for (auto i = 0u; i != index; i++) {
              node = node->next();
          }
        // node = current(index+1);
        return node->data();
    }
}

template<typename T>
T& structures::LinkedList<T>::at_end(std::size_t index) {
    if (index > size_) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
          Node* node = head;
          for (auto i = 0u; i != index; i++) {
              node = node->next();
          }
        // node = current(index+1);
        return node->endereco();
    }
}

template<typename T>
T& structures::LinkedList<T>::at_size(std::size_t index) {
    if (index > size_) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else if (empty()) {
        throw(std::out_of_range("Erro Lista vazia"));
    } else {
          Node* node = head;
          for (auto i = 0u; i != index; i++) {
              node = node->next();
          }
        // node = current(index+1);
        return node->tamanho();
    }
}

template<typename T>
T structures::LinkedList<T>::pop(std::size_t index) {
  Node *sai, *before;
  T dado;
  if (empty()) {
    throw("lista vazia");
    } else if (index < 0 || index > size_ -1) {
       throw(std::out_of_range("Erro Lista Cheia"));
    } else if (index == 0) {
      pop_front();
    } else {
    before = current(index);
    sai = before->next();
    before->next(sai->next());
    dado = sai->data();
    delete sai;
    size_--;
  }
    return dado;
}

template<typename T>
T structures::LinkedList<T>::pop_back() {
  Node *sai;
  if (empty()) {
    throw(std::out_of_range("Lista vazia"));
  } else {
    sai = end();
    T dado = sai->data();
    delete sai;
    size_--;
    return dado;
  }
}

template<typename T>
T structures::LinkedList<T>::pop_front() {
  Node *sai;
  if (empty()) {
    throw(std::out_of_range("Erro lista vazia"));
  } else {
    sai = head;
    head = head->next();
    T dado = sai->data();
    size_--;
    delete sai;
    return dado;
  }
}

template<typename T>
void structures::LinkedList<T>::remove(const T& data) {
    if (empty()) {
        throw(std::out_of_range("Erro Lista Cheia"));
    } else {
        std::size_t index = find(data);
        pop(index);
    }
}

template<typename T>
bool structures::LinkedList<T>::empty() const {
  return size_ == 0;
}

template<typename T>
bool structures::LinkedList<T>::contains(const T& data) const {
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
std::size_t structures::LinkedList<T>::find(const T& data) const {
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
std::size_t structures::LinkedList<T>::size() const {
  return size_;
}

template<typename T>
void structures::LinkedList<T>::print() {
    Node *verifica = head;
    for (auto i = 0; i != size_; i++) {
        cout << "Id: " << verifica->data() << ", endereço: " << verifica->endereco() << ", tamanho:" << verifica->tamanho() << endl;
        verifica = verifica->next();
    }
}

#endif