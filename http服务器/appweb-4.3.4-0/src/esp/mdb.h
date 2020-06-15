/*
    mdb.h -- Memory Database (MDB).

    Copyright (c) All Rights Reserved. See copyright notice at the bottom of the file.
 */

#ifndef _h_MDB
#define _h_MDB 1

/********************************* Includes ***********************************/

#include    "appweb.h"
#include    "edi.h"

#if BIT_PACK_ESP && BIT_PACK_MDB

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Forward Declarations **************************/

#if !DOXYGEN
#endif

/********************************** Tunables **********************************/

#define MDB_INCR    8                   /**< Default memory allocation increment for MDB */

/*
    Per column structure
 */
typedef struct MdbCol {
    MprList         *validations;       /* List of column validations */
    char            *name;              /* Column name */
    int             type;               /* Column type */
    int             flags;              /* Column flags */
    int             cid;                /* Column index in MdbSchema.cols */
    int64           lastValue;          /* Last value if auto-inc */
} MdbCol;

/*
    Table schema
 */
typedef struct MdbSchema {
    int             ncols;              /* Number of columns in table */
    int             capacity;           /* Capacity of cols */
    MdbCol          cols[ARRAY_FLEX];   /* Array of columns */
} MdbSchema;

/*
    Per row structure
 */
typedef struct MdbRow {
    struct MdbTable *table;             /* Reference to MdbTable */
    int             rid;                /* Table index in MdbTable.row */
    int             nfields;            /* Number of fields in fields */
    cchar           *fields[ARRAY_FLEX];
} MdbRow;

/*
    Per table structure
 */
typedef struct MdbTable {
    char            *name;              /* Table name */
    MdbSchema       *schema;            /* Table columns schema */
    MprHash         *index;             /* Table index */
    MdbCol          *keyCol;            /* Reference to the key column */
    MdbCol          *indexCol;          /* Reference to the index column */
    MprList         *rows;              /* Table row */
} MdbTable;

/*
    Mdb flags
 */
#define MDB_LOADING     0x1

/*
    Per database structure
 */
typedef struct Mdb {
    Edi             edi;                /**< EDI database interface structure */
    MprMutex        *mutex;             /**< Multithread lock for Schema modifications only */
    MprList         *tables;            /**< List of tables */

    /*
        When loading from file
     */
    MdbTable        *loadTable;         /* Current table */
    MdbCol          *loadCol;           /* Current column */
    MdbRow          *loadRow;           /* Current row */
    MprList         *loadStack;         /* State stack */
    int             loadState;          /* Current state */
    int             loadNcols;          /* Expected number of cols */
    int             lineNumber;         /* Current line number in path */
} Mdb;


#ifdef __cplusplus
} /* extern C */
#endif

#endif /* BIT_PACK_MDB */
#endif /* _h_MDB */

/*
    @copy   default

    Copyright (c) Embedthis Software LLC, 2003-2013. All Rights Reserved.

    This software is distributed under commercial and open source licenses.
    You may use the Embedthis Open Source license or you may acquire a 
    commercial license from Embedthis Software. You agree to be fully bound
    by the terms of either license. Consult the LICENSE.md distributed with
    this software for full details and other copyrights.

    Local variables:
    tab-width: 4
    c-basic-offset: 4
    End:
    vim: sw=4 ts=4 expandtab

    @end
 */
