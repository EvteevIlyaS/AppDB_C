/* Processed by ecpg (13.2) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "appdb.pgc"
#include <iostream>
#include <string>

using namespace std;


#line 1 "/usr/local/include/sqlca.h"
#ifndef POSTGRES_SQLCA_H
#define POSTGRES_SQLCA_H

#ifndef PGDLLIMPORT
#if  defined(WIN32) || defined(__CYGWIN__)
#define PGDLLIMPORT __declspec (dllimport)
#else
#define PGDLLIMPORT
#endif							/* __CYGWIN__ */
#endif							/* PGDLLIMPORT */

#define SQLERRMC_LEN	150

#ifdef __cplusplus
extern "C"
{
#endif

struct sqlca_t
{
	char		sqlcaid[8];
	long		sqlabc;
	long		sqlcode;
	struct
	{
		int			sqlerrml;
		char		sqlerrmc[SQLERRMC_LEN];
	}			sqlerrm;
	char		sqlerrp[8];
	long		sqlerrd[6];
	/* Element 0: empty						*/
	/* 1: OID of processed tuple if applicable			*/
	/* 2: number of rows processed				*/
	/* after an INSERT, UPDATE or				*/
	/* DELETE statement					*/
	/* 3: empty						*/
	/* 4: empty						*/
	/* 5: empty						*/
	char		sqlwarn[8];
	/* Element 0: set to 'W' if at least one other is 'W'	*/
	/* 1: if 'W' at least one character string		*/
	/* value was truncated when it was			*/
	/* stored into a host variable.             */

	/*
	 * 2: if 'W' a (hopefully) non-fatal notice occurred
	 */	/* 3: empty */
	/* 4: empty						*/
	/* 5: empty						*/
	/* 6: empty						*/
	/* 7: empty						*/

	char		sqlstate[5];
};

struct sqlca_t *ECPGget_sqlca(void);

#ifndef POSTGRES_ECPG_INTERNAL
#define sqlca (*ECPGget_sqlca())
#endif

#ifdef __cplusplus
}
#endif

#endif

#line 6 "appdb.pgc"


#line 1 "/usr/local/include/pgtypes_date.h"
/* src/interfaces/ecpg/include/pgtypes_date.h */

#ifndef PGTYPES_DATETIME
#define PGTYPES_DATETIME

#include <pgtypes.h>
#include <pgtypes_timestamp.h>

typedef long date;

#ifdef __cplusplus
extern "C"
{
#endif

extern date * PGTYPESdate_new(void);
extern void PGTYPESdate_free(date *);
extern date PGTYPESdate_from_asc(char *, char **);
extern char *PGTYPESdate_to_asc(date);
extern date PGTYPESdate_from_timestamp(timestamp);
extern void PGTYPESdate_julmdy(date, int *);
extern void PGTYPESdate_mdyjul(int *, date *);
extern int	PGTYPESdate_dayofweek(date);
extern void PGTYPESdate_today(date *);
extern int	PGTYPESdate_defmt_asc(date *, const char *, const char *);
extern int	PGTYPESdate_fmt_asc(date, const char *, char *);

#ifdef __cplusplus
}
#endif

#endif							/* PGTYPES_DATETIME */

#line 7 "appdb.pgc"


void create_db(){
    { ECPGsetcommit(__LINE__, "on", NULL);}
#line 10 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create database grocery_shop_cpp", ECPGt_EOIT, ECPGt_EORT);}
#line 11 "appdb.pgc"

    { ECPGsetcommit(__LINE__, "off", NULL);}
#line 12 "appdb.pgc"

    cout << "База данных создана\n";
}

void cr_table(){
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create procedure create_table ( ) as $$\
	create table grocery (\
		ProductID serial primary key,\
		ProductPlace varchar (50) not null,\
		IsRemoved varchar (50) not null\
	);\
    $$ language sql", ECPGt_EOIT, ECPGt_EORT);}
#line 23 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call create_table ( )", ECPGt_EOIT, ECPGt_EORT);}
#line 24 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 25 "appdb.pgc"

    cout << "Таблица создана\n";
}

