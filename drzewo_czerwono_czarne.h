#include "Node.h"
#include <iostream>
#include <cmath>
template <typename T>
class Tree{
private:
    Node<T>* root;
    int size;

    void preOrder(Node<T>*,std::string(*toString)(T),std::string&);
    void inOrder(Node<T>*,std::string(*toString)(T),std::string&);
    void clear(Node<T>*);
    int height(Node<T>*);
    void insertBalance(Node<T>*);
    void preOrderAll(Node<T>*,std::string(*toString)(T),std::string&,int&);
    void rotateR(Node<T>*);
    void rotateL(Node<T>*);
public:
    Tree();
    Node<T>* find(T,int(*comp)(T,T));
    std::string preOrder(std::string(*toString)(T));
    std::string inOrder(std::string(*toString)(T));
    void clear();
    int height();
    void insert(T,bool(*compare)(T,T));
    std::string to_string(std::string(*toString)(T));
};

// Konstruktor domyslny
template <typename T>
Tree<T>::Tree(){
    root = nullptr;
    size = 0;
}
// Znajdywanie elementu
template <typename T>
Node<T>* Tree<T>::find(T element,int(*comp)(T,T)){
    Node<T>* temp = new Node<T>(element,size);
    Node<T>* current = root;
    while(current != nullptr){
        if(comp(temp->data,current->data) < 0){
            current = current->left;
        }
        else if(comp(temp->data,current->data) > 0){
            current = current->right;
        }
        else if(comp(temp->data,current->data)==0){
            // Znaleziono element
            return current;
        }
    }
    // Czyszczenie pamieci
    delete temp;
    return current;
}
// Prywatne preOrder(wywolywane przez publiczne)
template <typename T>
void Tree<T>::preOrder(Node<T> *wsk, std::string (*toString)(T), std::string &text) {
    if(wsk == nullptr){
        return;
    }
    text += toString(wsk->data);
    text += " ";
    preOrder(wsk->left,toString,text);
    preOrder(wsk->rigt,toString,text);
}

// Publiczne preOrder
template <typename T>
std::string Tree<T>::preOrder(std::string(*toString)(T)){
    std::string text = "Przechodze drzewo metoda pre-order: \n";
    preOrder(root,toString,text);
    text += "\n";
    return text;
}

// Prywatne inOrder(wywolywane przez publiczne)
template <typename T>
void Tree<T>::inOrder(Node<T> *wsk, std::string (*toString)(T), std::string &text) {
    if(root == nullptr){
        return;
        // Do wytestowania!
    }
    preOrder(wsk->left,toString,text);
    text += toString(wsk->data);
    text += " ";
    preOrder(wsk->rigt,toString,text);
}

// Publiczne inOrder
template <typename T>
std::string Tree<T>::inOrder(std::string(*toString)(T)){
    if(root == nullptr){
        return "Drzewo puste!";
    }
    std::string text = "Przechodze drzewo metoda in-order: \n";
    preOrder(root,toString,text);
    text += "\n";
    return text;
}

// Prywatne czysczenie drzewa(wywolywane przez publiczne)
template <typename T>
void Tree<T>::clear(Node<T>* wsk){
    if(wsk == nullptr){
        return;
    }
    clear(wsk->left);
    clear(wsk->right);
    delete wsk;
}

// Publiczne clear
template <typename T>
void Tree<T>::clear(){
    clear(root);
    size = 0;
    root = nullptr;
}

// Prywatne height (wywolywane przez publiczne)
template <typename T>
int Tree<T>::height(Node<T>* wsk){
    if(wsk == nullptr){
        return -1;
    }
    return std::max(height(wsk->left),height(wsk->right))+1;
}

// Publiczne height
template <typename T>
int Tree<T>::height(){
    if(root == nullptr) return 0;
    return height(root);
}

// Rotacja w prawo
template<typename T>
void Tree<T>::rotateR(Node<T> *wsk){
    Node<T>* temp = wsk->left;
    wsk->left = temp->right;
    if(temp->right != nullptr){
        temp->right->parent = wsk;
    }
    temp->parent = wsk->parent;
    if(wsk->parent == nullptr){
        this->root = temp;
    }
    else if(wsk == wsk->parent->left){
        wsk->parent->left = temp;
    }
    else{
        wsk->parent->right = temp;
    }
    temp->right = wsk;
    wsk->parent = temp;
}
// Rotacja w lewo
template<typename T>
void Tree<T>::rotateL(Node<T> *wsk) {
    Node<T>* temp = wsk->right;
    wsk->right = temp->left;
    if(temp->left != nullptr){
        temp->left->parent = wsk;
    }
    temp->parent = wsk->parent;
    if(wsk->parent == nullptr){
        this->root = temp;
    }
    else if(wsk == wsk->parent->left){
        wsk->parent->left = temp;
    }
    else{
        wsk->parent->right = temp;
    }
    temp->left = wsk;
    wsk->parent = temp;
}
// Dodawanie elementu do drzewa
template <typename T>
void Tree<T>::insert(T value, bool (*compare)(T, T)) {
    Node<T>*temp = new Node<T>(value,size);
    if(root == nullptr){
        temp->is_red = false;
        root = temp;
    }
    else{
        Node<T>* current = root;
        Node<T>*prev = current;
        while(current != nullptr){
            prev = current;
            if(compare(temp->data,current->data)){
                current = current->right;
            }
            else{
                current = current->left;
            }
        }
        if(compare(temp->data,prev->data)){
            prev->right = temp;
            prev->right->parent = prev;
        }
        else{
            prev->left = temp;
            prev->left->parent = prev;
        }
    }
    size++;
    if(size > 1) {
        insertBalance(temp);
    }
}

