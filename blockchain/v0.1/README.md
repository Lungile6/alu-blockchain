# Holberton School Blockchain - v0.1

## 📝 Overview
This project is the initial implementation of a Blockchain system in C. It focuses on the foundational architecture of a distributed ledger, specifically managing a sequential chain of blocks, ensuring data integrity through SHA-256 hashing, and persisting the data to disk using binary serialization.

---

## 🎓 Concepts Learned

### 1. Cryptographic Hash Linking
I learned how to create an immutable chain where each block points to the previous one using its hash. This "linked" nature ensures that if any data in a previous block is changed, the hashes of all subsequent blocks become invalid.

### 2. Binary Serialization & Persistence
Unlike saving data in a text file (like JSON or CSV), this project implements **binary serialization**. I learned how to:
* Map C structures directly to file streams.
* Handle **Endianness** to ensure data remains consistent across different CPU architectures.
* Use **Magic Numbers** and **Version Headers** to identify valid file formats.

### 3. Pointer Arithmetic & Memory Alignment
Handling the `block_t` structure required a deep understanding of how C aligns data in memory. By treating the `info` and `data` structures as a contiguous buffer, I could hash them efficiently using OpenSSL's SHA-256.

### 4. Custom Library Integration
This project integrates three distinct layers:
* **The Application**: Our Blockchain logic.
* **The Data Structure**: A custom Doubly Linked List (`libllist`).
* **The Security Layer**: A wrapper for OpenSSL (`libhblk_crypto`).



---

## 🏗️ Data Structures

* **`blockchain_t`**: Stores the head of the chain (a `llist_t` of blocks).
* **`block_t`**: The main container.
    * `info`: Metadata (index, timestamp, nonce, previous hash).
    * `data`: The actual payload (buffer and length).
    * `hash`: The unique fingerprint of this block.

---

## 🛠️ Compilation & Environment

### Prerequisites
* **OpenSSL**: For cryptographic functions.
* **Betty Linter**: For code style compliance.

### Compilation Command
You must link the headers and libraries from the `llist` and `crypto` directories:

```bash
gcc -Wall -Wextra -Werror -pedantic \
    -I. -I../../llist -I../../crypto \
    -o blockchain_v0.1 \
    *.c \
    provided/_blockchain_print.c \
    provided/_endianness.c \
    test/main.c \
    -L../../llist -lllist \
    -L../../crypto -lhblk_crypto \
    -lssl -lcrypto -pthread
