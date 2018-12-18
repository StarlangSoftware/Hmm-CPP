//
// Created by Olcay Taner YILDIZ on 24/10/2018.
//

#ifndef DATASTRUCTURE_COUNTERHASHMAP_H
#define DATASTRUCTURE_COUNTERHASHMAP_H

#include <map>
using namespace std;

template <class K> class CounterHashMap : public map<K, int> {
private:
    bool containsKey(K key);
public:
    CounterHashMap();
    void put(K key);
    void putNTimes(K key, int N);
    int count(K key);
    int sumOfCounts();
    K max();
    K max(double threshold);
    void add(CounterHashMap<K> toBeAdded);
    vector<pair<K, int>> topN(int N);
    string to_string();
};


#endif //DATASTRUCTURE_COUNTERHASHMAP_H
