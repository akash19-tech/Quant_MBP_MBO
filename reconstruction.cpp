#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <string_view>
#include <iomanip>

using namespace std;

struct Order {
    double price;
    int quantity;
    char side;
};

static map<double, int, greater<>> bid_book;
static map<double, int> ask_book;
static unordered_map<string, Order> order_map;

inline void apply_update(const string &action, const string &order_id, double price, int qty, char side) {
    if (action == "A") {
        if (side == 'B') {
            bid_book[price] += qty;
        } else {
            ask_book[price] += qty;
        }
        order_map[order_id] = {price, qty, side};
    } else if (action == "C") {
        auto it = order_map.find(order_id);
        if (it != order_map.end()) {
            Order o = it->second;
            if (o.side == 'B') {
                auto bookIt = bid_book.find(o.price);
                if (bookIt != bid_book.end()) {
                    bookIt->second -= o.quantity;
                    if (bookIt->second <= 0) bid_book.erase(bookIt);
                }
            } else {
                auto bookIt = ask_book.find(o.price);
                if (bookIt != ask_book.end()) {
                    bookIt->second -= o.quantity;
                    if (bookIt->second <= 0) ask_book.erase(bookIt);
                }
            }
            order_map.erase(it);
        }
    } else if (action == "M") {
        // modify = cancel + add
        apply_update("C", order_id, price, qty, side);
        apply_update("A", order_id, price, qty, side);
    }
}

inline void write_snapshot(ofstream &out, const string &ts) {
    out << ts;
    int n = 0;
    for (auto &p : bid_book) {
        if (n++ >= 10) break;
        out << ',' << fixed << setprecision(2) << p.first << ',' << p.second;
    }
    while (n++ < 10) out << ",,";
    n = 0;
    for (auto &p : ask_book) {
        if (n++ >= 10) break;
        out << ',' << fixed << setprecision(2) << p.first << ',' << p.second;
    }
    while (n++ < 10) out << ",,";
    out << '\n';
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: ./reconstruction_akash <mbo.csv>\n";
        return 1;
    }

    ifstream infile(argv[1]);
    ofstream outfile("mbp_output.csv");
    if (!infile || !outfile) {
        cerr << "Error opening files.\n";
        return 1;
    }

    string line;
    getline(infile, line);

    outfile << "timestamp";
    for (int i = 1; i <= 10; ++i) outfile << ",bid_price_" << i << ",bid_size_" << i;
    for (int i = 1; i <= 10; ++i) outfile << ",ask_price_" << i << ",ask_size_" << i;
    outfile << '\n';

    while (getline(infile, line)) {
        // parse only required fields using manual scanning
        string ts, action, side_s, price_s, qty_s, oid;
        int field = 0, start = 0;
        for (int i = 0; i <= (int)line.size(); ++i) {
            if (i == (int)line.size() || line[i] == ',') {
                string_view token(line.c_str() + start, i - start);
                switch (field) {
                    case 1: ts = string(token); break;
                    case 5: action = string(token); break;
                    case 6: side_s = string(token); break;
                    case 7: price_s = string(token); break;
                    case 8: qty_s = string(token); break;
                    case 10: oid = string(token); break;
                    default: break;
                }
                if (++field > 10) break;
                start = i + 1;
            }
        }
        if (action == "R" || oid.empty() || price_s.empty() || qty_s.empty()) continue;
        char side = side_s.empty() ? 'N' : side_s[0];
        if (side == 'N') continue;
        double price = stod(price_s);
        int qty = stoi(qty_s);
        apply_update(action, oid, price, qty, side);
        write_snapshot(outfile, ts);
    }

    return 0;
}