#include "../../inc/tables.h"

/*
 * Inisitalisation des differentes tables
 */
void init_tables() {
    thash_init();
    tlex_init();
    tdec_init();
    trep_init();
}