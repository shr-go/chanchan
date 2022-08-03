// Author: shr-go

#include <benchmark/benchmark.h>
#include <string>
#include "Reflection.h"
#include "SQLCore.h"


struct account {
    int dollar;
    int yen;
};
REFLECT(account, dollar, yen);

struct person {
    std::string name;
    int age;
    account money;
};

REFLECT(person, name, money, age);

static void BM_CreateTable(benchmark::State& state) {
    std::string s;
    benchmark::DoNotOptimize(s);
    for (auto _ : state) {
        s = chanchan::createTable<person>();
    }
}

static void BM_InsertRecord(benchmark::State& state) {
    std::string s;
    benchmark::DoNotOptimize(s);
    person p {"Jack", 20, {100, 200}};
    for (auto _ : state) {
        s = chanchan::insertRecord(p);
    }
}

static void BM_InsertMultiRecord(benchmark::State& state) {
    std::string s;
    benchmark::DoNotOptimize(s);
    person p {"Jack", 20, {100, 200}};
    std::vector<person> vp;
    for (int i = 0; i < 20; ++i) {
        vp.push_back(p);
    }
    for (auto _ : state) {
        s = chanchan::insertRecord(vp);
    }
}

static void BM_UpdateRecord(benchmark::State& state) {
    std::string s;
    benchmark::DoNotOptimize(s);
    person p {"Jack", 20, {100, 200}};
    for (auto _ : state) {
        s = chanchan::updateRecord(p);
    }
}

BENCHMARK(BM_CreateTable);
BENCHMARK(BM_InsertRecord);
BENCHMARK(BM_InsertMultiRecord);
BENCHMARK(BM_UpdateRecord);
BENCHMARK_MAIN();
