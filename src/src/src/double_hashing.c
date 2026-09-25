
#include <stdio.h>
#include <stdlib.h>

#define M 10
#define R 7          /* auxiliary prime, R < M */
#define EMPTY -1

int table[M];

int h1(int key) { return key % M; }
int h2(int key) { return R - (key % R); } /* always in range [1, R] i.e. never 0 */

int insert(int key, int *probesOut) {
    int base = h1(key), step = h2(key);
    int probes = 0;
    for (int i = 0; i < M; i++) {
        int idx = (base + i * step) % M;
        probes++;
        if (table[idx] == EMPTY) {
            table[idx] = key;
            *probesOut = probes;
            return idx;
        }
    }
    *probesOut = probes;
    return -1;
}

int search(int key, int *probesOut) {
    int base = h1(key), step = h2(key);
    int probes = 0;
    for (int i = 0; i < M; i++) {
        int idx = (base + i * step) % M;
        probes++;
        if (table[idx] == key) {
            *probesOut = probes;
            return idx;
        }
        if (table[idx] == EMPTY) {
            *probesOut = probes;
            return -1;
        }
    }
    *probesOut = probes;
    return -1;
}

void printTable(void) {
    printf("\nFinal Hash Table (Double Hashing), M = %d\n", M);
    printf("Index : Value\n");
    for (int i = 0; i < M; i++) {
        if (table[i] == EMPTY)
            printf("  %2d  :  -\n", i);
        else
            printf("  %2d  : %2d\n", i, table[i]);
    }
}

int main(void) {
    for (int i = 0; i < M; i++) table[i] = EMPTY;

    int keys[] = {23, 43, 13, 33, 53, 63, 73};
    int n = sizeof(keys) / sizeof(keys[0]);
    int totalProbes = 0;

    printf("=== DOUBLE HASHING : INSERTION TRACE ===\n");
    printf("%-8s%-8s%-8s%-10s%-10s%-s\n", "Key", "h1", "h2", "Probes", "SlotUsed", "Sequence tried");
    for (int i = 0; i < n; i++) {
        int probes;
        int base = h1(keys[i]), step = h2(keys[i]);
        int idxSeq[M], cnt = 0;
        for (int p = 0; p < M; p++) {
            int idx = (base + p * step) % M;
            idxSeq[cnt++] = idx;
            if (table[idx] == EMPTY) break;
        }
        int slot = insert(keys[i], &probes);
        totalProbes += probes;
        printf("%-8d%-8d%-8d%-10d%-10d", keys[i], base, step, probes, slot);
        for (int j = 0; j < cnt; j++) printf("%d%s", idxSeq[j], j == cnt - 1 ? "\n" : "->");
    }

    printTable();

    printf("\nInsertion probe summary: total probes = %d, average probes/key = %.3f\n",
           totalProbes, (double) totalProbes / n);

    printf("\n=== DOUBLE HASHING : SEARCH TRACE ===\n");
    int searchKeys[] = {23, 63, 14, 100};
    const char *label[] = {"existing", "existing", "non-existing", "non-existing"};
    for (int i = 0; i < 4; i++) {
        int probes;
        int idx = search(searchKeys[i], &probes);
        printf("Search %-4d (%-13s): %-10s probes = %d\n",
               searchKeys[i], label[i],
               idx != -1 ? "FOUND" : "NOT FOUND",
               probes);
    }

    return 0;
}
