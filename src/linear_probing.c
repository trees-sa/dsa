/*
 * linear_probing.c
 * Railway Booking ID Hash Table - Linear Probing collision resolution.
 *
 * Hash function : h(key) = key % M
 * Probe sequence: index(i) = (h(key) + i) % M ,  i = 0,1,2,...
 */

#include <stdio.h>
#include <stdlib.h>

#define M 10          /* table size */
#define EMPTY -1

int table[M];

int hash_fn(int key) {
    return key % M;
}

/* Inserts key, returns number of probes used (>=1), or -1 if table full */
int insert(int key, int *probesOut) {
    int h = hash_fn(key);
    int probes = 0;
    for (int i = 0; i < M; i++) {
        int idx = (h + i) % M;
        probes++;
        if (table[idx] == EMPTY) {
            table[idx] = key;
            *probesOut = probes;
            return idx;
        }
    }
    *probesOut = probes;
    return -1; /* table full */
}

/* Searches key, returns index if found else -1. probesOut = probes used */
int search(int key, int *probesOut) {
    int h = hash_fn(key);
    int probes = 0;
    for (int i = 0; i < M; i++) {
        int idx = (h + i) % M;
        probes++;
        if (table[idx] == EMPTY) {
            *probesOut = probes;
            return -1; /* stopped at empty slot => not found */
        }
        if (table[idx] == key) {
            *probesOut = probes;
            return idx;
        }
    }
    *probesOut = probes;
    return -1;
}

void printTable(void) {
    printf("\nFinal Hash Table (Linear Probing), M = %d\n", M);
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

    printf("=== LINEAR PROBING : INSERTION TRACE ===\n");
    printf("%-8s%-8s%-10s%-10s%-s\n", "Key", "h(key)", "Probes", "SlotUsed", "Sequence tried");
    for (int i = 0; i < n; i++) {
        int probes;
        int h = hash_fn(keys[i]);
        int idxBefore[M], cnt = 0;
        /* recompute the sequence purely for display purposes */
        for (int p = 0; p < M; p++) {
            int idx = (h + p) % M;
            idxBefore[cnt++] = idx;
            if (table[idx] == EMPTY) break;
        }
        int slot = insert(keys[i], &probes);
        totalProbes += probes;
        printf("%-8d%-8d%-10d%-10d", keys[i], h, probes, slot);
        for (int j = 0; j < cnt; j++) printf("%d%s", idxBefore[j], j == cnt - 1 ? "\n" : "->");
    }

    printTable();

    printf("\nInsertion probe summary: total probes = %d, average probes/key = %.3f\n",
           totalProbes, (double) totalProbes / n);

    printf("\n=== LINEAR PROBING : SEARCH TRACE ===\n");
    int searchKeys[] = {23, 63, 14, 100};
    const char *label[] = {"existing", "existing", "non-existing", "non-existing"};
    for (int i = 0; i < 4; i++) {
        int probes;
        int idx = search(searchKeys[i], &probes);
        printf("Search %-4d (%-13s): %-10s probes = %d %s\n",
               searchKeys[i], label[i],
               idx != -1 ? "FOUND" : "NOT FOUND",
               probes,
               idx != -1 ? "" : "");
    }

    return 0;
}
