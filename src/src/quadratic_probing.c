/*
 * quadratic_probing.c
 * Railway Booking ID Hash Table - Quadratic Probing collision resolution.
 *
 * Hash function : h(key) = key % M
 * Probe sequence: index(i) = (h(key) + i*i) % M ,  i = 0,1,2,...,M-1
 *
 * NOTE: With a non-prime table size, quadratic probing is NOT guaranteed
 * to visit every slot, even when the table is not full. This program
 * detects that case (insertion fails after M attempts even though empty
 * slots exist) and reports it explicitly -- this is an expected and
 * important experimental finding, not a bug.
 */

#include <stdio.h>
#include <stdlib.h>

#define M 10
#define EMPTY -1

int table[M];

int hash_fn(int key) {
    return key % M;
}

/* Returns slot index on success, -1 if the probe sequence exhausts M
 * attempts without finding an empty slot. probesOut = probes attempted. */
int insert(int key, int *probesOut) {
    int h = hash_fn(key);
    int probes = 0;
    for (int i = 0; i < M; i++) {
        int idx = (h + i * i) % M;
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

/* Search stops (a) on match -> found, or (b) after M probe attempts
 * without hitting an empty slot or a match -> not found (bounded). */
int search(int key, int *probesOut) {
    int h = hash_fn(key);
    int probes = 0;
    for (int i = 0; i < M; i++) {
        int idx = (h + i * i) % M;
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
    printf("\nFinal Hash Table (Quadratic Probing), M = %d\n", M);
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
    int totalProbes = 0, successCount = 0;

    printf("=== QUADRATIC PROBING : INSERTION TRACE ===\n");
    printf("%-8s%-8s%-10s%-10s%-s\n", "Key", "h(key)", "Probes", "SlotUsed", "Sequence tried");
    for (int i = 0; i < n; i++) {
        int probes;
        int h = hash_fn(keys[i]);
        int idxSeq[M], cnt = 0;
        for (int p = 0; p < M; p++) {
            int idx = (h + p * p) % M;
            idxSeq[cnt++] = idx;
            if (table[idx] == EMPTY) break;
        }
        int slot = insert(keys[i], &probes);
        totalProbes += probes;
        if (slot != -1) successCount++;
        printf("%-8d%-8d%-10d%-10s", keys[i], h, probes, slot != -1 ? "" : "FAILED");
        if (slot != -1) printf("%-1d ", slot);
        for (int j = 0; j < cnt; j++) printf("%d%s", idxSeq[j], j == cnt - 1 ? "\n" : "->");
    }

    printTable();

    printf("\nInsertion result: %d / %d keys inserted successfully.\n", successCount, n);
    if (successCount < n) {
        printf("NOTE: Key(s) could NOT be placed -- quadratic probing with M=%d\n"
               "(non-prime) only reaches a limited subset of slots from a given\n"
               "starting index, even though empty slots remain elsewhere in the\n"
               "table. This is a known theoretical limitation of quadratic probing\n"
               "on non-prime table sizes / load factors above 0.5.\n", M);
    }
    printf("\nInsertion probe summary (successful insertions only): total probes = %d, "
           "average probes/key = %.3f\n", totalProbes, (double) totalProbes / n);

    printf("\n=== QUADRATIC PROBING : SEARCH TRACE ===\n");
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
