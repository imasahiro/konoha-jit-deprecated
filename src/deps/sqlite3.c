/****************************************************************************
 * KONOHA COPYRIGHT, LICENSE NOTICE, AND DISCRIMER
 *
 * Copyright (c) 2005-2009, Kimio Kuramitsu <kimio at ynu.ac.jp>
 *           (c) 2008-      Konoha Software Foundation
 * All rights reserved.
 *
 * You may choose one of the following two licenses when you use konoha.
 * See www.konohaware.org/license.html for further information.
 *
 * (1) GNU Lesser General Public License 3.0 (with KONOHA_UNDER_LGPL3)
 * (2) Konoha Software Foundation License 1.0
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/* ************************************************************************ */

#include"commons.h"

#ifdef KNH_USING_SQLITE3
#include<sqlite3.h>
#endif

#ifdef KNH_USING_MYSQL
#include<mysql/mysql.h>
#endif


/* ************************************************************************ */

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/* [NOP] */

static
knh_db_t *knh_dbopen__NOP(Ctx *ctx, knh_bytes_t url)
{
	return NULL;
}

/* ------------------------------------------------------------------------ */

static
knh_dbcur_t *knh_dbquery__NOP(Ctx *ctx, knh_db_t *hdr, knh_bytes_t sql, knh_ResultSet_t *b)
{
	return NULL;
}

/* ------------------------------------------------------------------------ */

static
void knh_dbclose__NOP(Ctx *ctx, knh_db_t *hdr)
{
	//
}

/* ------------------------------------------------------------------------ */

static
int knh_dbcurnext__NOP(Ctx *ctx, knh_dbcur_t *dbcur, struct knh_ResultSet_t *b)
{
	return 0;  /* NOMORE */
}

/* ------------------------------------------------------------------------ */

void knh_dbcurfree__NOP(knh_dbcur_t *dbcur)
{
	// DO NOTHING
}

/* ------------------------------------------------------------------------ */
/* @data */

static knh_db_drvapi_t DB__NOP = {
	KNH_DRVAPI_TYPE__DB, "NOP",
	knh_dbopen__NOP,
	knh_dbquery__NOP,
	knh_dbclose__NOP,
	knh_dbcurnext__NOP,
	knh_dbcurfree__NOP
};

/* ======================================================================== */
/* [sqlite3] */

void knh_write_USING_SQLITE3(Ctx *ctx, knh_OutputStream_t *w)
{
#ifdef KNH_USING_SQLITE3
	knh_write_char(ctx, w, " sqlite3");
#endif
}

#ifdef KNH_USING_SQLITE3

static
void knh_sqlite3_perror(Ctx *ctx, sqlite3 *db, int r)
{
	char *msg = "SQL!!", buf[512];
	if(r == SQLITE_PERM || r == SQLITE_AUTH) {
		msg = "Security!!";
	}
	knh_snprintf(buf, sizeof(buf), "%s: %s", msg, sqlite3_errmsg(db));
	KNH_WARNING(ctx, buf);
}

/* ------------------------------------------------------------------------ */

static
knh_db_t *knh_dbopen__sqlite3(Ctx *ctx, knh_bytes_t url)
{
	sqlite3 *db = NULL;
	url = knh_bytes_skipscheme(url);
	int r = sqlite3_open((char*)url.buf, &db);
	if (r != SQLITE_OK) {
		return NULL;
	}
	return (knh_db_t*)db;
}

/* ------------------------------------------------------------------------ */

static
int knh_dbcurnext__sqlite3(Ctx *ctx, knh_dbcur_t *dbcur, struct knh_ResultSet_t *rs)
{
	sqlite3_stmt *stmt = (sqlite3_stmt*)dbcur;
	int r = sqlite3_step(stmt);
	if(SQLITE_ROW == r) {
		size_t i;
		knh_ResultSet_initData(ctx, rs);
		for(i = 0; i < DP(rs)->column_size; i++) {
			int type = sqlite3_column_type(stmt, i);
			switch(type) {
				case SQLITE_INTEGER: {
					knh_ResultSet_setInt(ctx, rs, i, (knh_int_t)sqlite3_column_int64(stmt, i));
					break;
				}
				case SQLITE_FLOAT: {
					knh_ResultSet_setFloat(ctx, rs, i, (knh_float_t)sqlite3_column_double(stmt, i));
					break;
				}
				case SQLITE_TEXT: {
					knh_bytes_t t = {(knh_uchar_t*)sqlite3_column_text(stmt,i), sqlite3_column_bytes(stmt, i)};
					knh_ResultSet_setText(ctx, rs, i, t);
					break;
				}
				case SQLITE_BLOB: {
					knh_bytes_t t = {(knh_uchar_t*)sqlite3_column_blob(stmt,i), sqlite3_column_bytes(stmt, i)};
					knh_ResultSet_setBlob(ctx, rs, i, t);
					break;
				}
				case SQLITE_NULL:
				default: {
					knh_ResultSet_setNULL(ctx, rs, i);
				}
			}
		}
		return 1;
	}
	else if (r != SQLITE_DONE) {
	   //
	}
	return 0;  /* NOMORE */
}


