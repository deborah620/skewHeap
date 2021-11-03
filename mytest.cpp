// CMSC 341 - Spring 2021 - Project 3
// driver.cpp: sample driver for the RQueue class.

#include "rqueue.h"
using namespace std;

class Tester{
public:
    bool testCopy(RQueue&, RQueue&);

    bool testCopyHelper(Node *lhs, Node *rhs);

    bool testAssign(RQueue &lhs, RQueue &rhs);

    bool testAssignHelper(Node *lhs, Node *rhs);

    bool testPriority(RQueue &queue);

    bool testPriorityHelper(Node *node, prifn_t priority);

    bool testPriorityDomainError(RQueue &priorOne, RQueue &priorTwo);

    bool testDequeueDomainError(RQueue &queue);

    void InsertPerformance(int numTrials, int N, RQueue &queue);

    void ExtractPerformance(int numTrials, int N, RQueue &queue);

};

bool Tester::testCopy(RQueue& lhs, RQueue& rhs) {
    if (!testCopyHelper(lhs._heap, rhs._heap))
        return false;

    return true;
}

bool Tester::testCopyHelper(Node* lhs, Node* rhs) {
    bool deepCopy = true;

    //making sure pointers are difft but values are the same
    if (lhs == rhs || lhs->_student.getPriority() != rhs->_student.getPriority())
        return false;

    //left side
    if (rhs->_left)
        deepCopy = testCopyHelper(lhs->_left, rhs->_left);

    //right side
    if (rhs->_right)
        deepCopy = testCopyHelper(lhs->_right, rhs->_right);

    return deepCopy;
}

bool Tester::testAssign(RQueue &lhs, RQueue &rhs) {
    if (!testAssignHelper(lhs._heap, rhs._heap))
        return false;

    return true;
}

bool Tester::testAssignHelper(Node* lhs, Node* rhs) {
    bool deepCopy = true;

    //making sure pointers are difft but values are the same
    if (lhs == rhs || lhs->_student.getPriority() != rhs->_student.getPriority())
        return false;

    if (rhs->_left)
        deepCopy = testAssignHelper(lhs->_left, rhs->_left);

    if (rhs->_right)
        deepCopy = testAssignHelper(lhs->_right, rhs->_right);

    return deepCopy;
}

bool Tester::testPriority(RQueue &queue) {
  if (testPriorityHelper(queue._heap, queue.priority))
    return true;

  return false;
}

bool Tester::testPriorityHelper(Node *node, prifn_t priority) {
  if (node){

    //if there's a left node, make sure it's priority is less than parent
    if (node->_left && priority(node->_student) > priority(node->_left->_student))
      return false;

    //if there's a right node, make sure it's priority is less than parent
    if (node->_right && priority(node->_student) > priority(node->_right->_student))
      return false;

    //go down left
    if (node->_left)
      testPriorityHelper(node->_left, priority);

    //go down right
    if (node->_right)
      testPriorityHelper(node->_right, priority);
  }
  return true;
}

bool Tester::testPriorityDomainError(RQueue &priorOne, RQueue &priorTwo) {
  try {
    //purposefully merge two queues with different priorities
    priorOne.mergeWithQueue(priorTwo);
  }
  catch (domain_error &error) {
    return true;  
  }
  //wrong error caught
  catch(...) {
    return false;    
  }
  //no error caught
  return false;
}

bool Tester::testDequeueDomainError(RQueue &queue) {
  try {
    //purposefully dequeue an empty queue
    queue.getNextStudent();
  }
  catch (domain_error &error) {

    return true;
  }
  //wrong error caught
  catch(...) {

    return false;
  }
  //didn't catch an error
  return false;
}

void Tester::InsertPerformance(int numTrials, int N, RQueue& queue) {
  //scaling factor for input size
  const int a = 10;

  //to store running times
  double T = 0.0;

  //stores the clock ticks while running the program
  clock_t start, stop;
  for (int k = 0; k < numTrials; k++)
    {
      start = clock();

      for (int i = 0; i < N; i++) {
	//the algorithm to be analyzed for efficiency
	Student stu("Sam", k,1,1,1);
	queue.insertStudent(stu);
      }
      stop = clock();
      
      //number of clock ticks the algorithm took
      T = stop - start;
      cout << "Inserting " << N << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;

      //increase the input size by the scaling factor
      N = N * a;
    }
}

void Tester::ExtractPerformance(int numTrials, int N, RQueue& queue) {
  //scaling factor for input size
  const int a = 10;

  //to store running times
  double T = 0.0;

  //stores the clock ticks while running the program
  clock_t start, stop;
  for (int k = 0; k < numTrials; k++)
    {
      start = clock();
      for (int i = 0; i < N; i++) {
	//the algorithm to be analyzed for efficiency
	queue.getNextStudent();
      }
      stop = clock();
     
      //number of clock ticks the algorithm took
      T = stop - start;
      cout << "Extracting " << N << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;

      //increase the input size by the scaling factor
      N = N * a;
    }
}

