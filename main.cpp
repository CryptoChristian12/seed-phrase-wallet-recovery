#include <iostream>
#include <windows.h>
#include <fstream>
#include <ctime>

#include "generate.h"
#include "checker.h"

int main(int argc, char** argv)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string title = "[Seed phrase Generate & Check - BTC; ETH; LTC; DOGE] - By Cosmo11 - Updated by CryptoChristian";
    SetConsoleTitleA(title.c_str());

    DWORD webstatus = 0;
    if (!inet::webstatus_check("https://www.blockchain.com/explorer", &webstatus) || webstatus != 200) {
        std::cout << "Failed to access the blockchain. Check your internet connection\n";
        Sleep(6000);
        return 0;
    }

    srand(static_cast<unsigned int>(time(0)));
    DWORD64 seed_count = 0;
    float total_balance = 0;

    while (true) {
        std::cout << "" $$$$$$\  $$$$$$$$\ $$$$$$$$\ $$$$$$$\         $$$$$$\  $$$$$$$$\ $$\   $$\ \n$$  __$$\ $$  _____|$$  _____|$$  __$$\       $$  __$$\ $$  _____|$$$\  $$ | \n$$ /  \__|$$ |      $$ |      $$ |  $$ |      $$ /  \__|$$ |      $$$$\ $$ | \n\$$$$$$\  $$$$$\    $$$$$\    $$ |  $$ |      $$ |$$$$\ $$$$$\    $$ $$\$$ | \n \____$$\ $$  __|   $$  __|   $$ |  $$ |      $$ |\_$$ |$$  __|   $$ \$$$$ | \n$$\   $$ |$$ |      $$ |      $$ |  $$ |      $$ |  $$ |$$ |      $$ |\$$$ | \n\$$$$$$  |$$$$$$$$\ $$$$$$$$\ $$$$$$$  |      \$$$$$$  |$$$$$$$$\ $$ | \$$ | \n \______/ \________|\________|\_______/        \______/ \________|\__|  \__| \n"
By Cosmo11, Updated by CryptoChristian\nSelect an action:\n'1' - Generate 1 seed phrase\n'2' - Search for seed phrases with coins (BTC, ETH, LTC, DOGE)\n";

        if (GetAsyncKeyState('1') & 1) {
            std::cout << "\n\n";
            std::string seed = generate_seed_phrase(12);
            std::cout << "Seed: " << seed << "\n\n";
        }
        else if (GetAsyncKeyState('2') & 1) {
            break; // Exit loop to start brute-force mode
        }
        Sleep(100); // Prevent high CPU usage
    }

    // Brute-force mode
    while (true) {
        std::string seed = generate_seed_phrase(12);
        std::cout << "Seed: " << seed;

        balance wallet_balance;
        if (check_wallet(seed, &wallet_balance) != 0)
            continue;

        if (is_empty(wallet_balance)) {
            SetConsoleTextAttribute(hConsole, 4);
            std::cout << " (empty)";
            SetConsoleTextAttribute(hConsole, 7);
        } else {
            SetConsoleTextAttribute(hConsole, 2);
            std::cout << " (with balance)";
            SetConsoleTextAttribute(hConsole, 7);

            total_balance += wallet_balance.btc * get_btc_price();
            total_balance += wallet_balance.eth * get_eth_price();
            total_balance += wallet_balance.doge * get_doge_price();
            total_balance += wallet_balance.ltc * get_ltc_price();

            std::string found_info = "Address: " + get_wallet_address_from_mnemonic(seed) +
                "\nMnemonic: " + seed + "\nPrivate Key: " + get_private_key_from_mnemonic(seed) +
                "\nBalance: " + std::to_string(wallet_balance.btc) + " BTC, " +
                std::to_string(wallet_balance.eth) + " ETH, " +
                std::to_string(wallet_balance.doge) + " DOGE, " +
                std::to_string(wallet_balance.ltc) + " LTC\n\n";

            // Use fstream instead of CreateFileA
            std::ofstream outFile("found_wallets_phrases.txt", std::ios::app);
            if (outFile.is_open()) {
                outFile << found_info;
                outFile.close();
            }
        }

        std::cout << std::endl;
        std::string new_title = title + " | Checked seeds: " + std::to_string(seed_count) + " | Total balance: $" + std::to_string(total_balance);
        SetConsoleTitleA(new_title.c_str());
        ++seed_count;

        if ((seed_count % 10000000000) == 0) {
            system("cls");
        }
    }

    return 0;
}
