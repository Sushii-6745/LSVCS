# LSVCS

**LSVCS (Lightweight Version Control System)** is a Git-inspired version control system written completely from scratch in Modern C++.

The project aims to understand the internal design of Git by implementing its core concepts rather than wrapping Git commands.

---

## Features

- Repository initialization
- Staging area
- Blob objects
- Tree objects
- Commit objects
- Branches
- Checkout / Time-travel
- SHA-256 object hashing
- Commit graph visualization using Graphviz

---

## Commands

```bash
lsvcs init
lsvcs add 
lsvcs commit <message>
lsvcs checkout <commit/branch-name>
lsvcs branch <branch-name>
lsvcs viz
```

---

## Technologies

- Modern C++17
- OpenSSL (SHA-256)
- Graphviz
- STL

---

## Roadmap

- [x] Blob objects
- [x] Tree objects
- [x] Commit objects
- [x] Branches
- [x] Checkout
- [x] Graph visualization
- [ ] Compression (zlib)
- [ ] Merge
- [ ] Commit log
- [ ] Remote repositories
- [ ] Clone / Push / Pull

---

## Motivation

This project was built to understand how Git internally stores data, tracks history, and manages branches by implementing these mechanisms from scratch.
