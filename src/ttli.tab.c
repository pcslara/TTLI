/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "ttli.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

char true_val  = '1';
char false_val = '0';


extern FILE * yyin;
int curr_line = 1;
int errors = 0;
int flerror = 1;

int curr_val[26];
int alpha[26];
int eval_reg[26];
int eval_idx = 0;

struct tree {

    struct tree * l;
    struct tree * r;    
    char id;             // Se for folha eh uma letra se nao é uma operacao '*' ou '+'    
    unsigned char value; // Soh usado quando for folha /** 0 if negate, 1 otherwise **/ 
    char * vars;
    
};

struct __sym {
    struct tree * exp;
    char id[128];
    struct __sym * next;
};
int getkey(void);

typedef struct __sym sym;
sym * symbols = NULL;

void print_table( struct tree * t );


void printhelp( int func_id ) {
    
    switch( func_id ) {
        case 0:
            printf("Avaliable operations:\n\n");
            printf("+       Logic OR\n");    
            printf("        Logic AND\n");    
            printf("<->     If, only if\n");    
            printf("->      Implication\n");    
            printf("^       Exclusive OR XOR\n\n");    
            
                                    
            printf("Avaliable functions:\n\n");
            printf("    table\n");
            printf("    setlogic\n");
            printf("    vars\n");
               
            printf("Use help <function name>\n");
            break;       
    }

}
int getkey(void) {
      int c=0;

      struct termios org_opts, new_opts;
      int res=0;
          //-----  store old settings -----------
      res=tcgetattr(STDIN_FILENO, &org_opts);
      assert(res==0);
          //---- set new terminal parms --------
      memcpy(&new_opts, &org_opts, sizeof(new_opts));
      new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
      tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
      c=getchar();
          //------  restore old settings ---------
      res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
      assert(res==0);
      return(c);
}

void * HistoryMonitor( void * arg ) {
    while( 1 ) {
        char c = getkey();
        
        fprintf( stdout, "%c", c );
        fprintf( stdin, "%c", c );
        fflush( stdin );
        if( c == 10 ) {
                fflush( stdin );
         }
    }
}

void init_eval( unsigned char b ) {
    int i = 0;
    eval_reg[0] = b;
    for( i = 1; i < 26; i++ )
        eval_reg[i] = -1;
    eval_idx = 1;
}


void print_exp( struct tree * t ) {
    if( t->l == NULL ) {
        printf("%c", t->id );
        return;
    } else if( t->r == NULL ) {
        printf("%c", t->id );
        return;
    } 
    print_exp( t->l );
    print_exp( t );
    print_exp( t->r );
    
}
void put_alpha( struct tree * t ) {
    if( t == NULL )
        return;
        
    if( t->id != '*' && t->id != '+' )
        alpha[t->id - 'a'] = 1;
    
    put_alpha( t->l );
    put_alpha( t->r );    
}

void print_vars( struct tree * t ) {
    int i = 0;    
    if( t != NULL ) {
        for( i = 0; i < 26; i++ )
            alpha[i] = 0;
        put_alpha( t );
        printf("[");
        for( i = 0; i < 26; i++ ) {
            if( alpha[i] != 0 )
                printf(" %c ", (char)i + 'a' );
        }
        printf("]\n");
    }
        
}

void get_vars_str( char * out, struct tree * t ) {
    
    int i = 0;    
    char str[10];
    strcpy( out, "" );

    if( t != NULL ) {
        for( i = 0; i < 26; i++ )
            alpha[i] = 0;
        put_alpha( t );
        for( i = 0; i < 26; i++ ) {
            if( alpha[i] != 0 ) {
                
                sprintf(str, " %c ", (char)i + 'a' );
                strcat( out, str );
            }
        }

    }
}

/**
 * 1 if has same vars
 * 0 otherwise
 */
 
int cmp_vars( struct tree * t1 , struct tree * t2 ) {
    
    char t1vars[64], t2vars[64];
    get_vars_str( t1vars, t1 );
    get_vars_str( t2vars, t2 );    
    
    if( !strcmp( t1vars, t2vars ) )
        return 1;
    else
        return 0; 

}


void new_id( char id[128], struct tree * t ) {
    if( t != NULL ) {
        sym * tmp = (sym *) malloc( sizeof( sym ) );
        tmp->exp = t;
        strncpy( tmp->id, id, 128 );
        tmp->next = symbols;
        symbols = tmp;   
    } 
}