/* ------------------------------------------------------------------------ */

static
knh_dbcur_t *knh_dbquery__sqlite3(Ctx *ctx, knh_db_t *hdr, knh_bytes_t sql, knh_ResultSet_t *rs)
{
	if(rs == NULL) {
		int r = sqlite3_exec((sqlite3*)hdr, (const char*)sql.buf, NULL, NULL, NULL);
		if(r != SQLITE_OK) {
			knh_sqlite3_perror(ctx, (sqlite3*)hdr, r);
		}
		return NULL;
	}
	else {
		sqlite3_stmt *stmt = NULL;
		sqlite3_prepare((sqlite3*)hdr, (char*)sql.buf, sql.len, &stmt, NULL);
//	if (r != SQLITE_OK) {
//		sqlite3_finalize(stmt);
//		DBG2_P("msg='%s', sqlite3_errmsg((sqlite3)hdr));
//		return NULL;
//	}
//		r = sqlite3_reset(stmt);
//	if(r != SQLITE_OK) {
//		sqlite3_finalize(stmt);
//		return NULL;
//	}
		size_t column_size = (size_t)sqlite3_column_count(stmt);
		//DBG2_P("column_size=%d", column_size);
		knh_ResultSet_initColumn(ctx, rs, column_size);
		if(column_size > 0) {
			size_t i;
			for(i = 0; i < DP(rs)->column_size; i++) {
				char *n = (char*)sqlite3_column_name(stmt, i);
				//DBG2_P("(%d) name = '%s'", i, n);
				if(n != NULL) {
					knh_ResultSet_setName(ctx, rs, i, new_String(ctx, B(n), NULL));
				}
			}
		}
		return (knh_dbcur_t*)stmt;
	}
}

/* ------------------------------------------------------------------------ */

static
void knh_dbclose__sqlite3(Ctx *ctx, knh_db_t *hdr)
{
	sqlite3_close((sqlite3*)hdr);
}


/* ------------------------------------------------------------------------ */

static
void knh_dbcurfree__sqlite3(knh_dbcur_t *dbcur)
{
	sqlite3_stmt *stmt = (sqlite3_stmt*)dbcur;
	sqlite3_finalize(stmt);
}

/* ------------------------------------------------------------------------ */
/* @data */

static knh_db_drvapi_t DB__sqlite3 = {
	KNH_DRVAPI_TYPE__DB, "sqlite3",
	knh_dbopen__sqlite3,
	knh_dbquery__sqlite3,
	knh_dbclose__sqlite3,
	knh_dbcurnext__sqlite3,
	knh_dbcurfree__sqlite3
};

#endif/*KNH_USING_SQLITE3*/

/* ======================================================================== */
/* [mysql] */

void knh_write_USING_MYSQL(Ctx *ctx, knh_OutputStream_t *w)
{
#ifdef KNH_USING_MYSQL
	knh_write_char(ctx, w, " mysql");
#endif
}

#ifdef KNH_USING_MYSQL

static
void knh_mysql_perror(Ctx *ctx, MYSQL *db, int r)
{
	char *msg = "SQL!!", buf[512];
//	if(r == SQLITE_PERM || r == SQLITE_AUTH) {
//		msg = "Security!!";
//	}
	knh_snprintf(buf, sizeof(buf), "%s: %s", msg, mysql_errmsg(db));
	KNH_WARNING(ctx, buf);
}

/* ------------------------------------------------------------------------ */

static
knh_db_t *knh_dbopen__mysql(Ctx *ctx, knh_bytes_t url)
{
    MYSQL *db = NULL;
    char* server = "localhost";
    char* user   = "user";
    char* passwd = "passwd";
    char* dbname = "dbname";
    int port = 1024;

    url = knh_bytes_skipscheme(url);
    db  = mysql_init(NULL);
    if(mysql_real_connect(db,  server, user, passwd, dbname, port, NULL, 0)==NULL) {
        KNH_THROWs(ctx, "SQL!!: connection error");
    }
    return (knh_db_t*)db;
}

/* ------------------------------------------------------------------------ */

