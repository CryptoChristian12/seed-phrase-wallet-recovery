#include <iostream>
#include <Windows.h>
#include <ctime>
#include <chrono>  
#include "generate.h"
#include "checker.h"

int main(int argc, char** argv)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string title = "[Seed phrase Generate & Check - BTC; ETH; LTC; DOGE] - By Cosmo11 - Updated by CryptoChristian";
    SetConsoleTitleA(title.c_str());

    DWORD webstatus = 0;
    if (!inet::webstatus_check("https://www.blockchain.com/explorer", &webstatus) || webstatus != 200)
    {
        std::cout << ("Failed to access the blockchain. Check your internet connection\n");
        Sleep(6000);
        exit(0);
    }

    srand(static_cast<unsigned int>(time(0)));
    DWORD64 seed_count = 0;
    float total_balance = 0;
    float CPM = 0;  

menu:
    std::cout << ("" $$$$$$\  $$$$$$$$\ $$$$$$$$\ $$$$$$$\         $$$$$$\  $$$$$$$$\ $$\   $$\ \n$$  __$$\ $$  _____|$$  _____|$$  __$$\       $$  __$$\ $$  _____|$$$\  $$ | \n$$ /  \__|$$ |      $$ |      $$ |  $$ |      $$ /  \__|$$ |      $$$$\ $$ | \n\$$$$$$\  $$$$$\    $$$$$\    $$ |  $$ |      $$ |$$$$\ $$$$$\    $$ $$\$$ | \n \____$$\ $$  __|   $$  __|   $$ |  $$ |      $$ |\_$$ |$$  __|   $$ \$$$$ | \n$$\   $$ |$$ |      $$ |      $$ |  $$ |      $$ |  $$ |$$ |      $$ |\$$$ | \n\$$$$$$  |$$$$$$$$\ $$$$$$$$\ $$$$$$$  |      \$$$$$$  |$$$$$$$$\ $$ | \$$ | \n \______/ \________|\________|\_______/        \______/ \________|\__|  \__| \n"
Select an action:\n'1' - Generate 1 seed phrase\n'2' - Search for seed phrases with coins (BTC, ETH, LTC, DOGE)\n");
    while (true) {
        if (GetAsyncKeyState('1') & 1) {
            std::cout << "\n\n";
            std::string seed = generate_seed_phrase(12);
            std::cout << "Seed: " << seed << "\n\n";
            goto menu;
        }
        else if (GetAsyncKeyState('2') & 1) {
            auto start_time = std::chrono::steady_clock::now();
            auto last_CPM_update = start_time;  
            goto brute;
        }
        Sleep(1);
    }

brute:
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
        }
        else {
            SetConsoleTextAttribute(hConsole, 2);
            std::cout << " (with balance)";
            SetConsoleTextAttribute(hConsole, 7);

            total_balance += wallet_balance.btc * get_btc_price();
            total_balance += wallet_balance.eth * get_eth_price();
            total_balance += wallet_balance.doge * get_doge_price();
            total_balance += wallet_balance.ltc * get_ltc_price();

            std::string found_info = "Address: " + get_wallet_address_from_mnemonic(seed) + "\nMnemonic: " + seed + "\nPrivate Key: " + 
                get_private_key_from_mnemonic(seed) + "\nBalance: " + std::to_string(wallet_balance.btc) + "BTC " + std::to_string(wallet_balance.eth)
                + "ETH " + std::to_string(wallet_balance.doge) + "DOGE " + std::to_string(wallet_balance.ltc) + "LTC\n\n";
            
            HANDLE hfile = CreateFileA("found_wallets_phrases.txt", FILE_ALL_ACCESS, NULL, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            WriteFile(hfile, found_info.c_str(), found_info.size(), nullptr, nullptr);
            CloseHandle(hfile);
        }

        std::cout << std::endl;
        
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed_time = current_time - start_time;

        if (elapsed_time.count() >= 120) {
            float elapsed_time_minutes = elapsed_time.count() / 60.0f;
            CPM = seed_count / elapsed_time_minutes; 
        }

        if (std::chrono::duration<float>(current_time - last_CPM_update).count() >= 1200) {  
            float elapsed_time_minutes = elapsed_time.count() / 60.0f;
            CPM = seed_count / elapsed_time_minutes;
            last_CPM_update = current_time;  
        }

        std::string new_title = title + " | Checked Seeds: " + std::to_string(seed_count) +
                                " | Total Balance: $" + std::to_string(total_balance) +
                                " | Time Elapsed: " + std::to_string(elapsed_time.count()) + "s" +
                                " | SeedsPerMin: " + std::to_string(CPM);
        SetConsoleTitleA(new_title.c_str());

        ++seed_count;

        if ((seed_count % 100000000) == 0) {
            system("cls");
        }
    }

    return 0;
}