struct tree * has_id( char id[128] ) {
    sym * tmp = symbols;
    while( tmp != NULL ) {
        if( strncmp( tmp->id, id, 128 ) == 0 )
            return tmp->exp; 
        tmp = tmp->next;
    }
    return NULL;
}

void show( char id[128] ) {
    struct tree * t = has_id( id );
    if(  t == NULL ) {
        char str[1024];
        sprintf( str, "expression '$%s' not found", id );
        yyerror( str );
    } else        
        print_table( t );    
    
 
}


int count_vars( struct tree * t ) {
    int count = 0;
    int i;
    for( i = 0; i < 26; i++ )
        alpha[i] = 0;
    put_alpha( t );
    
    for( i = 0; i < 26; i++ )
        if( alpha[i] == 1 )
            count++;
    
    return count;   
}

unsigned char eval( struct tree * t ) {
    unsigned char ret;
    
    if( t->id == '*' )
        ret = eval( t->l ) & eval( t->r );
    else if( t->id == '+' )
        ret = eval( t->l ) | eval( t->r );
    else if( t->id == '>' )    // Implica
        ret = !eval( t->l ) | eval( t->r );
    else if( t->id == '-' ) { // Se e somente se
        unsigned char A =   eval( t->l );
        unsigned char B =   eval( t->r );        
        ret = A*B + (!A) * (!B);
    } else if( t->id == '^' )    // Xored
        ret = ( eval( t->l ) ^ eval( t->r ) ) & 1;
    else if( t->id == '!' )
        return !eval( t->l );
    else
        ret = t->value ? curr_val[t->id - 'a'] : !curr_val[t->id - 'a'];
    
    return ret;   
}

void evalb( struct tree * t ) {
    int i = 0;
    if( t != NULL ) {
        int c = count_vars( t );
        if( c != eval_idx ) {
            yyerror("invalid number of arguments");
            return;
        }
        for( i = 0; i < 26; i++ )
            alpha[i] = 0;
        
        put_alpha( t );
        int k = 0;
        for( i = 0; i < 26; i++ ) {
            if( alpha[i] != 0 ) {
                curr_val[i] = eval_reg[k];
                k++;
            }   
        }
        printf("%d\n", eval( t ) );
                      
    }
}

void make_table( unsigned char ** val, int * n_var, struct tree * t ) {
    int i, j, k = 0, count = count_vars( t );
    *val = (unsigned char *) malloc( (1 << count )*(count + 1) * sizeof( unsigned char ) );
    *n_var = count;
    val[0][2] = 1;
   
    for( i = 0; i < 26; i++ )
        curr_val[i] = -1;
       
    for( i = 0; i < (1 << count); i++ ) {
        /** Current Vals **/
        for( j = 0; j < count; j++ ) {
            val[0][ i * (count + 1) + ((count - 1) - j) ] = ( i >> j ) & 1;
        }
                
        k = 0;
        /** Set curr_val array **/
        for( j = 0; j < 26; j++ ) {
            if( alpha[j] != 0 ) {
                curr_val[j] = val[0][i * (count + 1) + k];
                k++;
            }
        }
        val[0][i*(count + 1) + count] = eval( t );        
    }       
}

void print_table( struct tree * t ) {
    int count, i, j;
    unsigned char * tab;
    if( t != NULL ) {
        make_table( &tab, &count, t );
        for( j = 0; j < count + 1; j++ ) { 
            if( j == 0 )
                printf("+-----");
            else if( j == count -1 )
                printf("-----+"); 
            else if( j == count )
                printf("-----+");
            else
                printf("-----");
        }
        printf("\n|");
        int f = 0;
        for( j = 0; j < 26; j++ ) {
            if( curr_val[j] != -1 ) {
                if( f == 0 ) {
                    printf("  %c  ", (char)j + 'a');
                } else if( f == count - 1 ) {
                    printf("  %c  |", (char)j + 'a');
                } else {
                    printf("  %c  ", (char)j + 'a');
                }
                f++;
            }
        }
        printf("  R  |\n");
        for( j = 0; j < count + 1; j++ ) { 
            if( j == 0 )
                printf("+-----");
            else if( j == count -1 )
                printf("-----+"); 
            else if( j == count )
                printf("-----+");
            else
                printf("-----");
        }
        printf("\n");
        for( i = 0; i < (1 << count)*(count + 1); i++ ) {
            
            if( i % (count + 1) == 0 )
                printf("|  %c  ", tab[i] ? true_val : false_val );
            else if( ((i + 1) % (count + 1)) == 0 )
                printf("  %c  |\n", tab[i] ? true_val : false_val );
            else if( ((i + 1) % (count + 1)) == count)
                printf("  %c  |", tab[i] ? true_val : false_val );            
            else 
                printf("  %c  ", tab[i] ? true_val : false_val );
                
        }    
        for( j = 0; j < count + 1; j++ ) { 
            if( j == 0 )
                printf("+-----");
            else if( j == count -1 )
                printf("-----+"); 
            else if( j == count )
                printf("-----+\n");
            else
                printf("-----");
        }
    }
}


