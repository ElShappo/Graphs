#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "PriorityQueue.hpp"
#include "Generator.hpp"

#include <vector>

TEST_CASE("'add' method in PriorityQueue")
{
    PriorityQueue<int, float> priorityQueue(My::Sequence<int>::toSequence(myGenerator<int>(10, 1000, 20)), My::Sequence<float>::toSequence(myGenerator<float>(10, 1000, 20)));
    PriorityQueue<int, float> other = priorityQueue;

    CHECK(priorityQueue == other);
    CHECK(priorityQueue.size() == 20);

    priorityQueue.add(5789, 47.56);
    priorityQueue.add(5789, 50.444);
    priorityQueue.add(525, 39.67897);

    CHECK(priorityQueue.size() == 23);
    CHECK(priorityQueue.peekLast() == Approx(50.444));
    CHECK(priorityQueue[priorityQueue.size()-1] == Approx(50.444));
    CHECK(priorityQueue != other);
}

TEST_CASE("PriorityQueue(My::Sequence<P>& , My::Sequence<T>&")
{
    std::vector<int> prioritiesBuff = {5890, 789, 1789, 1698, 127, 6, 8};
    std::vector<int> valuesBuff = {689, 202689, 619, 1678};

    My::Sequence<int> pBuff = My::Sequence<int>::toSequence(prioritiesBuff);
    My::Sequence<int> vBuff = My::Sequence<int>::toSequence(valuesBuff);

    REQUIRE_THROWS([&]()
    {
        PriorityQueue<int, int> priorityQueue(pBuff, vBuff);
    }());

    My::Sequence<int> pBuff1;
    My::Sequence<float> vBuff1;

    pBuff1 = My::Sequence<int>::toSequence(myGenerator<int>(-100, 100, 50));
    vBuff1 = My::Sequence<float>::toSequence(myGenerator<float>(-100, 100, 50));

    REQUIRE_NOTHROW([&]()
    {
        PriorityQueue<int, float> priorityQueue(pBuff1, vBuff1);
        CHECK(priorityQueue.size() == 50);
    }());
}

TEST_CASE("'pop_back' and 'append' methods in PriorityQueue")
{
    std::vector<int> prioritiesBuff = {56789, 32789, 1298, 1289, 578126};
    std::vector<int> valuesBuff = {689, 202689, 619, 1678, 345};

    My::Sequence<int> pBuff = My::Sequence<int>::toSequence(prioritiesBuff);
    My::Sequence<int> vBuff = My::Sequence<int>::toSequence(valuesBuff);

    PriorityQueue<int, int> priorityQueue(pBuff, vBuff);
    My::Sequence<int> popped;

    REQUIRE_NOTHROW([&]()
    {
        CHECK(priorityQueue.size() == 5);

        popped.push_back(priorityQueue.pop_back());
        CHECK(priorityQueue.size() == 4);

        popped.push_back(priorityQueue.pop_back());
        CHECK(priorityQueue.size() == 3);

        popped.push_back(priorityQueue.pop_back());
        CHECK(priorityQueue.size() == 2);

        popped.push_back(priorityQueue.pop_back());
        CHECK(priorityQueue.size() == 1);

        popped.push_back(priorityQueue.pop_back());
        CHECK(priorityQueue.size() == 0);
    }());

    REQUIRE_THROWS([&]()
    {
        priorityQueue.pop_back();
    }());

    My::Sequence<int> newPriorities = My::Sequence<int>::toSequence(myGenerator<int>(10, 1000, 5));
    priorityQueue.append(newPriorities, popped);
    CHECK(priorityQueue.size() == 5);
    REQUIRE_THROWS(priorityQueue[5]);
}