void roles_proc(){
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create or replace procedure create_start_guest ( ) as $$\
        begin\
            create role guest_ password '1' login;\
            grant select on table grocery to guest_;\
        end;\
    $$ language plpgsql", ECPGt_EOIT, ECPGt_EORT);}
#line 35 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 36 "appdb.pgc"


    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create or replace procedure create_guest ( ) as $$\
        begin\
            create role new_guest password '1' login;\
            grant select on table grocery to new_guest;\
        end;\
    $$ language plpgsql", ECPGt_EOIT, ECPGt_EORT);}
#line 43 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 44 "appdb.pgc"


        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create or replace procedure create_admin ( ) as $$\
        begin\
            create role new_admin password '1' superuser;\
        end;\
    $$ language plpgsql", ECPGt_EOIT, ECPGt_EORT);}
#line 50 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 51 "appdb.pgc"

}

int create_db_role(bool isFirstTime){
    int choice;

    if (!isFirstTime) {
        cout << "Выберите создаваемого пользователя:\n";
        cout << "1 - Администратор\n";
        cout << "2 - Гость\n";

        cin >> choice;
        while (choice != 1 && choice != 2){
            cout << "Неправильное значение, попробуйте снова\n";
            cin >> choice;
        }

        switch (choice){
            case 1:
                { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call create_admin ( )", ECPGt_EOIT, ECPGt_EORT);}
#line 70 "appdb.pgc"

                if (sqlca.sqlcode != 0){
                    cout << "Дополнительный супер-пользователь уже создан\n";
                    return 0;
                }
                { ECPGtrans(__LINE__, NULL, "commit");}
#line 75 "appdb.pgc"

                break;
            case 2:
                { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call create_guest ( )", ECPGt_EOIT, ECPGt_EORT);}
#line 78 "appdb.pgc"

                if (sqlca.sqlcode != 0){
                    cout << "Дополнительный гостевой пользователь уже создан\n";
                    return 0;
                }
                { ECPGtrans(__LINE__, NULL, "commit");}
#line 83 "appdb.pgc"

                break;
        }
        cout << "Пользователь создан\n";
        return 1;
    }
    else {
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call create_start_guest ( )", ECPGt_EOIT, ECPGt_EORT);}
#line 90 "appdb.pgc"

        { ECPGtrans(__LINE__, NULL, "commit");}
#line 91 "appdb.pgc"

        cout << "Гостевой пользователь создан\n";
        return 1;
    }
}

void fill_table(){
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create procedure fill_table ( ) as $$\
	INSERT INTO grocery VALUES\
    (001, 'Холодильник', true),\
    (002, 'Стелаж', false),\
    (003, 'Склад', false)\
    $$ language sql", ECPGt_EOIT, ECPGt_EORT);}
#line 103 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call fill_table ( )", ECPGt_EOIT, ECPGt_EORT);}
#line 104 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 105 "appdb.pgc"

    cout << "Таблица заполнена\n";
}

int create_conn(const char *t, const char *u, const char *p){
    /* exec sql begin declare section */
     
        
        
        
    
#line 111 "appdb.pgc"
 char dbname [ 1024 ] ;
 
#line 112 "appdb.pgc"
 const char * target = t ;
 
#line 113 "appdb.pgc"
 const char * user = u ;
 
#line 114 "appdb.pgc"
 const char * passwd = p ;
/* exec sql end declare section */
#line 115 "appdb.pgc"

    { ECPGconnect(__LINE__, 0, target , user , passwd , "con", 0); }
#line 116 "appdb.pgc"

    if(sqlca.sqlcode!=0){
            return 0;
        }
    { ECPGtrans(__LINE__, NULL, "commit");}
#line 120 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select current_database ( )", ECPGt_EOIT, 
	ECPGt_char,(dbname),(long)1024,(long)1,(1024)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 121 "appdb.pgc"

    cout << "Подключенная база - " << dbname << endl;
    return 1;
}

void clear_table(){
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call clear_table ( )", ECPGt_EOIT, ECPGt_EORT);}
#line 127 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 128 "appdb.pgc"

    cout << "Таблица упешно очищена\n";
}

int ins_row(){
    cout << "Введите данные новой строки\n";
    cout << "ID - int:\n";
    int i;
    cin >> i;
    if (cin.fail()){
        cout << "Неверный ввод ID\n";
        return 0;
    }
        
    cout << "Местоположение товара - String:\n";
    string p;
    cin >> p;
    if (cin.fail()){
        cout << "Неверный ввод местоположения\n";
        return 0;
    }

    cout << "Не утилизирован ли товар - boolean:\n";
    bool r;
    cin >> r;
    if (cin.fail()){
        cout << "Неверный ввод утилизированности товара\n";
        return 0;
    }

    /* exec sql begin declare section */
       
        
       
    
#line 159 "appdb.pgc"
 int id = i ;
 
#line 160 "appdb.pgc"
 const char * place = p . c_str () ;
 
#line 161 "appdb.pgc"
 bool rem = r ;
/* exec sql end declare section */
#line 162 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call insert_row ( $1  , $2  , $3  )", 
	ECPGt_int,&(id),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(place),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_bool,&(rem),(long)1,(long)1,sizeof(bool), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 163 "appdb.pgc"

    if (sqlca.sqlcode != 0){
        cout << "Невозможно добавить указанную строку (повторение первичного ключа)\n";
        return 0;
    }
    { ECPGtrans(__LINE__, NULL, "commit");}
#line 168 "appdb.pgc"


    cout << "Поле упешно добавлено\n";
    return 1;
}

int find_rows(){
    cout << "Введите местоположение товара для поиска:\n";
    string p;
    cin >> p;
    if (cin.fail()){
        cout << "Неверный ввод местоположения\n";
        return 0;
    }

    /* exec sql begin declare section */
        
     
     
     
    
#line 184 "appdb.pgc"
 const char * place = p . c_str () ;
 
#line 185 "appdb.pgc"
 int id ;
 
#line 186 "appdb.pgc"
  struct varchar_1  { int len; char arr[ 32 ]; }  pp ;
 
#line 187 "appdb.pgc"
  struct varchar_2  { int len; char arr[ 32 ]; }  rem ;
/* exec sql end declare section */
#line 188 "appdb.pgc"


    ECPGset_var( 0, &( place ), __LINE__);\
 /* declare search_res cursor for select * from find_rows ( $1  ) */
#line 190 "appdb.pgc"

#line 190 "appdb.pgc"


    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare search_res cursor for select * from find_rows ( $1  )", 
	ECPGt_char,&(place),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 192 "appdb.pgc"

    while(1){
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch search_res", ECPGt_EOIT, 
	ECPGt_int,&(id),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_varchar,&(pp),(long)32,(long)1,sizeof(struct varchar_1), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_varchar,&(rem),(long)32,(long)1,sizeof(struct varchar_2), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 194 "appdb.pgc"

        if(sqlca.sqlcode!=0){
            break;
        }
        printf("id=%d, productplace=%s, isremoved=%s\n", id, pp.arr, rem.arr);
    }
    
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close search_res", ECPGt_EOIT, ECPGt_EORT);}
#line 201 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 202 "appdb.pgc"

    return 1;
}


void drop_db(){
    { ECPGdisconnect(__LINE__, "ALL");}
#line 208 "appdb.pgc"

    create_conn("TASK_HSE@localhost:5432", "postgres", "8850");
    { ECPGsetcommit(__LINE__, "on", NULL);}
#line 210 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "drop database grocery_shop_cpp", ECPGt_EOIT, ECPGt_EORT);}
#line 211 "appdb.pgc"

    { ECPGsetcommit(__LINE__, "off", NULL);}
#line 212 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create or replace procedure drop_role_ ( ) as $$\
        drop role guest_;\
    $$ language sql", ECPGt_EOIT, ECPGt_EORT);}
#line 215 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create or replace procedure drop_role_new_guest ( ) as $$\
        drop role new_guest;\
    $$ language sql", ECPGt_EOIT, ECPGt_EORT);}
#line 218 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create or replace procedure drop_role_new_admin ( ) as $$\
        drop role new_admin;\
    $$ language sql", ECPGt_EOIT, ECPGt_EORT);}
#line 221 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call drop_role_ ( )", ECPGt_EOIT, ECPGt_EORT);}
#line 222 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 223 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call drop_role_new_guest ( )", ECPGt_EOIT, ECPGt_EORT);}
#line 224 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 225 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call drop_role_new_admin ( )", ECPGt_EOIT, ECPGt_EORT);}
#line 226 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 227 "appdb.pgc"

    cout << "База данных удалена\n";
    { ECPGdisconnect(__LINE__, "ALL");}
#line 229 "appdb.pgc"

}

int update_row(){
    cout << "Введите ID, по которому обновляеются данные:\n";
    int i;
    cin >> i;
    if (cin.fail()){
        cout << "Неверный ввод ID\n";
        return 0;
    }
        
    cout << "Введите новое местоположение товара:\n";
    string p;
    cin >> p;
    if (cin.fail()){
        cout << "Неверный ввод местоположения\n";
        return 0;
    }

    cout << "Введите новые данные об утилизованности товара:\n";
    bool r;
    cin >> r;
    if (cin.fail()){
        cout << "Неверный ввод утилизированности товара\n";
        return 0;
    }

    /* exec sql begin declare section */
       
        
       
    
#line 258 "appdb.pgc"
 int id = i ;
 
#line 259 "appdb.pgc"
 const char * place = p . c_str () ;
 
#line 260 "appdb.pgc"
 bool rem = r ;
/* exec sql end declare section */
#line 261 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call update_row ( $1  , $2  , $3  )", 
	ECPGt_int,&(id),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(place),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_bool,&(rem),(long)1,(long)1,sizeof(bool), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 262 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 263 "appdb.pgc"

    cout << "Строка изменена\n";
    return 1;
}

int delete_rows(){
    cout << "Введите местоположение, по которому будет производиться удаление:\n";
    string p;
    cin >> p;
    if (cin.fail()){
        cout << "Неверный ввод местоположения\n";
        return 0;
    }

    /* exec sql begin declare section */
        
    
#line 278 "appdb.pgc"
 const char * place = p . c_str () ;
/* exec sql end declare section */
#line 279 "appdb.pgc"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "call delete_rows ( $1  )", 
	ECPGt_char,&(place),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 280 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 281 "appdb.pgc"

    cout << "Соответствующие строки удалены\n";
    return 1;
}

void show_table(){
    /* exec sql begin declare section */
     
     
     
    
#line 288 "appdb.pgc"
 int id ;
 
#line 289 "appdb.pgc"
  struct varchar_3  { int len; char arr[ 32 ]; }  pp ;
 
#line 290 "appdb.pgc"
  struct varchar_4  { int len; char arr[ 32 ]; }  rem ;
/* exec sql end declare section */
#line 291 "appdb.pgc"


    /* declare full_t cursor for select * from grocery */
#line 293 "appdb.pgc"


    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare full_t cursor for select * from grocery", ECPGt_EOIT, ECPGt_EORT);}
#line 295 "appdb.pgc"

    while(1){
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch full_t", ECPGt_EOIT, 
	ECPGt_int,&(id),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_varchar,&(pp),(long)32,(long)1,sizeof(struct varchar_3), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_varchar,&(rem),(long)32,(long)1,sizeof(struct varchar_4), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 297 "appdb.pgc"

        if(sqlca.sqlcode!=0){
            break;
        }
        printf("id=%d, productplace=%s, isremoved=%s\n", id, pp.arr, rem.arr);
    }
    
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close full_t", ECPGt_EOIT, ECPGt_EORT);}
#line 304 "appdb.pgc"

    { ECPGtrans(__LINE__, NULL, "commit");}
#line 305 "appdb.pgc"

}

int main() {
    cout << "Выберите польователя:\n";
    cout << "1 - Администратор\n";
    cout << "2 - Гость\n";

    int role;
    cin >> role;
    while (role != 1 && role != 2){
        cout << "Неправильное значение, попробуйте снова\n";
        cin >> role;
    }

    string password;
    string right_p = "8850";

    switch (role) {
        case 1: 
            cout << "Введите пароль:\n";
            cin >> password;
            if (password != "8850"){
                cout << "Неверный пароль!\n";
                return 1;
            }

            if (create_conn("grocery_shop_cpp@localhost:5432", "postgres", "8850") == 0){
                cout << "Соответствующей базы не существует, сейчас она будет создана!\n";
                create_conn("TASK_HSE@localhost:5432", "postgres", "8850");
                create_db();
                { ECPGdisconnect(__LINE__, "ALL");}
#line 336 "appdb.pgc"

                create_conn("grocery_shop_cpp@localhost:5432", "postgres", "8850");
                cr_table();
                fill_table();
                roles_proc();
                create_db_role(true);

                { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create procedure clear_table ( ) as $$\
                    delete from grocery;\
                $$ language sql", ECPGt_EOIT, ECPGt_EORT);}
#line 345 "appdb.pgc"

                { ECPGtrans(__LINE__, NULL, "commit");}
#line 346 "appdb.pgc"

                cout << "Процедура для очистки таблицы создана\n";

                { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create procedure insert_row ( id int , place varchar , isrem bool ) as $$\
                    begin\
                        insert into grocery values (id, place, isrem);\
                    end;\
                $$ language plpgsql", ECPGt_EOIT, ECPGt_EORT);}
#line 353 "appdb.pgc"

                { ECPGtrans(__LINE__, NULL, "commit");}
#line 354 "appdb.pgc"

                cout << "Процедура для добавления новой строки создана\n";

                { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create or replace function find_rows ( v varchar ) returns setof grocery as $$\
                    select productid, productplace, isremoved from grocery where productplace = v;\
                $$ language sql", ECPGt_EOIT, ECPGt_EORT);}
#line 359 "appdb.pgc"

                { ECPGtrans(__LINE__, NULL, "commit");}
#line 360 "appdb.pgc"

                cout << "Функция для поиска строк создана\n";

                { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create or replace procedure update_row ( i int , v varchar , b bool ) as $$\
                    begin\
                        update grocery set productplace=v, isremoved=b where productid=i;\
                    end;\
                $$ language plpgsql", ECPGt_EOIT, ECPGt_EORT);}
#line 367 "appdb.pgc"

                { ECPGtrans(__LINE__, NULL, "commit");}
#line 368 "appdb.pgc"

                cout << "Процедура для изменения строк создана\n";

                { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "create or replace procedure delete_rows ( v varchar ) as $$\
                    begin\
                        delete from grocery where productplace=v;\
                    end;\
                $$ language plpgsql", ECPGt_EOIT, ECPGt_EORT);}
#line 375 "appdb.pgc"

                { ECPGtrans(__LINE__, NULL, "commit");}
#line 376 "appdb.pgc"

                cout << "Процедура для удаления строк создана\n";

                
            };

            while (1) {
                cout << "\nВыберите опцию:\n";
                cout << "1 - удаление базы данных\n";
                cout << "2 - очистка таблицы\n";
                cout << "3 - добавление новых данных\n";
                cout << "4 - поиск по полю 'productplace'\n";
                cout << "5 - обновление картежа\n";
                cout << "6 - удаление по полю 'productplace'\n";
                cout << "7 - создать нового пользователя\n";
                cout << "8 - показать всю таблицу\n";
                cout << "9 - завершить работу\n";

                int option;
                cin >> option;
                while (option != 1 && option != 2 && option != 3 && option != 4 && option != 5 && option != 6 && option != 7 && option != 8 && option != 9){
                    cout << "Неправильное значение, попробуйте снова\n";
                    cin >> option;
                }

                switch (option) {
                    case 1:
                        drop_db();
                        return 0;
                    case 2:
                        clear_table();
                        break;
                    case 3:
                        if (ins_row() == 0){
                            return 0;
                        };
                        break;
                    case 4:
                    if (find_rows() == 0){
                            return 0;
                        };
                        break;
                    case 5:
                        if (update_row() == 0){
                            return 0;
                        };
                        break;
                    case 6:
                        if (delete_rows() == 0){
                            return 0;
                        };
                        break;
                    case 7:
                        if (create_db_role(false) == 0){
                            return 0;
                        }
                        break;
                    case 8:
                        show_table();
                        break;
                    case 9:
                        { ECPGdisconnect(__LINE__, "ALL");}
#line 437 "appdb.pgc"

                        return 0;
                }  
            }

        case 2: 
            if (create_conn("grocery_shop_cpp@localhost:5432", "guest_", "1") == 0){
                cout << "Базы не существует, зайдите на администратора, чтобы создать ее\n";
                return 0;
            };

            while (1) {
                cout << "\nВыберите опцию:\n";
                cout << "1 - поиск по полю 'productplace'\n";
                cout << "2 - показать всю таблицу\n";
                cout << "3 - завершить работу\n";

                int option;
                cin >> option;
                while (option != 1 && option != 2 && option != 3){
                    cout << "Неправильное значение, попробуйте снова\n";
                    cin >> option;
                }

                switch (option) {
                    case 1:
                        if (find_rows() == 0){
                                return 0;
                        };
                        break;
                    case 2:
                        show_table();
                        break;
                    case 3:
                        { ECPGdisconnect(__LINE__, "ALL");}
#line 471 "appdb.pgc"

                        return 0;
                }
            }
    }
}

/*ecpg -o appdb.cpp appdb.pgc && g++ -c appdb.cpp -o appdb.o && g++ appdb.o -lecpg -o*/