void tree_print( struct tree * t ) {
    
    if( t == NULL ) {
        printf("NULL\n");
        return;
    }
    
    printf("[%c %d]\n", t->id, t->value );       
    
    tree_print( t->l );
    tree_print( t->r );

}

void tree_free( struct tree * t ) {
           
}

struct tree * tree_leaf( char id ) {
    struct tree * ret;
    unsigned char value = 0;
    
    //printf("id = %c\n", id );    
    
    ret = (struct tree *) malloc( sizeof( struct tree ) );

    if( islower( id ) )
        value = 1;
    
    ret->value = value;
    ret->id = tolower( id );
    
    ret->l = NULL;
    ret->r = NULL;    
    
    return ret;    
}

struct tree * tree_branch( char op, struct tree * l, struct tree * r ) {
    struct tree * ret;
    unsigned char value = 0;
    
    ret = (struct tree *) malloc( sizeof( struct tree ) );
    //printf("*\n");
    ret->l = l;
    ret->r = r;
    ret->id = op;
    ret->value = -1;
    
    return ret;
}

struct tree * tree_branchr( char op, struct tree * l, char id ) {

    struct tree * ret;
    struct tree * r;
    unsigned char value = 0;
    
    ret = (struct tree *) malloc( sizeof( struct tree ) );
    r = tree_leaf( id );
    
    ret->l = l;
    ret->r = r;
    ret->id = op;
    ret->value = -1;
    
    return ret;
}

int is_equal( struct tree * e1, struct tree * e2 ) {
    int count1, count2, i, j;
    unsigned char * tab1, * tab2;
    if( e1 != NULL && e2 != NULL ) {
        if( !cmp_vars( e1, e2 ) )
            return 0;
        make_table( &tab1, &count1, e1 );
        make_table( &tab2, &count2, e2 );
        int step = count1 + 1;
         
         for( i = count1; i < (1 << count1)*(count1 + 1); i += step ) {
                if ( tab1[i] !=  tab2[i]) 
                    return 0;
                
         }     
     } else {
        return -1;
     }
     return 1;    
}

void print_is_equal( struct tree * t1, struct tree * t2 ) {
    if( t1 != NULL && t2 != NULL )
        printf("%c\n", is_equal( t1, t2 ) == 1 ? true_val : false_val );
}

void print_bool_var( int v  ) {
    if( v != -1 )
        printf("%c\n", v == 1 ? true_val : false_val );
}
char print_expr( struct tree * t ) { 
//    printf("(");
    if( t->id == '*' ) {
        printf("%c%c",  print_expr( t->l ) , print_expr( t->r ) );      
        
    } else if( t->id == '+' ) {
        char l = print_expr( t->l );
        char r = print_expr( t->r );                
        if( l != '\0' && r != '\0' ) {
            printf("%c+%c",l, r);      
        } else {
            printf("%c+%c", l, r);      
        }

        
    } else if( t->id >= 'a' && t->id <= 'z' ) {
        return t->id;
    }
    return '\0';
}



