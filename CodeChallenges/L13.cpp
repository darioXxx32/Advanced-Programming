#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

// ======================
// Part 2: Implementing Move Semantics
// Define a Simple Resource-Managing Class (Buffer)
// ======================
class Buffer {
private:
    int* data;
    size_t size;

public:
    // Constructor: allocate the buffer and initialize size.
    Buffer(size_t s) : size(s), data(new int[s]) {
        std::cout << "Constructor called (size " << size << ")\n";
    }

    // Destructor: free the allocated memory.
    ~Buffer() {
        delete[] data;
        std::cout << "Destructor called (size " << size << ")\n";
    }

    // ======================
    // Task 2.2: Add Copy Constructor and Assignment
    // ======================
    // Copy Constructor: perform deep copy.
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        std::copy(other.data, other.data + other.size, data);
        std::cout << "Copy Constructor called (size " << size << ")\n"; 
    }
    
    // Copy Assignment Operator: perform deep copy.
    Buffer& operator=(const Buffer& other) {
        if (this != &other) { // Self-assignment check.
            delete[] data; // Release current resources.
            size = other.size;
            data = new int[size]; // Allocate new resources.
            std::copy(other.data, other.data + size, data); // Copy elements.
            std::cout << "Copy Assignment called (size " << size << ")\n";
        }
        return *this;
    }

    // ======================
    // Task 2.3: Add Move Constructor and Assignment
    // ======================
    // Move Constructor: transfer ownership of resources.
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr; // Leave the moved-from object in a safe state.
        other.size = 0;
        std::cout << "Move Constructor called (size " << size << ")\n";
    }
    
    // Move Assignment Operator: transfer ownership of resources.
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) { // Self-assignment check.
            delete[] data; // Release current resources.
            data = other.data;
            size = other.size;
            // Leave the moved-from object in a safe state.
            other.data = nullptr;
            other.size = 0;
            std::cout << "Move Assignment called (size " << size << ")\n";
        }
        return *this;
    }
};

// ======================
// Part 1: Introduction to Copy vs. Move (using std::vector)
// ======================
void vectorCopyAndMoveDemo() {
    std::cout << "== Part 1: Copy vs Move with std::vector ==\n";
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2 = v1;            // Copy operation
    std::vector<int> v3 = std::move(v1);   // Move operation

    std::cout << "v1 size: " << v1.size() << std::endl; // Expected 0 after move
    std::cout << "v2 size: " << v2.size() << std::endl; // Expected 4 (copy)
    std::cout << "v3 size: " << v3.size() << std::endl; // Expected 4 (moved)

     /*
      ✅ Task 1.1 Reading and Discussion

      1. What is the output of this program?
         The output is:
         v1 size: 0
         v2 size: 4
         v3 size: 4

      2. Why does v1.size() return 0 after the move?
         After moving 'v1' to 'v3', the move constructor transfers 'v1's resources to 'v3'.
         This leaves 'v1' in a valid but empty state, so its size is 0.

      3. What does std::move do?
         'std::move' casts its argument to an rvalue reference, indicating that it is safe to move
         (transfer) the internal resources of the object instead of copying them.

      4. Why is v1 still in a valid but unspecified state?
         According to the C++ specification, after a move operation, the moved-from object
         ('v1') is in a valid state (meaning it can be destructed or assigned to),
         but its exact content is unspecified.
    */

}

// ======================
// Part 3: Using Your Buffer Class
// ======================
void bufferUsageDemo() {
    std::cout << "\n== Part 3: Using the Buffer Class ==\n";
    Buffer buf1(5);               // Calls constructor.
    Buffer buf2 = buf1;           // Calls copy constructor.
    Buffer buf3(10);
    buf3 = buf1;                  // Calls copy assignment.
    Buffer buf4 = std::move(buf1);  // Calls move constructor.
    Buffer buf5(3);
    buf5 = std::move(buf2);       // Calls move assignment.

    /*
      Questions:
      - What constructors and assignments are called and when?
         * buf1: constructor.
         * buf2: copy constructor.
         * buf3: constructor then copy assignment.
         * buf4: move constructor.
         * buf5: constructor then move assignment.
      - What happens to the source object after a move?
         The source object is left in a valid but unspecified (typically empty) state.
      - What would happen if you didn't define the move constructor/assignment?
         The compiler might generate default ones, or fall back to copying, leading to inefficient operations.
    */
}

// ======================
// Part 4: Performance Comparison
// ======================
void performanceComparison() {
    std::cout << "\n== Part 4: Performance Comparison ==\n";
    const size_t largeSize = 10000000; // 10 million

    // Measure time for copy operation
    auto start = std::chrono::high_resolution_clock::now();
    Buffer bigBuffer1(largeSize);
    Buffer bigBuffer2 = bigBuffer1; // Copy operation
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Copy Duration: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";

    // Measure time for move operation
    start = std::chrono::high_resolution_clock::now();
    Buffer bigBuffer3 = std::move(bigBuffer1); // Move operation
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Move Duration: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";

    /*
      Task 3.1 Measure Performance:
      - The timings above compare the duration of copying versus moving a large buffer.
      - Typically, moving is significantly faster than copying because it only transfers pointers rather than performing a deep copy.
    */
}

// ======================
// Part 5: Summary (Answers as comments)
// ======================
/*
  Summary Questions:
  - What did you learn about move semantics?
      Move semantics allow for efficient resource transfer, avoiding expensive deep copies.
  - In what kinds of projects do you think this would matter?
      In projects that manage large data structures, resource-intensive objects, or real-time systems,
      move semantics can greatly improve performance.
  - What are some risks of using std::move incorrectly?
      Using std::move incorrectly can lead to resource leaks, undefined behavior, or using objects in an invalid state.
*/

int main() {
    // Demonstrate vector copy vs move.
    vectorCopyAndMoveDemo();

    // Demonstrate Buffer class usage.
    bufferUsageDemo();

    // Perform performance comparison between copy and move operations.
    performanceComparison();
  
    return 0;
}