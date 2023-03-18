#include "speed.h"
#include "../test/test.h"
#include "../main/db_file.h"
#include "util.h"

void bench_speed(int loops, int amount) {
    print_running("BENCHMARK SPEED");

    db_handler* db = open_db_file("bench-speed");
    insert_schema(db);

    double* insertDocumentsTime = malloc(sizeof(double)*loops);
    double* findAllTime = malloc(sizeof(double)*loops);
    double* findTrueTime = malloc(sizeof(double)*loops);

    for (int i = 0; i < loops; ++i) {
        clock_t beforeInsert = clock();
        clock_t afterInsert, afterFindAll, afterFindTrue;
        insert_documents(db, amount, &afterInsert);
        collection_find_all(db, &afterFindAll);
        collection_find_true(db, &afterFindTrue);

        insertDocumentsTime[i] = ((double) (afterInsert - beforeInsert)) / CLOCKS_PER_SEC;
        findAllTime[i] = ((double) (afterFindAll - afterInsert)) / CLOCKS_PER_SEC;
        findTrueTime[i] = ((double) (afterFindTrue - afterFindAll)) / CLOCKS_PER_SEC;
    }

    printf("RESULT:\n");
    for (int i = 0; i < loops; ++i) {
        printf("%d, %f, %f, %f\n", i, insertDocumentsTime[i], findAllTime[i], findTrueTime[i]);
    }
    printf("__________________________\n");

    utilize_db_file(db);

    free(insertDocumentsTime);
    free(findAllTime);
    free(findTrueTime);
}