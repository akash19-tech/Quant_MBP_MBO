# MBP-10 Orderbook Reconstructor from MBO Data

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](https://isocpp.org/std/the-standard) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> **High-performance C++ engine** to reconstruct top-10 depth snapshots of an orderbook from raw MBO feed.

---

## 📋 Table of Contents

1. [Overview](#overview)
2. [Repository Structure](#repository-structure)
3. [Prerequisites](#prerequisites)
4. [Build & Run](#build--run)
5. [Input & Output](#input--output)
6. [Key Features](#key-features)
7. [Optimization Techniques](#optimization-techniques)
8. [Known Limitations](#known-limitations)
9. [Future Work](#future-work)
10. [License](#license)

---

## 🔍 Overview

Reconstruct a **Market-by-Price (MBP-10)** orderbook from a **Market-by-Order (MBO)** data stream. Generates time-stamped CSV snapshots of the top 10 bid and ask levels, tailored for low-latency, high-frequency trading scenarios.

## 📁 Repository Structure

```bash
quant_dev_trial/
├── Makefile            # Build script
├── README.md           # This documentation
├── reconstruction.cpp  # C++ source code
├── mbo.csv             # Sample MBO input data
└── mbp_output.csv      # Generated MBP-10 output
```

## ⚙️ Prerequisites

* **Compiler** with C++17 support (e.g., `g++` 7+ or `clang++` 7+)
* **make** utility

## 🛠 Build & Run

```bash
# 1. Build
make

# 2. Execute
./reconstruction_akash mbo.csv
```

* **Input:** `mbo.csv` (Databento MBO schema)
* **Output:** `mbp_output.csv` (MBP-10 snapshots)

## 📂 Input & Output Format

* **Input CSV** (`mbo.csv`) columns include: `ts_event`, `action`, `side`, `price`, `size`, `order_id`, etc.
* **Output CSV** (`mbp_output.csv`):

  ```csv
  timestamp,bid_price_1,bid_size_1,...,bid_price_10,bid_size_10,ask_price_1,ask_size_1,...,ask_price_10,ask_size_10
  ```

  Each row represents the book state immediately after a valid update.

## ✨ Key Features

* **Top-10 Depth Maintenance** on both bid and ask sides
* **Action Handling:**

  * **A**dd orders
  * **C**ancel orders
  * **M**odify orders (Cancel + Add)
  * **R**eset (initial clear) is skipped
* **High Throughput:** Manual CSV parsing & fast I/O
* **Accurate Snapshots:** Consistent, reproducible snapshots after each update

## 🚀 Optimization Techniques

* **Data Structures:**

  * `std::map<double,int,greater<>>` for descending bids
  * `std::map<double,int>` for ascending asks
  * `std::unordered_map<string,Order>` for O(1) order lookup on cancel
* **Parsing:** Manual delimiter scan; avoids `stringstream` overhead
* **I/O Tuning:** `ios::sync_with_stdio(false)` & untied streams
* **Inlining:** Critical routines (`apply_update`, `write_snapshot`) are `inline`

## 🐞 Known Limitations

* No detailed handling of **partial fills** or **delta quantities** on modify
* Assumes **well-formed** MBO input with fixed field ordering
* **No unit tests** included (yet)

## 🔭 Future Work

* Integrate **unit testing** (e.g., Google Test)
* **Parallelize** snapshot generation for large data volumes
* Implement **memory pooling** for extreme low-latency use cases
* Support **extended Databento** action codes (e.g., hidden/iceberg orders)

---

## 📝 License

Distributed under the **MIT License**. See [LICENSE](LICENSE) for details.

---

*© 2025 Akash Mishra*
