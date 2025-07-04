#include <iostream>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

class DES {
private:
    const int IP[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };

    const int IP_INV[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    const int PC1[56] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
    };

    const int PC2[48] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    };

    const int E[48] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
    };

    const int P[32] = {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
    };

    const int SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    const int S[8][4][16] = {
        {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
         {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
         {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
         {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
        {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
         {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
         {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
         {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
        {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
         {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
         {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
         {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
        {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
         {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
         {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
         {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
        {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
         {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
         {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
         {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
        {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
         {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
         {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
         {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
        {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
         {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
         {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
         {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
        {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
         {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
         {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
         {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
    };

    bitset<64> initialPermutation(bitset<64> data) {
        bitset<64> result;
        for (int i = 0; i < 64; i++) {
            result[i] = data[IP[i] - 1];
        }
        return result;
    }

    bitset<56> keyPermutation1(bitset<64> key) {
        bitset<56> result;
        for (int i = 0; i < 56; i++) {
            result[i] = key[PC1[i] - 1];
        }
        return result;
    }

    bitset<48> keyPermutation2(bitset<56> key) {
        bitset<48> result;
        for (int i = 0; i < 48; i++) {
            result[i] = key[PC2[i] - 1];
        }
        return result;
    }

    bitset<48> expansion(bitset<32> right_half) {
        bitset<48> result;
        for (int i = 0; i < 48; i++) {
            result[i] = right_half[E[i] - 1];
        }
        return result;
    }

    bitset<32> permutation(bitset<32> data) {
        bitset<32> result;
        for (int i = 0; i < 32; i++) {
            result[i] = data[P[i] - 1];
        }
        return result;
    }

    bitset<64> finalPermutation(bitset<64> data) {
        bitset<64> result;
        for (int i = 0; i < 64; i++) {
            result[i] = data[IP_INV[i] - 1];
        }
        return result;
    }

    bitset<28> rotateLeft(bitset<28> bits, int n) {
        string s = bits.to_string();
        return bitset<28>(s.substr(n) + s.substr(0, n));
    }

    bitset<32> fFunction(bitset<32> right_half, bitset<48> round_key) {
        bitset<48> expanded = expansion(right_half);
        expanded ^= round_key;

        bitset<32> result;
        string expanded_str = expanded.to_string();

        for (int i = 0; i < 8; i++) {
            string block = expanded_str.substr(i * 6, 6);
            int row = (block[0] - '0') * 2 + (block[5] - '0');
            int col = (block[1] - '0') * 8 + (block[2] - '0') * 4 +
                      (block[3] - '0') * 2 + (block[4] - '0');
            int value = S[i][row][col];
            bitset<4> value_bits(value);
            for (int j = 0; j < 4; j++) {
                result[i * 4 + j] = value_bits[3 - j];
            }
        }
        return permutation(result);
    }

    vector<bitset<48>> generateRoundKeys(bitset<64> key) {
        bitset<56> permuted_key = keyPermutation1(key);
        bitset<28> left_half(permuted_key.to_string().substr(0, 28));
        bitset<28> right_half(permuted_key.to_string().substr(28));
        vector<bitset<48>> round_keys;

        for (int i = 0; i < 16; i++) {
            left_half = rotateLeft(left_half, SHIFTS[i]);
            right_half = rotateLeft(right_half, SHIFTS[i]);
            bitset<56> combined(left_half.to_string() + right_half.to_string());
            round_keys.push_back(keyPermutation2(combined));
        }
        return round_keys;
    }

public:
    bitset<64> encrypt(bitset<64> plaintext, bitset<64> key) {
        bitset<64> permuted = initialPermutation(plaintext);
        bitset<32> left(permuted.to_string().substr(0, 32));
        bitset<32> right(permuted.to_string().substr(32));
        vector<bitset<48>> round_keys = generateRoundKeys(key);

        for (int i = 0; i < 16; i++) {
            bitset<32> temp = right;
            right = left ^ fFunction(right, round_keys[i]);
            left = temp;
        }
        bitset<64> combined(right.to_string() + left.to_string());
        return finalPermutation(combined);
    }

    bitset<64> decrypt(bitset<64> ciphertext, bitset<64> key) {
        bitset<64> permuted = initialPermutation(ciphertext);
        bitset<32> left(permuted.to_string().substr(0, 32));
        bitset<32> right(permuted.to_string().substr(32));
        vector<bitset<48>> round_keys = generateRoundKeys(key);

        for (int i = 15; i >= 0; i--) {
            bitset<32> temp = right;
            right = left ^ fFunction(right, round_keys[i]);
            left = temp;
        }
        bitset<64> combined(right.to_string() + left.to_string());
        return finalPermutation(combined);
    }
};

bitset<64> stringToBitset(const string& str) {
    string binary_str;
    for (char c : str) {
        binary_str += bitset<8>(c).to_string();
    }
    while (binary_str.length() < 64) {
        binary_str = "0" + binary_str;
    }
    return bitset<64>(binary_str);
}

int main() {
    DES des;
    string plaintext_str, key_str = "POTATO";

    cout << "Введите открытый текст (до 8 символов): ";
    getline(cin, plaintext_str);

    if (plaintext_str.length() > 8) {
        plaintext_str = plaintext_str.substr(0, 8);
        cout << "Текст обрезан до 8 символов: " << plaintext_str << endl;
    }

    bitset<64> plain_bits = stringToBitset(plaintext_str);
    bitset<64> key_bits = stringToBitset(key_str);

    cout << "Открытый текст в двоичном виде: " << plain_bits << endl;

    bitset<64> encrypted = des.encrypt(plain_bits, key_bits);
    cout << "Зашифрованный текст в двоичном виде: " << encrypted << endl;

    bitset<64> decrypted = des.decrypt(encrypted, key_bits);
    cout << "Расшифрованный текст в двоичном виде: " << decrypted << endl;

    return 0;
}
