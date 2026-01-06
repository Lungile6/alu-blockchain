# ALU Blockchain - Cryptography

## 📋 Overview
This repository contains the foundational cryptographic implementations for the ALU Blockchain project. The primary focus of this module is the implementation of the SHA-256 hashing algorithm and understanding the mechanics of digital signatures and asymmetric cryptography.

---

## 🛡️ General Concepts

### 1. How a Blockchain is considered “unbreakable”
A blockchain is considered immutable and "unbreakable" due to **cryptographic linking**. Each block contains the hash of the previous block's header. 
* **The Chain Effect:** If a single character in Block 1 is changed, its hash changes. Since Block 2 contains Block 1's hash, Block 2's hash also changes. This continues in a domino effect.
* **Consensus:** To successfully "break" the chain, an attacker would need to recalculate the hashes for every subsequent block and convince the majority of the network to accept the fraudulent chain (a 51% attack).



### 2. What is a hash algorithm?
A hash algorithm is a mathematical function that maps data of arbitrary size to a bit string of a fixed size.
* **Deterministic:** The same input always produces the exact same output.
* **Quick to Compute:** The hash is generated rapidly for any given input.
* **Pre-image Resistance:** It is computationally infeasible to reverse the process (you cannot get the original data from the hash).
* **Small Change, Big Difference:** A tiny change in input creates a completely different output (Avalanche Effect).

### 3. What SHA stands for
**SHA** stands for **Secure Hash Algorithm**. It is a family of cryptographic hash functions designed by the NSA and published by NIST. **SHA-256**, used in this project, produces a 256-bit (32-byte) signature and is the standard used by Bitcoin.

### 4. How hash algorithms apply to Blockchains
* **Block Integrity:** Identifying if data has been tampered with.
* **Merkle Trees:** Efficiently verifying large data structures within a block.
* **Mining (Proof of Work):** Miners must find a hash that meets a certain difficulty target to add a new block.

### 5. What is asymmetric cryptography?
Also known as **Public-Key Cryptography**, it uses pairs of keys:
* **Public Key:** Shared openly; used to encrypt data or verify signatures.
* **Private Key:** Kept secret; used to decrypt data or create signatures.

### 6. How asymmetric cryptography applies to cryptocurrencies
It allows for **permissionless ownership**. Your "address" is derived from your public key. Only the person holding the corresponding private key can authorize a transaction (spending the funds).

### 7. What ECC and ECDSA stand for
* **ECC (Elliptic Curve Cryptography):** An approach to public-key cryptography based on the algebraic structure of elliptic curves over finite fields. It requires smaller keys than RSA to provide the same level of security.
* **ECDSA (Elliptic Curve Digital Signature Algorithm):** The specific implementation of a digital signature algorithm that utilizes ECC. It is the gold standard for securing cryptocurrency transactions.



### 8. What a digital signature is
A digital signature is a mathematical proof of two things:
1. **Authentication:** The message was created by a known sender.
2. **Integrity:** The message was not altered in transit.

### 9. How digital signatures apply to cryptocurrencies
In a transaction, the sender signs a hash of the transaction data with their **Private Key**. Any node on the network can then use the sender's **Public Key** to verify that the signature is valid, ensuring the sender actually authorized the move of funds.

---

## 💻 Technical Implementation

### Function Prototype
`uint8_t *sha256(int8_t const *s, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH]);`

### Requirements
* **No dynamic memory allocation** (no `malloc` or `free`).
* Uses OpenSSL's `crypto` library.
* Handles `NULL` digest pointers safely.

### Compilation
To compile the test suite:
```bash
gcc -Wall -Wextra -Werror -pedantic -I. -o sha256-test test/sha256-main.c provided/_print_hex_buffer.c sha256.c -lssl -lcrypto
