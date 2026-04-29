# IST Interface Module Documentation

## Introduction

The IST Interface module provides connectivity and transaction processing capabilities for the IST network within the payment switching system. It is responsible for managing communication, transaction flows, and session management between the core system and the IST network, ensuring reliable and secure message exchange for financial operations such as balance inquiries, transaction authorizations, settlements, and safe store-and-forward (SAF) processing.

The IST Interface is designed to operate as a set of cooperating threads and initialization routines, each handling a specific aspect of the IST protocol and transaction lifecycle. It interacts with the core system libraries, threading utilities, and data structures, and follows architectural patterns common to other network interface modules (e.g., Visa, Base24, JCB, etc.) in the system.

## Core Functionality

The IST Interface module consists of the following core components:

- **ist_ini.c**: Handles initialization, configuration loading, and signal management for the IST interface.
- **ist_thread_adm.c**: Manages administrative tasks and control messages for the IST network.
- **ist_thread_bal.c**: Processes balance inquiry requests and responses.
- **ist_thread_line.c**: Manages the communication line/session with the IST network, including connection establishment and monitoring.
- **ist_thread_saf.c**: Handles store-and-forward (SAF) operations for offline or deferred transactions.
- **ist_thread_signin.c**: Manages sign-in/sign-out procedures and session authentication with the IST network.
- **ist_thread_timeout.c**: Handles timeout management for transactions and network operations.

Each component is implemented as a separate thread or process, enabling concurrent handling of different transaction types and network events.

## Architecture Overview

The IST Interface module follows a modular, thread-based architecture. Each thread is responsible for a specific function, and all threads coordinate via shared data structures and inter-thread communication mechanisms provided by the core threading library.

### High-Level Architecture

```mermaid
graph TD
    A[ist_ini.c\nInitialization & Signals] --> B[ist_thread_adm.c\nAdmin Thread]
    A --> C[ist_thread_bal.c\nBalance Thread]
    A --> D[ist_thread_line.c\nLine Management Thread]
    A --> E[ist_thread_saf.c\nSAF Thread]
    A --> F[ist_thread_signin.c\nSign-in Thread]
    A --> G[ist_thread_timeout.c\nTimeout Thread]
    
    D -->|Session Events| F
    D -->|Transaction Events| C
    D -->|Admin Events| B
    D -->|SAF Events| E
    G -->|Timeouts| D
    G -->|Timeouts| C
    G -->|Timeouts| E
```

### Component Relationships

- **ist_ini.c** initializes all IST threads, loads configuration, and sets up signal handling (using `sigset_t`).
- **ist_thread_line.c** acts as the main communication handler, routing messages to the appropriate processing threads.
- **ist_thread_timeout.c** monitors for operation timeouts and triggers recovery or retry logic as needed.
- **ist_thread_saf.c** ensures that transactions which cannot be immediately processed are safely stored and forwarded when possible.
- **ist_thread_signin.c** manages session authentication and periodic sign-in requirements.

## Data Flow

```mermaid
flowchart TD
    CoreSystem([Core System]) --> IST_Line[ist_thread_line.c]
    IST_Line -->|Balance Inquiry| IST_Bal[ist_thread_bal.c]
    IST_Line -->|Admin| IST_Adm[ist_thread_adm.c]
    IST_Line -->|SAF| IST_SAF[ist_thread_saf.c]
    IST_Line -->|Sign-in| IST_Signin[ist_thread_signin.c]
    IST_Timeout[ist_thread_timeout.c] -->|Timeout| IST_Line
    IST_Timeout -->|Timeout| IST_Bal
    IST_Timeout -->|Timeout| IST_SAF
    IST_Bal --> IST_Line
    IST_Line --> CoreSystem
```

## Dependencies

The IST Interface module depends on several core libraries and data structures:

- **Threading Library**: For thread management, signal handling (`sigset_t`), and timeouts (`timeval`, `timespec`). See [Threading Library](Threading Library.md).
- **Core Data Structures**: For transaction, account, and message representations. See [Core Data Structures](Core Data Structures.md).
- **Core Libraries**: For TCP/IP communication and network abstraction. See [Core Libraries](Core Libraries.md).

It also follows architectural conventions established by other network interface modules, such as [Visa Interface](Visa Interface.md), [Base24 Interface](Base24 Interface.md), and [JCB Interface](JCB Interface.md).

## Process Flows

### Initialization and Startup

```mermaid
flowchart TD
    Start([System Startup]) --> LoadConfig[ist_ini.c: Load Configuration]
    LoadConfig --> SetupSignals[ist_ini.c: Setup Signal Handlers]
    SetupSignals --> SpawnThreads[ist_ini.c: Spawn IST Threads]
    SpawnThreads --> Ready([IST Interface Ready])
```

### Transaction Processing

```mermaid
flowchart TD
    Receive([Receive Transaction]) --> Route[ist_thread_line.c: Route to Handler]
    Route -->|Balance Inquiry| Bal[ist_thread_bal.c]
    Route -->|Admin| Adm[ist_thread_adm.c]
    Route -->|SAF| SAF[ist_thread_saf.c]
    Route -->|Sign-in| Signin[ist_thread_signin.c]
    Bal --> Respond([Send Response])
    Adm --> Respond
    SAF --> Respond
    Signin --> Respond
```

## Integration in the Overall System

The IST Interface is one of several network interface modules in the payment switching system. It is designed to be functionally parallel to other interfaces (e.g., Visa, Base24, JCB, etc.), each handling a specific network protocol but sharing common architectural patterns and core libraries. This modular approach allows for scalability, maintainability, and consistent transaction processing across multiple payment networks.

For more details on the overall system architecture and other interfaces, refer to:
- [Visa Interface](Visa Interface.md)
- [Base24 Interface](Base24 Interface.md)
- [JCB Interface](JCB Interface.md)
- [Core Data Structures](Core Data Structures.md)
- [Threading Library](Threading Library.md)
- [Core Libraries](Core Libraries.md)

## Summary

The IST Interface module is a robust, thread-based component that enables seamless integration with the IST network, supporting secure, reliable, and concurrent transaction processing. Its design leverages shared system libraries and follows best practices established across all network interface modules in the payment switching platform.
