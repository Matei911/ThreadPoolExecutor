# ThreadPoolExecutor

ThreadPoolExecutor is a comprehensive system designed to manage tasks efficiently using a thread pool mechanism. This project is structured into several key sections, each responsible for different aspects of the system's functionality.

## Sections

### main.c

This section contains the foundational code of the project, including file reading operations, initialization of necessary variables, and division of commands into manageable tasks. The main functionality orchestrates the overall flow of the application, ensuring smooth operation and task distribution.

### queue.c and queue.h

- **Purpose:** Implements a generic queue structure.
- **Details:** These files provide the functionality to manage a queue, allowing for operations such as enqueue, dequeue, and checking if the queue is empty. This generic queue is utilized across the project to manage tasks and data efficiently.

### stack.c and stack.h

- **Purpose:** Implements a generic stack structure.
- **Details:** Similar to the queue implementation, these files define a generic stack that supports basic stack operations such as push, pop, and checking if the stack is empty. This stack is used for managing tasks in a LIFO (Last In, First Out) manner.

### scheduler.c and scheduler.h

- **Overview:** The scheduler is at the heart of the ThreadPoolExecutor, managing the distribution and execution of tasks.
- **Implementation:** Within the scheduler, there's a combination of a stack and three queues, each serving a specific purpose in task management. Additionally, this section includes essential data related to the problem, such as time quantum and the number of threads.
- **Design Philosophy:** The aim was to modularize the code as much as possible, particularly in the display functions, to facilitate easier debugging and maintenance. By relying on structured functions, the system's overall readability and debuggability are significantly enhanced.

## Summary

ThreadPoolExecutor is designed with modularity and efficiency in mind, leveraging generic data structures and a well-thought-out architecture to manage and execute tasks effectively. Through careful implementation and attention to detail, this project demonstrates a robust approach to concurrent programming and task scheduling.
