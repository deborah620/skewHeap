// CMSC 341 - Spring 2021 - Project 3
// RQueue: 

#include "rqueue.h"
using namespace std;
using std::cout;
using std::endl;
using std::domain_error;

RQueue::RQueue(prifn_t priFn)
{
  /************************
   * To be implemented
   * *********************/
   priority = priFn;
   _size = 0;
   _heap = nullptr;
}

RQueue::~RQueue()
{
  /************************
   * To be implemented
   * *********************/
   clear();

   _heap = nullptr;
   _size = 0;
   priority = nullptr;
}

RQueue::RQueue(const RQueue& rhs)
{
  /************************
   * To be implemented
   * *********************/
   _size = rhs._size;
   priority = rhs.priority;

   //copy value of rhs heap
   if (rhs._heap) {
       _heap = new Node(rhs._heap->_student);
       copyHelper(_heap, rhs._heap);
   }
   else
       _heap = nullptr;
}

Node* RQueue::copyHelper(Node* node, Node* copy) {
    //copy left-hand side
    if (copy->_left) {
        node->_left = new Node(copy->_left->_student);
        copyHelper(node->_left, copy->_left);
    }
    
    //copy right-hand side
    if (copy->_right) {
        node->_right = new Node(copy->_right->_student);
        copyHelper(node->_right, copy->_right);
    }

    return node;
}


RQueue& RQueue::operator=(const RQueue& rhs)
{
  /************************
   * To be implemented
   * *********************/
    if (this != &rhs){
        clear();

	_heap = nullptr;
        _size = rhs._size;
        priority = rhs.priority;

	//copy heap
        if (rhs._heap) {
            _heap = new Node(rhs._heap->_student);
            assignmentHelper(_heap, rhs._heap);
        }
        else
            _heap = nullptr;

    }return *this;

}

Node* RQueue::assignmentHelper(Node* node, Node* copy) {
    //copy left
    if (copy->_left) {
        node->_left = new Node(copy->_left->_student);
        assignmentHelper(node->_left, copy->_left);
    }

    //copy right
    if (copy->_right) {
        node->_right = new Node(copy->_right->_student);
        assignmentHelper(node->_right, copy->_right);
    }

    return node;
}

void RQueue::insertStudent(const Student& input) {
  /************************
   * To be implemented
   * *********************/

   //creating first node
   if (!_heap)
       _heap = new Node(input);

   else {     
       Node *newNode = new Node(input);
       insertHelper(_heap, newNode);
   }
   _size++;
}

Node *RQueue::insertHelper(Node *p1, Node *p2) {
    Node* temp;

    if (!p1)
        return p2;
    if (!p2)
        return p1;

    //making sure p1 has greater priority (smaller number)
    if (priority(p1->getStudent()) > priority(p2->getStudent())){
        Node* tempP2 = p2;
        p2 = p1;

        //if swapping the root
        if (p1 == _heap)
            _heap = tempP2;

        p1 = tempP2;
    }
    //swap children
    Node* tempRightSub = p1->_right;
    p1->_right = p1->_left;
    p1->_left = tempRightSub;

    //left subtree of p1 is the result of recursion
    temp = insertHelper(p2, p1->_left);
    p1->_left = temp;

    return p1;
}

Student RQueue::getNextStudent() {
  /************************
   * To be implemented
   * *********************/

  //throw error is dequeue empty heap
   if (!_heap)
       throw domain_error("heap is empty");

   Node* rootNode = _heap;

   //remove last node
   if (!_heap->_left && !_heap->_right) {
      Student rootStu = _heap->getStudent();
       _size--;
       delete _heap;
       _heap = nullptr;
       
       return rootStu;
   }

   //go down left side
   if (!_heap->_right || priority(_heap->_left->getStudent()) <= priority(_heap->_right->getStudent())) {
       _heap = _heap->_left;
       mergeHelper(_heap, rootNode->_right);
   }
   //do down right side
   else {
       _heap = _heap->_right;
       mergeHelper(_heap, rootNode->_left);
   }
  
   _size--;

   delete rootNode;
   rootNode = nullptr;
   
   return _heap->getStudent();
}


