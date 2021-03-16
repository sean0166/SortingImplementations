
#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>

using namespace std;
//This can also use std template type as oposed to std::function
template <typename T>
using ComparisonFunc = std::function<bool (T, T)>;

//Slow
template <typename T>
void bubble_sort(vector<T>& v, const ComparisonFunc<T>& comparison)
{
    size_t length = v.size();
    for (size_t i = 0; i < length - 1; i++)
    {
        for (size_t j = 0; j < length - 1; j++)
        {
            if (comparison(v.at(j + 1), v.at(j) ))
                iter_swap(v.begin() + j, v.begin() + j + 1);
            
        }
    }
}

//Slow
template <typename T>
void selection_sort(vector<T>& v, const ComparisonFunc<T>& comparison)
{
    size_t length = v.size();
    size_t swapIndex; 

    for (size_t i = 0; i < length; i++)
    {
        T currentSwapVal = v.at(i);
        swapIndex = i;

        for (size_t j = i+1; j < length; j++)
        {
            if (comparison(v.at(j), currentSwapVal))
            {
                currentSwapVal = v.at(j);
                swapIndex = j;
            }
        }
        iter_swap(v.begin() + i, v.begin() + swapIndex);
    }

}

//Good for nearly sorted array
template<typename T>
void insertion_sort(vector<T>& v, const ComparisonFunc<T>& comparison)
{
    size_t listSize = v.size();
    for (size_t i = 1; i < listSize; i++)
    {
        auto currentEle = move(v.at(i));
        for (size_t j = 0; j < i; j++)
        {
            if (comparison(currentEle, v.at(j)))
            {
                v.erase(v.begin() + i);
                v.insert(v.begin() + j, currentEle);

                break;
            }
        }
    }
}

template<typename T>
vector<T> merge(const vector<T>& left, const vector<T>& right, const ComparisonFunc<T>& comparison)
{
    const auto leftLength = left.size(); 
    const auto rightLength = right.size();
    vector<T> mergeAry;
    size_t leftIndex = 0;
    size_t rightIndex = 0;
    
    while (leftIndex < leftLength && rightIndex < rightLength)
    {
        if (comparison(left.at(leftIndex), right.at(rightIndex)))
        {
            mergeAry.emplace_back(left.at(leftIndex));
            leftIndex++;
        }
        else
        {
            mergeAry.emplace_back(right.at(rightIndex));
            rightIndex++;
        }
    }
    //Insert the remaining parts of right and left array to the merge array (Only one of the two arrays will have leftovere values)
    leftIndex < leftLength ?
        mergeAry.insert(mergeAry.end(), left.begin() + leftIndex, left.end()) :
        mergeAry.insert(mergeAry.end(), right.begin() + rightIndex, right.end());

    return mergeAry;
}
/************************************************************************************************************************************************
* merge_sort function splits array in two before calling merge on seperated parts
* merge function does comparisons and sorting of divided arrays
* O(log(n)) - number of levels of tree
* O(n) - sorting a single level of tree is o(n) time
*
* EXAMPLE CALL STACK OF MERGE SORT USING RECURSION: 
* Using Ary[9]:
*
*                                                                                                                merge( subAry[1], subAry[1])
*                                                                                                                        /
*    merge( subAry[1], subAry[1])	merge( subAry[1], subAry[1])  merge( subAry[1], subAry[1])  merge( subAry[1], subAry[2])
*                    \			         /							       \                    ^
*                 merge( subAry[2], subAry[2])				   merge( subAry[2], subAry[3])_____|
*                                      \				           /
*                                       \				      /
*                                        merge( subAry[4], subAry[5])
*
************************************************************************************************************************************************/
template<typename T>
vector<T> merge_sort(const vector<T>& v, const ComparisonFunc<T>& comparison)
{
    if (v.size() == 1) return v;
    
    const auto length = v.size();
    const auto midIndx = length / 2;
    auto leftAry = vector<T>(v.begin(), v.begin() + midIndx);
    auto rightAry = vector<T>(v.begin() + midIndx, v.end());

    return merge(
        merge_sort(leftAry, comparison),
        merge_sort(rightAry, comparison),
        comparison
    );
}

namespace IteratorOnly
{

    template<typename ForwardIt, typename Comparator = std::less<typename std::iterator_traits<ForwardIt>::value_type>>
    void selection_sort(ForwardIt first, ForwardIt last, Comparator cmp = Comparator())
    {
        using category = typename std::iterator_traits<ForwardIt>::iterator_category;
        static_assert(std::is_base_of<std::forward_iterator_tag, category>::value,
            "This selection_sort algorithm requires forward iterators.");

        if (first != last)
        {
            for (ForwardIt curIt = first; curIt != last; ++curIt)
            {
                ForwardIt swapIt = curIt;
                for (ForwardIt nextIt = std::next(curIt); nextIt != last; nextIt++)
                {
                    if (cmp(*nextIt, *swapIt)) swapIt = nextIt;
                }
                swap(*curIt, *swapIt);
            }
        }
    }

    template<typename BidirIt, typename Comparator = std::less<typename std::iterator_traits<BidirIt>::value_type>>
    void insertion_sort(BidirIt first, BidirIt last, Comparator cmp = Comparator())
    {
        using category = typename std::iterator_traits<BidirIt>::iterator_category;
        static_assert(std::is_base_of<std::bidirectional_iterator_tag, category>::value, 
            "This insertion_sort algorithm requires bidirectional iterators.");

        if (first != last)
        {
            for (BidirIt cur = first; ++cur != last;)
            {
                auto val = std::move(*cur);
                BidirIt next = cur, prev = std::prev(next);
                while (cmp(val, *prev))
                {
                    *next = std::move(*prev);
                    next = prev;
                    if (prev != first) --prev;
                    else break;
                }
                *next = std::move(val);
            }
        }
    }
}

int main()
{
    const ComparisonFunc<int> lessThanFunc = [](int x, int y) -> bool
    {
        return x < y;
    };

    const auto printList = [](int x) {
        cout << x << " ";
    };
    vector<int> list{1, 22, 99, 44, 6, 2, 1, 5, 63, 87, 283, 4, 0, 2222, 1 };
    forward_list<int> ll{ 1, 22, 99, 44, 6, 2, 1, 5, 63, 87, 283, 4, 0, 2222, 1 };

    list = merge_sort(list, lessThanFunc);

    for_each(list.begin(), list.end(), printList);

}