/* Line 268 of yacc.c  */
#line 572 "ttli.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     VAR = 258,
     logic_letter = 259,
     NVAR = 260,
     EOL = 261,
     CMPEQ = 262,
     SHOW = 263,
     VARS = 264,
     TABLE = 265,
     SETLOGIC = 266,
     HELP = 267,
     EXPR = 268,
     IF = 269,
     ENDIF = 270,
     PRINT = 271,
     ID = 272,
     STRING = 273,
     BOOL = 274
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union 
/* Line 293 of yacc.c  */
#line 501 "ttli.y"
tt_types
{

/* Line 293 of yacc.c  */
#line 501 "ttli.y"
 
    char var;
    char nvar;
    char  id[256];
    char str[1024];
    struct tree * expr;
    unsigned char bvar;



/* Line 293 of yacc.c  */
#line 641 "ttli.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 653 "ttli.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   143

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  12
/* YYNRULES -- Number of rules.  */
#define YYNRULES  40
/* YYNRULES -- Number of states.  */
#define YYNSTATES  97

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   274

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,     2,     2,     2,     2,     2,
      32,    33,    22,    20,    26,    21,     2,    23,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      29,    27,    28,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    24,     2,    25,    30,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      20,    23,    29,    35,    41,    47,    48,    55,    63,    71,
      74,    83,    86,    90,    93,    94,    97,   101,   103,   107,
     109,   113,   118,   124,   128,   131,   133,   136,   138,   142,
     144
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      35,     0,    -1,    -1,    35,    37,    -1,    20,    -1,    21,
      -1,    22,    -1,    23,    -1,     6,    -1,    40,     6,    -1,
      42,     6,    -1,    10,    24,    42,    25,     6,    -1,     9,
      24,    42,    25,     6,    -1,    17,    24,    41,    25,     6,
      -1,    13,    24,    42,    25,     6,    -1,    -1,    11,    24,
      36,    26,    36,    25,    -1,    11,    24,     3,    26,     3,
      25,     6,    -1,    11,    24,    19,    26,    19,    25,     6,
      -1,    45,     6,    -1,    14,    24,    45,    25,     6,    38,
      15,     6,    -1,    12,     6,    -1,    16,    18,     6,    -1,
       1,     6,    -1,    -1,    39,    37,    -1,    17,    27,    42,
      -1,    19,    -1,    41,    26,    19,    -1,    43,    -1,    42,
      20,    43,    -1,    42,    21,    28,    43,    -1,    42,    29,
      21,    28,    43,    -1,    42,    30,    43,    -1,    31,    42,
      -1,    44,    -1,    43,    44,    -1,     3,    -1,    32,    42,
      33,    -1,    17,    -1,    42,     7,    42,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   526,   526,   527,   530,   530,   530,   530,   532,   533,
     534,   535,   536,   537,   546,   547,   548,   549,   550,   551,
     552,   554,   555,   556,   559,   559,   565,   569,   570,   572,
     573,   574,   575,   576,   577,   581,   582,   586,   587,   588,
     598
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "VAR", "logic_letter", "NVAR", "EOL",
  "CMPEQ", "SHOW", "VARS", "TABLE", "SETLOGIC", "HELP", "EXPR", "IF",
  "ENDIF", "PRINT", "ID", "STRING", "BOOL", "'+'", "'-'", "'*'", "'/'",
  "'['", "']'", "','", "'='", "'>'", "'<'", "'^'", "'!'", "'('", "')'",
  "$accept", "program", "signals", "stm", "if_stm", "$@1", "declare",
  "var_list", "exp", "factor", "term", "bool_expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      43,    45,    42,    47,    91,    93,    44,    61,    62,    60,
      94,    33,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    34,    35,    35,    36,    36,    36,    36,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    39,    38,    40,    41,    41,    42,
      42,    42,    42,    42,    42,    43,    43,    44,    44,    44,
      45
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     2,
       2,     5,     5,     5,     5,     0,     6,     7,     7,     2,
       8,     2,     3,     2,     0,     2,     3,     1,     3,     1,
       3,     4,     5,     3,     2,     1,     2,     1,     3,     1,
       3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,    37,     8,     0,     0,     0,     0,
       0,     0,     0,    39,     0,     0,     3,     0,     0,    29,
      35,     0,    23,     0,     0,     0,    21,     0,     0,     0,
       0,     0,    39,    34,     0,     9,    10,     0,     0,     0,
       0,     0,    36,    19,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,     0,    22,    27,     0,    26,
      38,    40,    30,     0,     0,    33,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,     0,    12,    11,     0,
       0,     0,    14,    24,    13,    28,    32,     0,     0,    16,
       0,     0,    17,    18,     0,    25,    20
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    52,    16,    90,    91,    17,    58,    18,    19,
      20,    21
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -38
static const yytype_int8 yypact[] =
{
     -38,     2,   -38,     0,   -38,   -38,   -14,    -2,    13,    36,
      21,    23,    30,    -7,     4,     4,   -38,    44,    62,     6,
     -38,    46,   -38,     4,     4,    96,   -38,     4,     4,    47,
      51,     4,   -38,   113,    11,   -38,   -38,     4,     6,    27,
      45,     6,   -38,   -38,    83,   100,    49,    50,   -38,   -38,
     -38,   -38,    52,   102,    66,    56,   -38,   -38,    -1,   113,
     -38,   113,     6,     6,    43,     6,    68,    71,    69,    81,
       7,    82,    84,    99,    87,     6,     6,   -38,   -38,    64,
      77,    85,   -38,   -38,   -38,   -38,     6,   103,   105,   -38,
     109,    48,   -38,   -38,   108,   -38,   -38
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -38,   -38,    58,    35,   -38,   -38,   -38,   -38,    70,   -37,
     -19,   107
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -16
static const yytype_int8 yytable[] =
{
      42,    62,     2,     3,    65,     4,    22,     4,     5,     4,
      23,     6,     7,     8,     9,    10,    11,    30,    12,    13,
      31,    32,    24,    32,    73,    74,    75,    48,    49,    50,
      51,    38,    39,    14,    15,    14,    15,    25,    15,    86,
      40,    41,    26,    42,    60,    27,    42,    28,    29,     3,
      35,     4,    43,    56,     5,    63,    42,     6,     7,     8,
       9,    10,    11,   -15,    12,    13,    64,    42,    36,    37,
      57,    76,    79,    37,    77,    68,    69,    78,    70,    14,
      15,    72,    38,    39,    33,    34,    38,    39,    82,    87,
      83,    40,    41,    44,    45,    40,    41,    53,    54,    46,
      80,    59,    88,    38,    39,    84,    85,    61,    66,    92,
      89,    93,    40,    41,    96,    47,    48,    49,    50,    51,
      38,    39,    38,    39,    94,    67,    95,    71,    81,    40,
      41,    40,    41,    38,    39,    55,     0,     0,     0,     0,
       0,     0,    40,    41
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-38))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      19,    38,     0,     1,    41,     3,     6,     3,     6,     3,
      24,     9,    10,    11,    12,    13,    14,    24,    16,    17,
      27,    17,    24,    17,    25,    26,    63,    20,    21,    22,
      23,    20,    21,    31,    32,    31,    32,    24,    32,    76,
      29,    30,     6,    62,    33,    24,    65,    24,    18,     1,
       6,     3,     6,     6,     6,    28,    75,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    21,    86,     6,     7,
      19,    28,     3,     7,     6,    26,    26,     6,    26,    31,
      32,    25,    20,    21,    14,    15,    20,    21,     6,    25,
       6,    29,    30,    23,    24,    29,    30,    27,    28,     3,
      19,    31,    25,    20,    21,     6,    19,    37,    25,     6,
      25,     6,    29,    30,     6,    19,    20,    21,    22,    23,
      20,    21,    20,    21,    15,    25,    91,    25,    70,    29,
      30,    29,    30,    20,    21,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    35,     0,     1,     3,     6,     9,    10,    11,    12,
      13,    14,    16,    17,    31,    32,    37,    40,    42,    43,
      44,    45,     6,    24,    24,    24,     6,    24,    24,    18,
      24,    27,    17,    42,    42,     6,     6,     7,    20,    21,
      29,    30,    44,     6,    42,    42,     3,    19,    20,    21,
      22,    23,    36,    42,    42,    45,     6,    19,    41,    42,
      33,    42,    43,    28,    21,    43,    25,    25,    26,    26,
      26,    25,    25,    25,    26,    43,    28,     6,     6,     3,
      19,    36,     6,     6,     6,    19,    43,    25,    25,    25,
      38,    39,     6,     6,    15,    37,     6
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:

/* Line 1806 of yacc.c  */
#line 527 "ttli.y"
    { printf(">> "); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 530 "ttli.y"
    {}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 532 "ttli.y"
    {  }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 533 "ttli.y"
    { }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 534 "ttli.y"
    { print_table( (yyvsp[(1) - (2)].expr) ); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 535 "ttli.y"
    { print_table( (yyvsp[(3) - (5)].expr) );}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 536 "ttli.y"
    { print_vars( (yyvsp[(3) - (5)].expr) );}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 537 "ttli.y"
    {  
                                  struct tree * t = has_id( (yyvsp[(1) - (5)].id) );
                                  if( t == NULL ) {
                                      char str[1024]; sprintf( str, "expression '$%s' not found", (yyvsp[(1) - (5)].id) );
                                      yyerror( str );                                     
                                  } else 
                                      evalb( t );
                                 }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 546 "ttli.y"
    { print_expr( (yyvsp[(3) - (5)].expr) ); printf("\n"); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 548 "ttli.y"
    {}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 549 "ttli.y"
    { true_val = (yyvsp[(3) - (7)].var); false_val = (yyvsp[(5) - (7)].var); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 550 "ttli.y"
    { true_val = (yyvsp[(3) - (7)].bvar) ? '1' : '0'; false_val = (yyvsp[(5) - (7)].bvar) ? '1' : '0'; }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 551 "ttli.y"
    { print_bool_var( (yyvsp[(1) - (2)].var) ); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 552 "ttli.y"
    { printf("IF FOUND\n"); if( (yyvsp[(3) - (8)].var) ) printf("true\n"); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 554 "ttli.y"
    { printhelp( 0 ); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 555 "ttli.y"
    { printf("%s\n", (yyvsp[(2) - (3)].str) ); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 556 "ttli.y"
    { }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 559 "ttli.y"
    { printf("\t"); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 559 "ttli.y"
    { printf("   "); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 565 "ttli.y"
    { new_id( (yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].expr) );  }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 569 "ttli.y"
    {  init_eval( (yyvsp[(1) - (1)].bvar) );  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 570 "ttli.y"
    {  eval_reg[eval_idx++] = (int) (yyvsp[(3) - (3)].bvar);     }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 572 "ttli.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 573 "ttli.y"
    { (yyval.expr) = tree_branch( '+', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr) ); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 574 "ttli.y"
    { (yyval.expr) = tree_branch( '>', (yyvsp[(1) - (4)].expr), (yyvsp[(4) - (4)].expr) ); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 575 "ttli.y"
    { (yyval.expr) = tree_branch( '-', (yyvsp[(1) - (5)].expr), (yyvsp[(5) - (5)].expr) ); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 576 "ttli.y"
    { (yyval.expr) = tree_branch( '^', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr) ); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 577 "ttli.y"
    { (yyval.expr) = tree_branch( '!', (yyvsp[(2) - (2)].expr), NULL ); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 582 "ttli.y"
    { (yyval.expr) = tree_branch( '*', (yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].expr) ); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 586 "ttli.y"
    { (yyval.expr) = tree_leaf( (yyvsp[(1) - (1)].var) ); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 587 "ttli.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 588 "ttli.y"
    { struct tree * t = has_id( (yyvsp[(1) - (1)].id) );
                              if( t == NULL ) {
                                  char str[1024]; sprintf( str, "expression '$%s' not found", (yyvsp[(1) - (1)].id) );
                                  yyerror( str );
                                  (yyval.expr) = NULL;                                     
                              } else { 
                                (yyval.expr) = t;
                              }
                          }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 598 "ttli.y"
    {  (yyval.var) = is_equal( (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr) ); }
    break;



/* Line 1806 of yacc.c  */
#line 2204 "ttli.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 602 "ttli.y"

yyerror( char * str ) {
    errors++;
    printf("Error: %s\n", str );
}

void printapp() {
    printf("Truth Table Language TTL Interpreter 1.0 Copyright (C) 2010, ISTCC-P\n");
}



int main(int argc, char **argv) {

    
    int i = 0;
    char output[256] = { 0 };
    char input[256]  = { 0 };
    int has_input = 0;
    int has_output = 0;
    
    int rc;
   
    
    if( argc == 1 )
        yyin = stdin;
    
    for( i = 1; i < argc; i++ ) {
            if( strcmp( argv[i], "-h" ) == 0 ) {
                printapp();     
            } else if( strcmp( argv[i], "-o" ) == 0 ) {
                if( i + 1 != argc && has_output == 0) {
                    strncpy( output, argv[i + 1], 256 );    
                    i++;
                    has_output = 1;
                } else {
                    fprintf( stderr, "Error: specify a output file\n");
                    return 3;
                }
            } else {
                    if( has_input == 0 ) {
                        strncpy( input, argv[i], 256 );    
                        has_input = 1;   
                    }
            }
    }
    printapp();
    if( yyin != stdin ) 
        yyin = fopen( input, "r");
    if( yyin != 0 ) {
         printf(">> ");
         while( 1 ) {
            
            yyparse();
            
         }
    } else {
        fprintf( stderr, "Error: input file not found\n");
        return 1;
    }
}
    

