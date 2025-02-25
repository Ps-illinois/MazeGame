/* Your code here! */

#include "dsets.h"

void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; i++) {
        list.push_back(-1);
    }
    size_ += num;
}
 
int DisjointSets::find(int elem) {
    if (list[elem] < 0) {
        return elem;
    } else {
        list[elem] = find(list[elem]);
        return list[elem];
    }
}



void DisjointSets::setunion(int x, int y) {
    int a = find(x);
    int b = find (y);
    if (x != y) {
        if (size(a) < size(b)) {
            list[b] += list[a];
            list[a] = b;
        } else {
            list[a] += list[b];
            list[b] = a;
        }
    }


}
    
    /**
     * This function should return the number of nodes in the up-tree containing 
     * the element.
     * @return number of nodes in the up-tree containing the element
     */
int DisjointSets::size(int elem) {
    return -list[find(elem)];
}
