MBP-10 Reconstructor from MBO Data

## Overview

This project reconstructs a Market-by-Price (MBP-10) orderbook from Market-by-Order (MBO) data using a high-performance C++ engine. It is designed for low-latency, high-throughput trading environments where accurate snapshots of the top ten price levels on both bid and ask sides are critical.

## Prerequisites

* C++17-compatible compiler (e.g., g++ 7+)
* Make utility

## Building the Project

1. Open a terminal and navigate to the project directory.
2. Run:

   ```bash
   make
   ```
3. The build produces the executable `reconstruction_akash`.

## Running the Reconstructor

```bash
./reconstruction_akash mbo.csv
```

* **Input:** `mbo.csv` (Market-by-Order data stream)
* **Output:** `mbp_output.csv` (Market-by-Price top 10 levels)

## Features

* **Top-10 Depth:** Maintains the top 10 price levels for bids and asks.
* **Action Support:**

  * **A (Add):** Inserts new orders into the book.
  * **C (Cancel):** Removes orders by order ID.
  * **M (Modify):** Implements as Cancel + Add for simplicity.
  * **R (Reset):** Skips initial reset/clear row.
* **Efficiency:**

  * Uses `std::map` for sorted book levels.
  * Hash map (`unordered_map`) for O(1) order lookup on cancel.
  * Manual CSV parsing to minimize temporary allocations.
  * Fast I/O: `ios::sync_with_stdio(false)` and untied streams.

## Optimization Notes

* **Data Structures:**

  * `map<double,int,greater<>>` for bids (descending)
  * `map<double,int>` for asks (ascending)
  * `unordered_map<string,Order>` for order tracking
* **Parsing:** Manual field extraction without `stringstream` to reduce overhead.
* **Inlining:** Critical update and snapshot routines inlined for minimal call overhead.

## Known Limitations

* Partial fills and delta quantities on modify events are not differentiated.
* Assumes well-formed MBO input with consistent field ordering.

## Future Improvements

* **Unit Tests:** Add automated tests covering edge cases.
* **Concurrency:** Parallelize snapshot generation for very large data streams.
* **Memory Pooling:** Reuse objects to further lower allocation costs.
* **Extended Actions:** Support advanced Databento actions (e.g., hidden orders).

## License & Contact

* Akash © 2025
* Questions or issues? Contact: [aakuakash091@gmail.com](mailto:aakuakash091@gmail.com)
