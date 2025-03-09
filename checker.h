#pragma once  // Prevent multiple inclusions

#include <Windows.h>
#include <Wininet.h>
#include <vector>
#include <string>
#include "wordlist.h"

#pragma comment(lib, "wininet.lib")  // Link WinINet library

struct balance
{
    double btc, eth, ltc, doge;  // Changed float to double
};

const std::vector<std::string> websites = { 
    "https://blockscan.com/", 
    "https://www.blockchain.com/explorer", 
    "https://etherscan.io/", 
    "https://privatekeyfinder.io/mnemonic-converter", 
    "https://mempool.space/", 
    "https://live.blockcypher.com/ltc/", 
    "https://dogechain.info/"
};

namespace inet
{
    std::string read_url(const std::string& url);
    DWORD post_request(const std::string& url, const std::string& request, DWORD* code, std::string* response);
    bool webstatus_check(const std::string& szAddress, DWORD* pdwWebStat);
}

std::string get_wallet_address_from_mnemonic(const std::string& mnemonic);
std::string get_private_key_from_mnemonic(const std::string& mnemonic);

DWORD check_wallet(const std::string& mnemonic, balance* wallet_balance);
bool is_empty(const balance& b);

double get_btc_price();
double get_eth_price();
double get_doge_price();
double get_ltc_price();