// Naprawa drezewa po dodaniu elementu
template<typename T>
void Tree<T>::insertBalance(Node<T> *wsk) {
    Node<T>*parent_wsk = nullptr;
    Node<T>* grand_parent_wsk = nullptr;
    while((wsk != root) && (wsk->is_red != false) && (wsk->parent->is_red == true)){
        parent_wsk = wsk->parent;
        grand_parent_wsk = wsk->parent->parent;

        if(parent_wsk == grand_parent_wsk->left){
            // Wujek jest prawym
            Node<T>* uncle_wsk = grand_parent_wsk->right;
            // Przypadek 1 wujek jest czerwony
            // Wystarczy tylko przekolorowanie
            if(uncle_wsk != nullptr && uncle_wsk->is_red == true){
                grand_parent_wsk->is_red = true;
                parent_wsk->is_red = false;
                uncle_wsk->is_red = false;
                wsk = grand_parent_wsk;
            }

            else{
                // Przypadek 2 zygzak (nieprosta linia)
                if(wsk == parent_wsk->right){
                    // Ustawiamy w lini
                    rotateL(parent_wsk);
                    wsk = parent_wsk;
                    parent_wsk = wsk->parent;
                }
                // Przypadek 3 prosta linia
                rotateR(grand_parent_wsk);
                // Zamiana kolorow ojca i dziadka
                bool tempColor = parent_wsk->is_red;
                parent_wsk->is_red = grand_parent_wsk->is_red;
                grand_parent_wsk->is_red = tempColor;
                wsk = parent_wsk;
            }
        }
        else{
            Node<T>* uncle_wsk = grand_parent_wsk->left;

            if(uncle_wsk != nullptr && uncle_wsk->is_red == true){
                grand_parent_wsk->is_red = true;
                parent_wsk->is_red = false;
                uncle_wsk->is_red = false;
                wsk = grand_parent_wsk;
            }
            else{
                if(wsk == parent_wsk->left){
                    rotateR(parent_wsk);
                    wsk = parent_wsk;
                    parent_wsk = wsk->parent;
                }
                rotateL(grand_parent_wsk);
                bool tempColor = parent_wsk->is_red;
                parent_wsk->is_red = grand_parent_wsk->is_red;
                grand_parent_wsk->is_red = tempColor;
                wsk = parent_wsk;
            }
        }
    }
    this->root->is_red = false;
}

// Wypisywanie drzewa
template<typename T>
std::string Tree<T>::to_string(std::string (*toString)(T)) {
    std::string text = "";
    text += "Size: ";
    text += std::to_string(size);
    text += "\nLog2(size): ";
    text += std::to_string(log2(size));
    text += "\n height/log2n: ";
    text += std::to_string(height()/log2(size));
    text += "\nHeight: ";
    text += std::to_string(height());
    text += "\n";
    int howMuch;
    if(size > 5){
        howMuch = 5;
    }
    else howMuch = size;
    preOrderAll(root,toString,text,howMuch);
    return text;
}

// Wypisywanie elementu //
template<typename T>
void Tree<T>::preOrderAll(Node<T> *wsk, std::string (*toString)(T),std::string &text,int &howMuch) {
    if(wsk == nullptr){
        return;
    }
    --howMuch;
    text += std::to_string(wsk->id);
    text += " Data: ";
    text += toString(wsk->data);
    text += " R: ";
    if(wsk->right == nullptr) text += "NULL";
    else {
        text += std::to_string(wsk->right->id);
    }
    text += " L: ";
    if(wsk->left == nullptr) text += "NULL";
    else {
        text += std::to_string(wsk->left->id);
    }
    text += " P: ";
    if(wsk->parent == nullptr) text += "NULL";
    else {
        text += std::to_string(wsk->parent->id);
    }
    text += " Color: ";
    if(wsk->is_red == true) text += "RED";
    else text+= "BLACK";
    text += "\n";
    if(howMuch > 0) preOrderAll(wsk->left,toString,text,howMuch);
    if(howMuch > 0) preOrderAll(wsk->right,toString,text,howMuch);
}