static
int knh_dbcurnext__mysql(Ctx *ctx, knh_dbcur_t *dbcur, struct knh_ResultSet_t *rs)
{
    MYSQL_ROW row;
    int i = 0;
    if((row = mysql_fetch_row((MYSQL_RES*) dbcur)) != NULL) {
        knh_ResultSet_initData(ctx, rs); /* DO NOT TOUCH */
        for(i = 0; i < rs->column_size; i++) {
            if(row[i] == NULL) {
                knh_ResultSet_setNull(ctx, rs, i);
                continue;
            }
            switch(rs->column[i].dbtype) {
                case MYSQL_TYPE_TINY:
                case MYSQL_TYPE_SHORT:
                case MYSQL_TYPE_LONG:
                case MYSQL_TYPE_INT24:
                case MYSQL_TYPE_BIT:
                    knh_ResultSet_setInt(ctx, rs, i, knh_bytes_toint(B(row[i])));
                    break;
                case MYSQL_TYPE_DECIMAL:
                case MYSQL_TYPE_FLOAT:
                case MYSQL_TYPE_DOUBLE:
                    knh_ResultSet_setFloat(ctx, rs, i, knh_bytes_tofloat(B(row[i])));
                    break;
                case MYSQL_TYPE_LONGLONG:
                    knh_ResultSet_setInt(ctx, rs, i, knh_bytes_toint64(B(row[i])));
                    break;
                default:
                    knh_ResultSet_setBytes(ctx, rs, i, B(row[i]));
                    break;
            }
        }
        return 1; /* if you have a tuple */
    }
    return 0;  /* NOMORE */
}


/* ------------------------------------------------------------------------ */

static
knh_dbcur_t *knh_dbquery__mysql(Ctx *ctx, knh_db_t *hdr, knh_bytes_t sql, knh_ResultSet_t *rs)
{
    if(rs == NULL) {
        // rs is NULL
        knh_mysql_perror(ctx, (MYSQL*)hdr, r);
    }
    else {
        MYSQL_RES *res = NULL;
        MYSQL_FIELD *field = NULL;
        int i = 0;
        int r = mysql_query((MYSQL*)hdr, (const char*)sql.buf);
        if(r > 0) {
            // query error
            knh_mysql_perror(ctx, (MYSQL*)hdr, r);
        }
        res = mysql_store_result((MYSQL*)hdr);
        if(res == NULL) {
            if(mysql_error((MYSQL*)hdr)) {
                KNH_THROWs(ctx, "result is NULL");
            } else {
                KNH_THROWs(ctx, "unknown error");
            }
        }
        while((field = mysql_fetch_field(res)) != NULL) {
            rs->column[i].dbtype = field->type;
            i++;
        }
        return (knh_dbcur_t *) res;
    }
}

/* ------------------------------------------------------------------------ */

static
void knh_dbclose__mysql(Ctx *ctx, knh_db_t *hdr)
{
    mysql_close((MYSQL*)hdr);
}


/* ------------------------------------------------------------------------ */

static
void knh_dbcurfree__mysql(knh_dbcur_t *dbcur)
{
	MYSQL_STMT *stmt = (MYSQL_STMT*)dbcur;
	mysql_stmt_close(stmt);
}

/* ------------------------------------------------------------------------ */
/* @data */

static knh_db_drvapi_t DB__mysql = {
	KNH_DRVAPI_TYPE__DB, "mysql",
	knh_dbopen__mysql,
	knh_dbquery__mysql,
	knh_dbclose__mysql,
	knh_dbcurnext__mysql,
	knh_dbcurfree__mysql
};

#endif/*KNH_USING_MYSQL*/


/* ======================================================================== */
/* [drivers] */

knh_db_drvapi_t *knh_System_getDefaultDBDriver(void)
{
	return &DB__NOP;
}

/* ------------------------------------------------------------------------ */

knh_db_drvapi_t *knh_System_getDBDriver(Ctx *ctx, knh_bytes_t name)
{
    if(ctx == NULL) {
        return &DB__NOP;
    }
    else {
        knh_db_drvapi_t *p = (knh_db_drvapi_t *)knh_getDriverAPI(ctx, KNH_DRVAPI_TYPE__DB, name);
        if(p == NULL) {
#ifdef KNHX_SQLITE3_EXCEPTION
            KNH_THROWf(ctx, "DB!!: unsupported scheme '%s'", name);
#else
            KNH_WARNING(ctx, "DB: unsupported scheme '%s'", name);
#endif
            p = &DB__NOP;
        }
        return p;
    }
}

/* ------------------------------------------------------------------------ */

KNHAPI(void) knh_addDBDriver(Ctx *ctx, char *alias, knh_db_drvapi_t *d)
{
	knh_addDriverAPI(ctx, alias, (knh_drvapi_t*)d);
}

/* ======================================================================== */
/* [init] */

void knh_Connection_newClass(Ctx *ctx, knh_class_t cid)
{
	knh_addDBDriver(ctx, NULL, &DB__NOP);
#ifdef KNH_USING_SQLITE3
	knh_addDBDriver(ctx, NULL, &DB__sqlite3);
	knh_addDBDriver(ctx, "sqlite", &DB__sqlite3);
#endif

#ifdef KNH_USING_MYSQL
	knh_addDBDriver(ctx, "mysql", &DB__mysql);
#endif
}

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
