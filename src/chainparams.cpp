// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2019 The Bluecoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include <assert.h>
#include <boost/assign/list_of.hpp>
#include <stdio.h>
using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x0000095dd9ee973b261c54e9a8eec62b7760647854e816ce4d5432f5813fd7ad"));
    
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1548447987,
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    500        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256("0x000005ee2329b5287ecc738140fe50b25445844a77e148c57f52a1d98ae7ece7"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
     1535535865,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of
    (0, uint256("0x00000c232fba062cca70418c046c46428145751cd1217d403a71db29289dd0cc"));	
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1535536258,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x6f;
        pchMessageStart[1] = 0x1d;
        pchMessageStart[2] = 0x89;
        pchMessageStart[3] = 0xc4;
        vAlertPubKey = ParseHex("04400ffefd95cd494bd122bb00be4fdda5a8804c95665bd99a1a146605010ef4582cda9bf9d2da9bd40854ff3d34679ea82a5bd99ab757528056500e96c2a9c96e");
        nDefaultPort = 18898;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Bluecoin starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 350400;   // halving every year ~ 350400 blocks
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 60 * 60; // Bluecoin: 1 hour
        nTargetSpacing = 90;  // Bluecoin: 90 seconds
        nMaturity = 100;  
        nMasternodeCollateral = 10000; // 10000 BLUE
        nMasternodeCountDrift = 20;
        nMaxMoneyOut =  60000000 * COIN; //60M BLUE
        nSwiftTxMinFee = 0.01 * COIN;   

        /** Height or Time Based Activations **/
        nPresaleStartBlock = 9000; // Presale Start
        nPresaleEndBlock = 15000; // Presale End 15000 
        
        nLastPOWBlock = 13000; // POW ends at block 13000
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = -1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = -1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 0; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 1; //Start enforcing the invalid UTXO's

        /** Information for generating Genesis Block**/
        const char* pszTimestamp = "BLUECOIN - The Future of Masternode";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 504365040 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 100 * COIN; // initial reward for genesis block
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0495ac264bb3e9c9f1994922949d0afd282856d8f7f828a6166e7cc06f9fff8e3764aee8db48e869f57e4c7692fc4d1d41852d8fd71247f162cce3acab615fd0dc") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
	    genesis.nTime = 1553367653;
        genesis.nBits = 0x1e0ffff0;
	    genesis.nNonce = 1432745;

        hashGenesisBlock = genesis.GetHash();
	    assert(genesis.hashMerkleRoot == uint256("0x4a5615547e35851c4ca314a0098751f47f185ba5875a758418e6a9e63eefe9ba"));
	    assert(hashGenesisBlock == uint256("0x0000095dd9ee973b261c54e9a8eec62b7760647854e816ce4d5432f5813fd7ad"));
        
        // Zerocoin, disabled
        nZerocoinStartHeight = 2147483647; // Tuesday, January 19, 2038 3:14:07 AM
        nZerocoinStartTime = 2147483647; // Tuesday, January 19, 2038 3:14:07 AM

        // DNS Seeders maintaining a dynamic list of active nodes
        vSeeds.push_back(CDNSSeedData("51.75.64.158", "51.75.64.158"));

        // Base58prefixes Information:  https://en.bitcoin.it/wiki/List_of_address_prefixes      
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);   //B  
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 53);    
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 191);      // 7 or V
        
        // BIP32 format: 0x0420BD3A for spub and 0x0420B900 for sprv        
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x20)(0xBD)(0x3A).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x20)(0xB9)(0x00).convert_to_container<std::vector<unsigned char> >();

        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x19)(0x89).convert_to_container<std::vector<unsigned char> >();

        // Hardcoded seed node generated in chainparamsseeds.h
        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
		strSporkKey = "04412f5f289163166ce3b5917aac5e8fd36970b80e018771231b3792598c8c28702105b04f81e73a9e0f3e14c712e601a701eb1787924ebde792fe7b3c83323dc3";
        strObfuscationPoolDummyAddress = "BGg3VkCbS9xqebf8YEEH5rPdK735wfNtSg";
        nStartMasternodePayments = genesis.nTime + 60 * 60; // 1 hr after genesis 

        /** Zerocoin */
        // See https://github.com/Zerocoin/libzerocoin/wiki/Generating-Zerocoin-parameters
        // http://zerocoin.org/media/pdf/ZerocoinOakland.pdf
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nBudget_Fee_Confirmations = 8; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
	pchMessageStart[0] = 0x6f;
        pchMessageStart[1] = 0x1d;
        pchMessageStart[2] = 0x89;
        pchMessageStart[3] = 0xc3;        
        vAlertPubKey = ParseHex("047a348d6cac6eca2cd79d91815fbc81f4d1cc24745065eef59d82c200c6674085509ff29de80629a87bac82f934ed30d710df01f42d34cbd7fd125b4013ff940d");
        nDefaultPort = 18900;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 60 * 60; // Bluecoin: 1 hour
        nTargetSpacing = 90;  // Bluecoin: 90 seconds
        nLastPOWBlock = 1000;  
        nMaturity = 10;  
        nMasternodeCountDrift = 4;
        //nModifierUpdateBlock = 0; // Bluecoin TBD
        nMaxMoneyOut =  50000000 * COIN; // Bluecoin maxcap 50M
        
        //! Modify the testnet genesis block so the timestamp is valid for a later start.
	    genesis.nTime = 1553368302;
        genesis.nBits = 0x1e0ffff0;
	    genesis.nNonce = 543981;

        hashGenesisBlock = genesis.GetHash();
  	    assert(hashGenesisBlock == uint256("0x000005ee2329b5287ecc738140fe50b25445844a77e148c57f52a1d98ae7ece7"));      
        
        // Zerocoin, disabled by default
        nZerocoinStartHeight = 2147483647; // Tuesday, January 19, 2038 3:14:07 AM
        nZerocoinStartTime = 2147483647;  //Tuesday, January 19, 2038 3:14:07 AM

        vFixedSeeds.clear();
        vSeeds.clear();
        
        // Add seeder node information here
        
        
        // base58Prefixes information 
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 86);    
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 112);    
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 253);       
        
        // 0x0420BD3A for spub and 0x0420B900 for sprv        
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x20)(0xBD)(0x3A).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x20)(0xB9)(0x00).convert_to_container<std::vector<unsigned char> >();
        
        // Testnet bluecoin BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "048a3dea74e006c42e9ddbdf588fe6da7b4d19860f7dc4d0125d647541cd5cb573563a57ea31a48310c657d7b26619779bd4c9e9675a11e3a346c6fe34bf487936";
        strObfuscationPoolDummyAddress = "BGg3VkCbS9xqebf8YEEH5rPdK735wfNtSg";
        nStartMasternodePayments = genesis.nTime + 3600; // 1 hr after genesis
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. 
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
	pchMessageStart[0] = 0x6f;
        pchMessageStart[1] = 0x1d;
        pchMessageStart[2] = 0x89;
        pchMessageStart[3] = 0xc2;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 60 * 60; // Bluecoin: 1 hour
        nTargetSpacing = 90;        // Bluecoin: 90 seconds
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        
	    genesis.nTime = 1553368638;
        genesis.nBits = 0x1e0ffff0;
	    genesis.nNonce = 248271;
	
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18902;
  	    assert(hashGenesisBlock == uint256("0x00000c232fba062cca70418c046c46428145751cd1217d403a71db29289dd0cc"));      
        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 18993;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }
    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
