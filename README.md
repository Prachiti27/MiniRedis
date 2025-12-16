# MiniRedis

**MiniRedis** is a minimal, Redis-like in-memory key–value database built **from scratch in C++**.
The project is designed purely for **learning purposes**, to understand how systems like Redis work at a low level.

> This is **not** a production-ready Redis clone.

---

## What This Project Is About

MiniRedis focuses on the *fundamentals* behind Redis:

* TCP socket programming
* Server–client architecture
* In-memory data storage
* File descriptors & blocking I/O
* Command parsing and execution

The goal is to build intuition and confidence in **low-level backend systems**.

---

## Features (Work in Progress)

* Single client support
* key–value storage
* Supported commands:

  * `SET key value`
  * `GET key`
  * `DEL key`
  * `INCR key`
  * `EXIT`
* TTL / key expiry
* Basic error handling

> Feature set may evolve as the project grows.

---

## 🛠 Tech Stack

* **Language:** C++
* **Platform:** Linux / Unix-based systems
* **Networking:** POSIX sockets

---

## 🗂 Project Goals

This project aims to:

* Understand Redis internals from scratch
* Learn how TCP servers handle client requests
* Practice low-level memory and state management
* Strengthen backend and systems programming skills

---