void RQueue::mergeWithQueue(RQueue& rhs) {
  /************************
   * To be implemented
   * *********************/

   //protect against self-assignment
   if (this != &rhs) {
       //merging with different priorities
       if (priority != rhs.priority)
           throw domain_error("different priorities");

       mergeHelper(_heap, rhs._heap);

       _size += rhs._size;

       //empty rhs
       rhs._heap = nullptr;
       rhs._size = 0;
       rhs.priority = nullptr;
   }

}
Node* RQueue::mergeHelper(Node* p1, Node* p2) {
    Node* temp;

   if (!p1)
       return p2;
   if (!p2)
       return p1;
   
   //change priority if needed
   if (priority(p1->getStudent()) > priority(p2->getStudent())){
       Node* tempP2 = p2;

       //if swapping the root
       if (p1 == _heap)
	 _heap = tempP2;
       
       p2 = p1;
       p1 = tempP2;
   }
   //swap children of p1
    Node* tempRightSub = p1->_right;
    p1->_right = p1->_left;
    p1->_left = tempRightSub;

    //recursively merge p2 and the left subtree of p1
    temp = mergeHelper(p2, p1->_left);

    //replace the let subtree of p1 with the result of recursion
    p1->_left = temp;

    return p1;
}

void RQueue::clear() {
  /************************
   * To be implemented
   * *********************/
   if (_heap){
        clearHelper(_heap);
   }
}

void RQueue::clearHelper(Node* node) {
    if (!node)
        return;

    //clear left
    clearHelper(node->_left);

    //clear right
    clearHelper(node->_right);

    delete node;
    node = nullptr;
}

int RQueue::numStudents() const
{
  /************************
   * To be implemented
   * *********************/
    return _size;
}

void RQueue::printStudentQueue() const {
  /************************
   * To be implemented
   * *********************/
   if (_heap){
       cout << "Contents of the student queue:" << endl;

       printHelper(_heap);
   }
}

void RQueue::printHelper(Node* nodeToPrint) const {
    //preorder traversal, root -> left -> right
    if (!nodeToPrint)
        return;

    cout << "[" << priority(nodeToPrint->_student) << "] " << nodeToPrint->_student << endl;

    printHelper(nodeToPrint->_left);

    printHelper(nodeToPrint->_right);
}

prifn_t RQueue::getPriorityFn() const {
  /************************
   * To be implemented
   * *********************/
    return priority;
}

void RQueue::setPriorityFn(prifn_t priFn) {
  /************************
   * To be implemented
   * *********************/
    priority = priFn;
    
    //remake the tree using an array
    Node** treeArray = new Node*[_size];
    int i = 0;

    array(_heap, treeArray, i);

    //reconstruct heap
    _heap = treeArray[0];
    
    for (int j = 0; j < _size - 1; j++) {
        insertHelper(_heap, treeArray[j + 1]);
    }

    delete [] treeArray;
}

void RQueue::array(Node*& node, Node* treeArray[], int &i) {
    if (!node) {
        return;
    }

    //put left nodes in array
    array(node->_left, treeArray, i);

    treeArray[i] = node;
    i++;

    //put right nodes in array
    array(node->_right, treeArray, i);

    node->_right = nullptr;
    node->_left = nullptr;
}

// for debugging
void RQueue::dump() const
{
  if (_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(_heap);
    cout << endl;
  }
}

// for debugging
void RQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->_left);
    cout << priority(pos->_student) << ":" << pos->_student.getName();
    dump(pos->_right);
    cout << ")";
  }
}

// overloaded insertion operator for Student class
ostream& operator<<(ostream& sout, const Student& student) {
  sout << "Student: " << student.getName() << ", priority: " << student.getPriority()
       << ", year: " << student.getYear() << ", major: " << student.getMajorStr() 
       << ", group: " << student.getGroupStr();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getStudent();
  return sout;
}
