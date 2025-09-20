# cpp09

# Merge-Insertion Sort (Ford–Johnson Algorithm)

## Overview
Merge-insertion sort, also known as the **Ford–Johnson algorithm**, is a comparison-based sorting algorithm introduced in 1959 by **L. R. Ford Jr.** and **Selmer M. Johnson**. For many years, it was considered the algorithm with the fewest comparisons in the worst-case scenario, outperforming other known algorithms like binary insertion sort and merge sort. Although it's not used in practical applications today, it remains of theoretical interest, especially in the study of sorting with a minimal number of comparisons.

---

## Algorithm Steps

1. **Pairing Elements:**
    - Group the input elements \( X \) (of size \( n \)) into pairs. If \( n \) is odd, one element will be left unpaired.

2. **Comparing Pairs:**
    - Perform \( \lfloor n / 2 \rfloor \) comparisons to identify the larger element in each pair.

3. **Recursive Sorting:**
    - Recursively sort the larger elements from each pair using merge-insertion sort. This will give a sorted sequence \( S \) of \( \lfloor n / 2 \rfloor \) elements.

4. **Inserting the Smallest Element:**
    - Insert the smallest unpaired element into the sorted sequence \( S \).

5. **Inserting Remaining Elements:**
    - For the rest of the elements not yet inserted into \( S \), insert them one by one using binary search. This search is performed on subsequences of \( S \), ensuring minimal comparisons.

    **Special Ordering for Insertion:**
    - The insertion order is carefully chosen so that binary search is most efficient. After the recursive sorting step, the elements are ordered in groups, with smaller index elements placed last in each group. The uninserted elements are then inserted using binary search into their correct position in \( S \).

---

## Analysis of Comparisons

Let \( C(n) \) be the total number of comparisons required to sort \( n \) elements. This total can be broken down into three parts:

1. Comparisons among the pairs.
2. Comparisons made during the recursive sorting call.
3. Comparisons for binary insertions.

The worst-case number of comparisons for each group during the binary insertions is dependent on the group’s size. The worst-case for the first group is two comparisons, and the worst-case increases with each subsequent group.

The recurrence relation used to calculate the total number of comparisons gives the formula:

\[
C(n) \approx n \log_2 n - 1.415n
\]

For specific values of \( n \), the number of comparisons follows this pattern:

\[
0, 1, 3, 5, 7, 10, 13, 16, 19, 22, 26, 30, 34, \dots
\]

*(This sequence is identified as A001768 in the OEIS).*

---

## Relation to Other Sorting Algorithms

- **Merge-insertion sort** is a hybrid of **merge sort** and **insertion sort**:
  - It performs initial pairwise comparisons similar to merge sort.
  - It inserts elements into a sorted sequence like insertion sort, using binary search.

- For smaller values of \( n \) (up to 11 elements), it achieves the theoretical lower bound on comparison sorting. However, for larger inputs, the number of comparisons exceeds this bound.

- For \( n \leq 22 \), merge-insertion sort is the most efficient algorithm in terms of comparisons. However, other algorithms, like the one introduced by **Glenn Manacher** in 1979, use fewer comparisons for larger inputs.

---

## Conclusion

Merge-insertion sort was the most efficient algorithm in terms of comparisons for two decades, but newer algorithms have since been developed. Nonetheless, the **Ford–Johnson algorithm** remains an important part of the theoretical study of sorting, and its ideas have influenced subsequent sorting algorithms.