// Priority functions compute an integer priority for a student. Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Student &student);
int priorityFn2(const Student &student);

int main() {
  Tester tester;

  RQueue queue1(priorityFn1);
  RQueue queue2(priorityFn1);

  // Create some test students and insert them into the queue
  //Student(string name, int priority, int year, int major, int group)
  Student student1("Sam",1,1,1,1);
  Student student2("Liz",5,1,2,0);
  Student student3("Nick",2,0,0,0);
  Student student4("Eva",4,3,2,1);

  queue1.insertStudent(student3);
  queue1.insertStudent(student1);
  queue1.insertStudent(student2);
  queue1.insertStudent(student4);

  std::cout << "\nqueue1 dump after inserting 4 students (Nick, Sam, Liz, Eva):" << std::endl;
  queue1.dump();

  Student student5("John",6,3,2,1);
  Student student6("Mia",3,0,0,0);

  queue2.insertStudent(student5);
  queue2.insertStudent(student6);

  std::cout << "\nqueue2 dump after inserting 2 students (John, Mia):" << std::endl;
  queue2.dump();

  queue1.mergeWithQueue(queue2);

  // Print the queue, change priority function, print again, change
  // again, print again.  Changing the priority function should cause
  // the heap to be rebuilt.

  std::cout << std::endl << "Contents of queue1 after merging with queue2:\n";
  queue1.printStudentQueue();
  std::cout << "\nDump queue1 after merging with queue2:\n";
  queue1.dump();
    
  queue1.setPriorityFn(priorityFn2);

  std::cout << std::endl << "Contents of queue1 after changing the priority function to priorityFn2:\n";
  queue1.printStudentQueue();
  std::cout << "\nDump queue1 after changing the priority function to priorityFn2:\n";
  queue1.dump();
  
  queue1.setPriorityFn(priorityFn1);

  std::cout << std::endl << "Contents of queue1 after changing the priority function back to priorityFn1:\n";
  queue1.printStudentQueue();
  std::cout << "\nDump queue1 after changing the priority function back to priorityFn1:\n";
  queue1.dump();

  // Remove and print students in priority order
  
  std::cout << "\nFinal queue1 in priority order:\n";
  while (queue1.numStudents() > 0) {
    Student student = queue1.getNextStudent();
    std::cout << "[" << priorityFn1(student) << "] " << student << std::endl;
  }
  
  //test copy constructor, empty heap
  RQueue zero(priorityFn1);
  RQueue copyQueue0(zero);
  if (!tester.testCopy(copyQueue0, zero))
      cout << "Copy test 1 passed" << endl;
  else
      cout << "Copy test 1 failed" << endl;
  
  //test copy constructor, one node heap
  RQueue one(priorityFn1);
  Student studentOne("Eva",4,3,2,1);
  one.insertStudent(studentOne);

  RQueue copyQueue1(one);
  if (tester.testCopy(copyQueue1, one))
      cout << "Copy test 2 passed" << endl;
  else
      cout << "Copy test 2 failed" << endl;
  //try to get the next student with one node
  one.getNextStudent();
    
  //test copy constructor, normal size heap
  RQueue reg(priorityFn1);
  Student studentTwo("Sam",1,1,1,1);
  Student studentThree("Liz",5,1,2,0);
  Student studentFour("Nick",2,0,0,0);
  Student studentFive("John",6,3,2,1);
  Student studentSix("Mia",3,0,0,0);
  reg.insertStudent(studentTwo);
  reg.insertStudent(studentThree);
  reg.insertStudent(studentFour);
  reg.insertStudent(studentFive);
  reg.insertStudent(studentSix);

  RQueue copyQueue2(reg);
  if (tester.testCopy(copyQueue2, reg))
      cout << "Copy test 3 passed" << endl;
  else
      cout << "Copy test 3 failed" << endl;

  
  //test overloaded assignment, empty heap
  RQueue zeroAssign(priorityFn1);
  RQueue assignQueue0 = zeroAssign;
  if (!tester.testAssign(assignQueue0, zeroAssign))
      cout << "Assign test 1 passed" << endl;
  else
      cout << "Assign test 1 failed" << endl;

  //test overloaded assignment, one node heap
  RQueue oneAssign(priorityFn1);
  Student studentOneAssign("Eva",4,3,2,1);
  oneAssign.insertStudent(studentOneAssign);

  RQueue assignQueue1 = oneAssign;
  if (tester.testAssign(assignQueue1, oneAssign))
      cout << "Assign test 2 passed" << endl;
  else
      cout << "Assign test 2 failed" << endl;
  
  //test overloaded assignment, normal size heap
  RQueue regAssign(priorityFn1);
  Student studentTwoAssign("Sam",1,1,1,1);
  Student studentThreeAssign("Liz",5,1,2,0);
  Student studentFourAssign("Nick",2,0,0,0);
  Student studentFiveAssign("John",6,3,2,1);
  Student studentSixAssign("Mia",3,0,0,0);

  regAssign.insertStudent(studentTwoAssign);
  regAssign.insertStudent(studentThreeAssign);
  regAssign.insertStudent(studentFourAssign);
  regAssign.insertStudent(studentFiveAssign);
  regAssign.insertStudent(studentSixAssign);

  RQueue assignQueue2 = regAssign;
  if (tester.testAssign(assignQueue2, regAssign))
      cout << "Assign test 3 passed" << endl;
  else
      cout << "Assign test 3 failed" << endl;
  
  //test overloaded assignment, lhs has value
  RQueue rhsAssign(priorityFn1);
  RQueue lhsAssign(priorityFn1);

  Student studentTwoAssign1("Sam",1,1,1,1);
  Student studentThreeAssign1("Liz",5,1,2,0);
  Student studentFourAssign1("Nick",2,0,0,0);

  Student studentFiveAssign1("John",6,3,2,1);
  Student studentSixAssign1("Mia",3,0,0,0);

  rhsAssign.insertStudent(studentTwoAssign1);
  rhsAssign.insertStudent(studentThreeAssign1);
  rhsAssign.insertStudent(studentFourAssign1);

  lhsAssign.insertStudent(studentFiveAssign1);
  lhsAssign.insertStudent(studentSixAssign1);

  lhsAssign = rhsAssign;
  if (tester.testAssign(lhsAssign, rhsAssign))
      cout << "Assign test 4 passed" << endl;
  else
      cout << "Assign test 4 failed" << endl;

  //test priority order
  RQueue queueOrder1(priorityFn1);

  // Create some test students and insert them into the queue
  //Student(string name, int priority, int year, int major, int group)
  Student studentOrder1("Sam",1,1,1,1);
  Student studentOrder2("Liz",5,1,2,0);
  Student studentOrder3("Nick",2,0,0,0);
  Student studentOrder4("Eva",4,3,2,1);

  queueOrder1.insertStudent(studentOrder3);
  queueOrder1.insertStudent(studentOrder1);
  queueOrder1.insertStudent(studentOrder2);
  queueOrder1.insertStudent(studentOrder4);

  if (tester.testPriority(queueOrder1))
    cout << "Priority test 1 passed" << endl;
  else
    cout << "Priority test 1 failed" << endl;

  //switch priority and test that the priority has been maintained and restructured
  queueOrder1.setPriorityFn(priorityFn2);

  if (tester.testPriority(queueOrder1))
    cout << "Priority test 2 passed" << endl;
  else
    cout << "Priority test 2 failed" << endl;

  //throw domain_error exception if user tries to merge two queues with different priorities
  RQueue priorOne(priorityFn1);
  RQueue priorTwo(priorityFn2);

  Student studentPrior1("Nick",2,0,0,0);
  Student studentPrior2("Eva",4,3,2,1);

  priorOne.insertStudent(studentPrior1);
  priorTwo.insertStudent(studentPrior2);

  if (tester.testPriorityDomainError(priorOne, priorTwo))
    cout << "Priority test 3 passed" << endl;
  else
    cout << "Priority test 3 failed" << endl;

  //attempting to dequeue an emtpy queue
  if (tester.testDequeueDomainError(queue1))
    cout << "Dequeue empty queue test 3 passed" << endl;
  else
    cout << "Dequeue empty queue test 3 failed" << endl;

  //testing that insert runs in O(logn)
  RQueue queuePerform(priorityFn1);
  int M = 5;//number of trials
  int N = 1000;//original input size
  tester.InsertPerformance(M, N, queuePerform);

  //testing that extract runs in O(logn), uses previously constructed queue
  tester.ExtractPerformance(M, N, queuePerform);
  
  return 0;
}

int priorityFn1(const Student&student) {
  //priority value is determined based on some criteria
  //priority value falls in the range [0-6]
  //the smaller value means the higher priority
  int priority = student.getYear() + student.getMajor() + student.getGroup();
  return priority;
}

int priorityFn2(const Student&student) {
  //priority is determined by an officer in registrar office
  //priority value falls in the range [0-6]
  //the smaller value means the higher priority
  return student.getPriority();
}
