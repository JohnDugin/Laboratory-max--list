#include "OneList.h"

#include <cassert>

OneList::Node::Node(const ValueType& value, Node* next)
{
    this->value = value;
    this->next = next;
}

OneList::Node::~Node()
{
    // ничего не удаляем, т.к. агрегация
}

void OneList::Node::insertNext(const ValueType& value)
{
    Node* newNode = new Node(value, this->next);
    this->next = newNode;

}

void OneList::Node::removeNext()
{
    Node* removeNode = this->next;
    Node* newNext = removeNode->next;
    delete removeNode;
    this->next = newNext;
}

OneList::OneList()
        : _head(nullptr), _size(0)//просто класс создан здесь
{

}

OneList::OneList(const OneList& copyList)
{
    this->_size = copyList._size;
    if (this->_size == 0) {
        this->_head = nullptr;
        return;
    }

    this->_head = new Node(copyList._head->value);

    Node* currentNode = this->_head;
    Node* currentCopyNode = copyList._head->next;

    while (currentCopyNode) {
        currentNode->next = new Node(currentCopyNode->value);
        currentCopyNode = currentCopyNode->next;
        currentNode = currentNode->next;
    }
}

OneList& OneList::operator=(const OneList& copyList)
{
    if (this == &copyList) {
        return *this;
    }
    OneList bufList(copyList);
    this->_size = bufList._size;
    this->_head = bufList._head;

    return *this;
}

OneList::OneList(OneList&& moveList) noexcept
{
    this->_size = moveList._size;
    this->_head = moveList._head;

    moveList._size = 0;
    moveList._head = nullptr;
}

OneList& OneList::operator=(OneList&& moveList) noexcept
{
    if (this == &moveList) {
        return *this;
    }
    forceNodeDelete(_head);
    this->_size = moveList._size;
    this->_head = moveList._head;

    moveList._size = 0;
    moveList._head = nullptr;

    return *this;
}

OneList::~OneList()
{
    forceNodeDelete(_head);
}

ValueType& OneList::operator[](const size_t pos) const
{
    return getNode(pos)->value;
}

OneList::Node* OneList::getNode(const size_t pos) const
{
    if (pos < 0) {
        assert(pos < 0);
    }
    else if (pos >= this->_size) {
        assert(pos >= this->_size);
    }

    Node* bufNode = this->_head;
    for (int i = 0; i < pos; ++i) {
        bufNode = bufNode->next;
    }

    return bufNode;
}

void OneList::insert(const size_t pos, const ValueType& value)
{

    if (pos < 0) {
        assert(pos < 0);
    }
    else if (pos > this->_size) {
        assert(pos > this->_size);
    }

    if (pos == 0) {
        pushFront(value);
    }
    else {
        Node* bufNode = this->_head;
        for (size_t i = 0; i < pos-1; ++i) {
            bufNode = bufNode->next;
        }
        bufNode->insertNext(value);
        ++_size;
    }
}

void OneList::insertAfterNode(Node* node, const ValueType& value)
{
    node->insertNext(value);
    ++_size;
}

void OneList::pushBack(const ValueType& value)
{
    if (_size == 0) {
        pushFront(value);
        return;
    }
    insert(_size, value);
}

void OneList::pushFront(const ValueType& value)//добавить новый элемент
{
    _head = new Node(value, _head);
    ++_size;
}

void OneList::remove(const size_t pos)
{

    if (pos < 0) {
        assert(pos < 0);
    }
    else if (pos > this->_size) {
        assert(pos > this->_size);
    }

    else if (pos == 0) {
        removeFront();
    }
    else if(pos == _size - 1)
    {
        removeBack();
    }
    else {

        Node *deleteNode = getNode(pos);
        Node *bufNode = getNode(pos + 1);
        getNode(pos - 1)->next = bufNode;
        delete deleteNode;
        --_size;
    }

}
void OneList::removeNode(Node* node)
{
    int index = findIndex(node->value);

    if(index == -1)
    {
        assert(index == -1);
    }

    else if (index == 0) {
        removeFront();
    }
    else if(index == _size - 1)
    {
        removeBack();
    }
    else {


        remove(index);
        --_size;
    }

}

void OneList::removeNextNode(Node* node)
{
    int index = findIndex(node->value);
    remove(index + 1);
}
void OneList::removeBack()
{

    Node *deleteNode = getNode(_size - 1);
    getNode(_size - 2)->next = nullptr;
    delete deleteNode;
    --_size;
}

void OneList::removeFront()
{
    Node *deleteBuf = getNode(0);
    this->_head = getNode(1);
    delete deleteBuf;
    --_size;
}

long long int OneList::findIndex(const ValueType& value) const
{
    Node *node = this->_head;

    for(int i = 0; i < _size; ++i)
    {
        if(node->value == value){
            return i;
        }
        node = node->next;
    }
    return -1;
}

OneList::Node* OneList::findNode(const ValueType& value) const
{
    int index = findIndex(value);
    assert(index != -1);
    return getNode(index);
}

void OneList::reverse()
{
    int index = _size-1;
    long double n;
    for(int i = 0; i <= int((_size-1)/2); ++i){
        n = getNode(i)->value;
        getNode(i)->value = getNode(_size - 1 - i)->value;
        getNode(_size - 1 - i)->value = n;
    }

}

OneList OneList::reverse() const{

    OneList revList;

    Node* bufNode = this->_head;

    for(int i = 0;i < _size;i++){
        revList.pushBack(bufNode->value);
        bufNode = bufNode->next;
    }

    revList.reverse();

    return revList;


}

OneList OneList::getReverseList() const{
    return  this->reverse();
}

size_t OneList::size() const{
    return _size;
}

void OneList::forceNodeDelete(Node* node)
{
    if (node == nullptr) {
        return;
    }

    Node* nextDeleteNode = node->next;
    delete node;
    forceNodeDelete(nextDeleteNode);
}

