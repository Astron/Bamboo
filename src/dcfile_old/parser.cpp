/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         DCSTYPE
/* Substitute the variable and function names.  */
#define yyparse         dcparse
#define yylex           dclex
#define yyerror         dcerror
#define yydebug         dcdebug
#define yynerrs         dcnerrs

#define yylval          dclval
#define yychar          dcchar

/* Copy the first part of user declarations.  */
#line 4 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:339  */

	#include "lex.h"
	#include "parse.h"

	#include <cstdint>  // for fixed-width integers
	#include <cmath>    // std::isinf
	#include <stack>    // std::stack
	#include <fstream>  // std::ifstream
	#include <sstream>  // std::stringstream

	#include "../dcfile/token.h"
	#include "../dcfile/parser-defs.h"
	#include "../module/Module.h"
	#include "../module/Type.h"
	#include "../module/NumericRange.h"
	#include "../module/Numeric.h"
	#include "../module/Array.h"
	#include "../module/Struct.h"
	#include "../module/Class.h"
	#include "../module/Field.h"
	#include "../module/Method.h"
	#include "../module/Parameter.h"
	#include "../module/MolecularField.h"
	#include "../bits/buffers.h"

	//#define yylex dclex
	//#define yyparse dcparse
	//#define yyerror dcerror
	//#define yywarning dcwarning
	//#define yychar dcyychar
	//#define yydebug dcyydebug
	//#define yynerrs dcyynerrs

	// Because our token type contains objects of type string, which
	// require correct copy construction (and not simply memcpying), we
	// cannot use bison's built-in autTo-stack-grow feature.  As an easy
	// solution, we ensure here that we have enough yacc stack to start
	// with, and that it doesn't ever try to grow.
	#define YYINITDEPTH 1000
	#define YYMAXDEPTH 1000

	using namespace std;
	using namespace bamboo;
	namespace bamboo   // open namespace bamboo
	{


	// Parser output
	static Module* parsed_module = nullptr;
	static Value parsed_value;

	// Parser state
	static Class* current_class = nullptr;
	static Struct* current_struct = nullptr;

	// Stack of distributed types for parsing values
	struct TypeAndDepth
	{
		int depth;
		const Type* type;
		TypeAndDepth(const Type* t, int d) : depth(d), type(t) {}
	};
	static stack<TypeAndDepth> type_stack;
	static int current_depth;

	// These two types are really common types the parser doesn't need to make new
	//     duplicates of every time a string or blob is used.
	static Array* default_string = nullptr;
	static Array* default_blob = nullptr;

	/* Helper functions */
	static bool check_depth();
	static void depth_error(string what);
	static void depth_error(int depth, string what);
	static Value number_value(const Type *type, double &number);
	static Value number_value(const Type *type, int64_t &number);
	static Value number_value(const Type *type, uint64_t &number);

////////////////////////////////////////////////////////////////////
// Defining the interface to the parser.
////////////////////////////////////////////////////////////////////

	static void dcparser_init_file(istream& in, const string& filename, Module& module)
	{
		dclexer_init(in, filename, START_DC_FILE);
		parsed_module = &module;
	}

	static void dcparser_init_value(istream& in, const string& source, const Type* type)
	{
		current_depth = 0;
		type_stack.push(TypeAndDepth(type, 0));
		dclexer_init(in, source, START_DC_VALUE);
	}

	static void dcparser_cleanup()
	{
		current_depth = 0;
		type_stack = stack<TypeAndDepth>();
		parsed_module = nullptr;
		parsed_value = Value();
	}


	} // close namespace bamboo

#line 182 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser-defs.h".  */
#ifndef YY_DC_MEDIA_STORAGE_DEVEL_ASTRON_BAMBOO_SRC_DCFILE_PARSER_DEFS_H_INCLUDED
# define YY_DC_MEDIA_STORAGE_DEVEL_ASTRON_BAMBOO_SRC_DCFILE_PARSER_DEFS_H_INCLUDED
/* Debug traces.  */
#ifndef DCDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define DCDEBUG 1
#  else
#   define DCDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define DCDEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined DCDEBUG */
#if DCDEBUG
extern int dcdebug;
#endif

/* Token type.  */
#ifndef DCTOKENTYPE
# define DCTOKENTYPE
  enum dctokentype
  {
    UNSIGNED_INTEGER = 258,
    REAL = 259,
    STRING = 260,
    HEX_STRING = 261,
    IDENTIFIER = 262,
    CHAR = 263,
    START_DC_FILE = 264,
    START_DC_VALUE = 265,
    KW_DCLASS = 266,
    KW_STRUCT = 267,
    KW_FROM = 268,
    KW_IMPORT = 269,
    KW_TYPEDEF = 270,
    KW_KEYWORD = 271,
    KW_INT8 = 272,
    KW_INT16 = 273,
    KW_INT32 = 274,
    KW_INT64 = 275,
    KW_UINT8 = 276,
    KW_UINT16 = 277,
    KW_UINT32 = 278,
    KW_UINT64 = 279,
    KW_FLOAT32 = 280,
    KW_FLOAT64 = 281,
    KW_STRING = 282,
    KW_BLOB = 283,
    KW_CHAR = 284
  };
#endif

/* Value type.  */


extern DCSTYPE dclval;

int dcparse (void);

#endif /* !YY_DC_MEDIA_STORAGE_DEVEL_ASTRON_BAMBOO_SRC_DCFILE_PARSER_DEFS_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 266 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined DCSTYPE_IS_TRIVIAL && DCSTYPE_IS_TRIVIAL)))

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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  23
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   289

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  79
/* YYNRULES -- Number of rules.  */
#define YYNRULES  171
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  266

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   284

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    43,     2,     2,
      41,    42,    32,    45,    33,    44,    31,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,    30,
       2,    40,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    35,     2,    36,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29
};

#if DCDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   215,   215,   216,   221,   222,   223,   224,   225,   226,
     227,   231,   235,   240,   248,   253,   262,   263,   270,   271,
     278,   282,   290,   291,   298,   343,   347,   355,   365,   380,
     384,   385,   393,   392,   432,   433,   437,   444,   454,   485,
     486,   487,   525,   549,   557,   556,   596,   597,   598,   632,
     633,   637,   638,   639,   640,   644,   649,   648,   663,   670,
     675,   684,   683,   695,   694,   706,   705,   719,   726,   727,
     731,   749,   753,   757,   761,   765,   769,   776,   802,   834,
     856,   884,   907,   908,   909,   910,   914,   918,   927,   936,
     948,   960,   967,   977,   981,   988,   998,  1011,  1012,  1013,
    1014,  1019,  1018,  1032,  1039,  1044,  1053,  1052,  1064,  1063,
    1077,  1078,  1079,  1083,  1084,  1085,  1089,  1101,  1105,  1118,
    1119,  1120,  1124,  1133,  1137,  1151,  1165,  1179,  1200,  1227,
    1253,  1254,  1259,  1258,  1295,  1296,  1311,  1310,  1347,  1348,
    1349,  1355,  1364,  1401,  1400,  1462,  1464,  1463,  1486,  1491,
    1510,  1529,  1548,  1594,  1643,  1644,  1648,  1649,  1653,  1654,
    1655,  1656,  1657,  1658,  1659,  1660,  1661,  1662,  1663,  1667,
    1671,  1684
};
#endif

#if DCDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "UNSIGNED_INTEGER", "REAL", "STRING",
  "HEX_STRING", "IDENTIFIER", "CHAR", "START_DC_FILE", "START_DC_VALUE",
  "KW_DCLASS", "KW_STRUCT", "KW_FROM", "KW_IMPORT", "KW_TYPEDEF",
  "KW_KEYWORD", "KW_INT8", "KW_INT16", "KW_INT32", "KW_INT64", "KW_UINT8",
  "KW_UINT16", "KW_UINT32", "KW_UINT64", "KW_FLOAT32", "KW_FLOAT64",
  "KW_STRING", "KW_BLOB", "KW_CHAR", "';'", "'.'", "'*'", "','", "'/'",
  "'['", "']'", "'{'", "'}'", "':'", "'='", "'('", "')'", "'%'", "'-'",
  "'+'", "$accept", "grammar", "file", "value", "import", "import_module",
  "import_symbols", "import_symbol_list", "import_alternatives", "typedef",
  "typedef_type", "nonmethod_type_with_name", "defined_type",
  "keyword_decl", "keyword_decl_list", "dclass", "$@1",
  "class_inheritance", "class_parents", "defined_class", "class_fields",
  "class_field", "dstruct", "$@2", "struct_fields", "struct_field",
  "named_field", "unnamed_field", "$@3", "field_with_name",
  "field_with_name_as_array", "field_with_name_and_default", "$@4", "$@5",
  "$@6", "method_as_field", "nonmethod_type", "nonmethod_type_no_array",
  "type_with_array", "molecular", "defined_field", "builtin_array_type",
  "numeric_type", "numeric_token_only", "numeric_with_range",
  "numeric_with_modulus", "numeric_with_divisor", "method", "method_body",
  "parameter", "$@7", "param_with_name", "param_with_name_as_array",
  "param_with_name_and_default", "$@8", "$@9", "numeric_range",
  "array_range", "char_or_uint", "small_unsigned_integer", "number",
  "char_or_number", "type_value", "method_value", "$@10",
  "parameter_values", "struct_value", "$@11", "field_values",
  "array_value", "$@12", "element_values", "$@13", "array_expansion",
  "signed_integer", "array_type_token", "numeric_type_token",
  "keyword_list", "empty", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
      59,    46,    42,    44,    47,    91,    93,   123,   125,    58,
      61,    40,    41,    37,    45,    43
};
# endif

#define YYPACT_NINF -188

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-188)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      76,  -188,    14,    31,   259,  -188,  -188,  -188,  -188,  -188,
    -188,    24,  -188,  -188,    68,    71,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,    45,    83,    84,    84,   219,  -188,
    -188,  -188,  -188,    58,  -188,  -188,  -188,  -188,    73,    14,
      91,  -188,  -188,  -188,  -188,  -188,    11,    64,    69,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,    66,    95,  -188,    70,    74,    77,
      -8,  -188,    16,    63,    72,  -188,   104,  -188,    38,    82,
      87,    92,    93,  -188,    12,  -188,    97,  -188,  -188,    23,
    -188,    -6,    94,    78,     8,    84,   109,    38,  -188,    38,
      38,    38,   128,    20,     9,   128,    20,    20,    38,  -188,
    -188,  -188,    96,    90,  -188,  -188,   128,   128,   128,   128,
    -188,  -188,   128,    14,  -188,    91,  -188,   130,   101,  -188,
    -188,  -188,  -188,   106,    64,    64,  -188,   105,   107,   111,
     112,  -188,  -188,  -188,  -188,    98,  -188,   126,  -188,  -188,
    -188,  -188,   100,   103,   108,  -188,    38,  -188,  -188,  -188,
    -188,    73,  -188,  -188,  -188,  -188,  -188,   116,  -188,  -188,
     165,  -188,    84,  -188,  -188,  -188,  -188,  -188,    20,  -188,
    -188,  -188,  -188,  -188,   130,   190,  -188,   110,  -188,  -188,
    -188,   123,  -188,  -188,    27,    28,  -188,   115,     7,    64,
    -188,  -188,    -7,  -188,  -188,   141,  -188,   140,   139,  -188,
      -3,  -188,    38,  -188,    38,  -188,   133,  -188,   168,  -188,
     169,  -188,   168,  -188,   137,   171,  -188,    40,    49,  -188,
     219,  -188,   143,    91,   160,    91,  -188,    91,  -188,  -188,
    -188,  -188,  -188,  -188,    38,  -188,    38,  -188,  -188,  -188,
    -188,  -188,  -188,    91,  -188,    91,   162,    91,   163,    91,
    -188,  -188,  -188,  -188,  -188,  -188
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   171,   171,     0,     2,     4,   125,   126,   128,   129,
     127,   143,   136,   132,     0,     0,     3,    12,    13,   131,
     130,   124,    11,     1,     0,     0,     0,     0,     0,   171,
       5,     8,     9,    24,    10,     6,     7,   142,     0,     0,
       0,   155,   154,    32,    44,    22,     0,    16,    14,    28,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     156,   157,   158,    25,    70,     0,    68,    69,    72,    71,
      82,    85,    83,    84,    80,    86,    29,    30,   171,   125,
     126,   128,   129,   148,     0,   145,   124,   138,   139,     0,
     134,     0,   171,     0,     0,     0,     0,   171,    27,   171,
     171,   171,     0,   171,     0,     0,   171,   171,   171,    31,
     118,   116,     0,   114,   117,   113,     0,     0,     0,     0,
     146,   144,     0,     0,   137,     0,   133,     0,     0,    34,
     171,    19,    15,    18,    20,    17,    23,     0,     0,     0,
       0,    91,   119,   121,   122,     0,   123,   111,   120,   110,
      90,    92,     0,     0,     0,    26,     0,   150,   151,   152,
     153,     0,   149,   140,   141,   135,    38,    35,    36,   171,
       0,    46,     0,    74,    76,    75,    73,    87,     0,    88,
      89,    81,   115,   147,     0,     0,    39,    28,    47,    45,
      58,     0,    49,    50,    51,    52,    53,    54,    55,    21,
     112,    37,    28,    40,    33,     0,   171,    43,     0,    67,
       0,    48,   171,    61,   171,    63,     0,    56,     0,    41,
      42,   169,     0,    93,   100,    68,    95,    97,    98,    99,
       0,    94,     0,     0,     0,     0,    65,     0,    79,    77,
     170,    78,   101,   103,   171,   106,   171,   108,    96,    59,
      62,    60,    64,     0,    57,     0,     0,     0,     0,     0,
      66,   102,   104,   107,   105,   109
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -188,  -188,  -188,  -188,  -188,   173,  -188,  -188,   -84,  -188,
    -188,   174,  -188,  -188,  -188,  -188,  -188,  -188,  -188,    17,
    -188,  -188,  -188,  -188,  -188,  -188,    19,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -164,  -187,  -188,  -188,
     -17,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,    -9,
    -188,  -188,  -188,  -188,  -188,  -188,   -24,   -92,    67,   147,
     118,    46,    -2,   -36,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,    89,   -34,  -188,  -188,  -188,     0
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,    16,    31,    46,   132,   133,    47,    32,
      33,   190,    64,    34,    76,    35,    92,   128,   167,   168,
     185,   205,    36,    93,   170,   191,   192,   193,   237,   194,
     195,   196,   233,   235,   253,   197,    65,    66,    67,   207,
     239,    68,    69,    70,    71,    72,    73,   209,   210,   226,
     255,   227,   228,   229,   257,   259,   145,   112,   113,   114,
     146,   147,    83,    18,    40,    91,    19,    39,    89,    20,
      38,    84,   161,    85,    21,    74,    75,   220,   115
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      17,     5,    22,    88,    86,   137,   198,   138,   139,   140,
     134,   135,   142,   143,    98,    45,   154,     6,     7,     8,
       9,   225,    10,   142,   143,    94,   102,   125,   144,    77,
     230,    23,   218,   103,   208,   104,   126,    87,    90,   231,
     131,   110,    95,   225,   224,   120,   111,   217,   121,    11,
     105,    12,    43,    14,    15,    13,   123,   106,    14,    15,
      37,   124,   212,   214,    14,    15,   224,   213,   215,   148,
     148,    41,   148,   148,    42,   244,    79,    80,    81,    82,
     245,    10,   152,   153,   246,     1,     2,   164,   199,   247,
      44,    45,   129,    78,     6,     7,     8,     9,    96,    10,
      95,    97,    98,   149,   107,    99,   149,   149,    11,   100,
      12,   109,   101,   108,   116,   130,   136,    14,    15,   117,
     232,   163,   234,   165,   118,   119,    11,    86,    12,   122,
     171,   110,   155,   127,   156,    14,    15,   166,   169,   172,
     177,   173,   179,   174,   148,   180,    49,   175,   176,   184,
     181,   208,   256,   211,   258,   216,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,   186,
     178,   219,   187,   222,    13,   238,   240,   242,   243,   249,
     236,   223,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,   188,   251,   202,   262,   264,
      48,   201,    63,   189,   206,   241,   221,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
     203,   248,   150,   182,   200,     0,    49,     0,   204,     0,
       0,   250,     0,   252,     0,   254,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,   141,
     183,   260,   151,   261,     0,   263,     0,   265,     0,     0,
       0,     0,     0,   157,   158,   159,   160,     0,     0,   162,
      24,    25,    26,    27,    28,    29,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    30
};

static const yytype_int16 yycheck[] =
{
       2,     1,     2,    39,    38,    97,   170,    99,   100,   101,
      94,    95,     3,     4,     7,     7,   108,     3,     4,     5,
       6,   208,     8,     3,     4,    14,    34,    33,     8,    29,
      33,     0,    39,    41,    41,    43,    42,    39,    40,    42,
      32,     3,    31,   230,   208,    33,     8,    40,    36,    35,
      34,    37,     7,    44,    45,    41,    33,    41,    44,    45,
      36,    38,    35,    35,    44,    45,   230,    40,    40,   103,
     104,     3,   106,   107,     3,    35,     3,     4,     5,     6,
      40,     8,   106,   107,    35,     9,    10,   123,   172,    40,
       7,     7,    92,    35,     3,     4,     5,     6,    34,     8,
      31,    35,     7,   103,    41,    35,   106,   107,    35,    35,
      37,     7,    35,    41,    32,    37,     7,    44,    45,    32,
     212,   123,   214,   125,    32,    32,    35,   161,    37,    32,
     130,     3,    36,    39,    44,    44,    45,     7,    37,    33,
      42,    36,    42,    36,   178,    42,     7,    36,    36,    33,
      42,    41,   244,    30,   246,    40,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,   169,
      44,    30,     7,    33,    41,     7,     7,    40,     7,    36,
     216,    42,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    36,     7,    36,    36,
      27,   184,    28,    38,   185,   222,   206,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   230,   104,   156,   178,    -1,     7,    -1,    38,    -1,
      -1,   233,    -1,   235,    -1,   237,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,   102,
     161,   253,   105,   255,    -1,   257,    -1,   259,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,    -1,    -1,   122,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     9,    10,    47,    48,   124,     3,     4,     5,     6,
       8,    35,    37,    41,    44,    45,    49,   108,   109,   112,
     115,   120,   124,     0,    11,    12,    13,    14,    15,    16,
      30,    50,    55,    56,    59,    61,    68,    36,   116,   113,
     110,     3,     3,     7,     7,     7,    51,    54,    51,     7,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    57,    58,    82,    83,    84,    87,    88,
      89,    90,    91,    92,   121,   122,    60,   124,    35,     3,
       4,     5,     6,   108,   117,   119,   120,   108,   109,   114,
     108,   111,    62,    69,    14,    31,    34,    35,     7,    35,
      35,    35,    34,    41,    43,    34,    41,    41,    41,     7,
       3,     8,   103,   104,   105,   124,    32,    32,    32,    32,
      33,    36,    32,    33,    38,    33,    42,    39,    63,   124,
      37,    32,    52,    53,    54,    54,     7,   103,   103,   103,
     103,   105,     3,     4,     8,   102,   106,   107,   120,   124,
     106,   105,   102,   102,   103,    36,    44,   105,   105,   105,
     105,   118,   105,   108,   109,   108,     7,    64,    65,    37,
      70,   124,    33,    36,    36,    36,    36,    42,    44,    42,
      42,    42,   104,   119,    33,    66,   124,     7,    30,    38,
      57,    71,    72,    73,    75,    76,    77,    81,    82,    54,
     107,    65,     7,    30,    38,    67,    72,    85,    41,    93,
      94,    30,    35,    40,    35,    40,    40,    40,    39,    30,
     123,   124,    33,    42,    82,    83,    95,    97,    98,    99,
      33,    42,   103,    78,   103,    79,   109,    74,     7,    86,
       7,    86,    40,     7,    35,    40,    35,    40,    95,    36,
     108,    36,   108,    80,   108,    96,   103,   100,   103,   101,
     108,   108,    36,   108,    36,   108
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    48,    48,    48,    48,    48,    48,
      48,    49,    49,    49,    50,    50,    51,    51,    52,    52,
      53,    53,    54,    54,    55,    56,    56,    57,    58,    59,
      60,    60,    62,    61,    63,    63,    64,    64,    65,    66,
      66,    66,    67,    67,    69,    68,    70,    70,    70,    71,
      71,    72,    72,    72,    72,    73,    74,    73,    75,    76,
      76,    78,    77,    79,    77,    80,    77,    81,    82,    82,
      83,    83,    83,    84,    84,    84,    84,    85,    85,    86,
      87,    87,    88,    88,    88,    88,    89,    90,    90,    90,
      91,    92,    92,    93,    93,    94,    94,    95,    95,    95,
      95,    96,    95,    97,    98,    98,   100,    99,   101,    99,
     102,   102,   102,   103,   103,   103,   104,   104,   105,   106,
     106,   106,   107,   107,   108,   108,   108,   108,   108,   108,
     108,   108,   110,   109,   111,   111,   113,   112,   114,   114,
     114,   114,   115,   116,   115,   117,   118,   117,   119,   119,
     119,   119,   119,   119,   120,   120,   121,   121,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   123,
     123,   124
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     2,     2,     2,     2,     2,
       2,     1,     1,     1,     2,     4,     1,     3,     1,     1,
       1,     3,     1,     3,     1,     2,     4,     2,     1,     2,
       1,     2,     0,     7,     1,     2,     1,     3,     1,     1,
       2,     3,     2,     1,     0,     6,     1,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     0,     4,     1,     4,
       4,     0,     4,     0,     4,     0,     5,     2,     1,     1,
       1,     1,     1,     4,     4,     4,     4,     3,     3,     1,
       1,     4,     1,     1,     1,     1,     1,     4,     4,     4,
       3,     3,     3,     2,     2,     2,     3,     1,     1,     1,
       1,     0,     4,     2,     4,     4,     0,     4,     0,     4,
       1,     1,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     4,     1,     3,     0,     4,     1,     1,
       3,     3,     2,     0,     4,     1,     0,     4,     1,     3,
       3,     3,     3,     3,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if DCDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !DCDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !DCDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
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
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
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

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

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

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
      yychar = yylex ();
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 11:
#line 232 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		parsed_value = Value();
	}
#line 1564 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 236 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		parsed_value = (yyvsp[0].value);
		if(!type_stack.empty()) depth_error(0, "type");
	}
#line 1573 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 241 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		parsed_value = (yyvsp[0].value);
		if(!type_stack.empty()) depth_error(0, "method");
	}
#line 1582 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 249 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		Import* import = new Import((yyvsp[0].str));
		parsed_module->add_import(unique_ptr<Import>(import));
	}
#line 1591 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 254 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		Import* import = new Import((yyvsp[-2].str));
		import->symbols.assign((yyvsp[0].strings).begin(), (yyvsp[0].strings).end());
		parsed_module->add_import(unique_ptr<Import>(import));
	}
#line 1601 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 262 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 1607 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 264 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.str) = (yyvsp[-2].str) + string(".") + (yyvsp[0].str);
	}
#line 1615 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 270 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.strings) = (yyvsp[0].strings); }
#line 1621 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 272 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.strings) = vector<string>();
	}
#line 1629 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 279 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.strings) = vector<string>(1, (yyvsp[0].str));
	}
#line 1637 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 283 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyvsp[-2].strings).push_back((yyvsp[0].str));
		(yyval.strings) = (yyvsp[-2].strings);
	}
#line 1646 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 290 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.str) = (yyvsp[0].str); }
#line 1652 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 292 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.str) = (yyvsp[-2].str) + string("/") + (yyvsp[0].str);
	}
#line 1660 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 299 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[0].nametype).type == nullptr)
		{
			// Ignore this typedef, it should have already produced an error
			break;
		}

		// Set the type's typedef
		(yyvsp[0].nametype).type->set_alias((yyvsp[0].nametype).name);

		bool type_added	= parsed_module->add_typedef((yyvsp[0].nametype).name, (yyvsp[0].nametype).type);
		if(!type_added)
		{
			// Lets be really descriptive about why this failed
			Type* dtype = parsed_module->type_by_name((yyvsp[0].nametype).name);
			if(dtype == nullptr)
			{
				dcerror("Unknown error adding typedef to module.");
				break;
			}

			Struct* dstruct = dtype->as_struct();
			if(dstruct == nullptr)
			{
				dcerror("Cannot add 'typedef " + (yyvsp[0].nametype).name
				             + "' to module because a typedef was already declared with that name.");
				break;
			}

			if(dstruct->as_class())
			{
				dcerror("Cannot add 'typedef " + (yyvsp[0].nametype).name
				             + "' to module because a class was already declared with that name.");
			}
			else
			{
				dcerror("Cannot add 'typedef " + (yyvsp[0].nametype).name
				             + "' to module because a struct was already declared with that name.");
			}
		}
	}
#line 1706 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 344 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.nametype) = (yyvsp[0].nametype);
	}
#line 1714 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 348 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.nametype) = (yyvsp[-3].nametype);
		(yyval.nametype).type = new Array((yyvsp[-3].nametype).type, (yyvsp[-1].range));
	}
#line 1723 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 356 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		DCToken::NameType nt;
		nt.type = (yyvsp[-1].dtype);
		nt.name = (yyvsp[0].str);
		(yyval.nametype) = nt;
	}
#line 1734 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 366 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		Type* dtype = parsed_module->type_by_name((yyvsp[0].str));
		if(dtype == nullptr)
		{
			dcerror("Subtype '" + string((yyvsp[0].str)) + "' has not been declared.");
			(yyval.dtype) = nullptr;
			break;
		}

		(yyval.dtype) = dtype;
	}
#line 1750 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 386 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		parsed_module->add_keyword((yyvsp[0].str));
	}
#line 1758 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 393 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_class = new Class(parsed_module, (yyvsp[0].str));
	}
#line 1766 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 397 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		bool class_added = parsed_module->add_class(unique_ptr<Class>(current_class));
		if(!class_added)
		{
			// Lets be really descriptive about why this failed
			Type* dtype = parsed_module->type_by_name(current_class->name());
			if(dtype == nullptr)
			{
				dcerror("Unknown error adding class to module.");
				break;
			}

			Struct* dstruct = dtype->as_struct();
			if(dstruct == nullptr)
			{
				dcerror("Cannot add 'dclass " + current_class->name()
				             + "' to module because a typedef was already declared with that name.");
				break;
			}

			if(dstruct->as_class())
			{
				dcerror("Cannot add 'dclass " + current_class->name()
				             + "' to module because a class was already declared with that name.");
			}
			else
			{
				dcerror("Cannot add 'dclass " + current_class->name()
				             + "' to module because a struct was already declared with that name.");
			}
		}
	}
#line 1803 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 438 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[0].dclass) != nullptr)
		{
			current_class->add_parent((yyvsp[0].dclass));
		}
	}
#line 1814 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 445 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[0].dclass) != nullptr)
		{
			current_class->add_parent((yyvsp[0].dclass));
		}
	}
#line 1825 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 455 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		Type* dtype = parsed_module->type_by_name((yyvsp[0].str));
		if(dtype == nullptr)
		{
			dcerror("'dclass " + string((yyvsp[0].str)) + "' has not been declared.");
			(yyval.dclass) = nullptr;
			break;
		}

		Struct* dstruct = dtype->as_struct();
		if(dstruct == nullptr)
		{
			dcerror("class cannot inherit from non-class type '" + string((yyvsp[0].str)) + "'.");
			(yyval.dclass) = nullptr;
			break;
		}

		Class* dclass = dstruct->as_class();
		if(dclass == nullptr)
		{
			dcerror("class cannot inherit from struct type '" + string((yyvsp[0].str)) + "'.");
			(yyval.dclass) = nullptr;
			break;
		}

		(yyval.dclass) = dclass;
	}
#line 1857 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 488 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[-1].dfield) == nullptr)
		{
			// Ignore this field, it should have already generated a parser error
			break;
		}

		bool field_added = current_class->add_field(unique_ptr<Field>((yyvsp[-1].dfield)));
		if(!field_added)
		{
			// Lets be really descriptive about why this failed
			if(current_class->field_by_name((yyvsp[-1].dfield)->name()))
			{
				dcerror("Cannot add field '" + (yyvsp[-1].dfield)->name()
				             + "', a field with that name already exists in 'dclass "
				             + current_class->name() + "'.");
			}
			else if(current_class->name() == (yyvsp[-1].dfield)->name())
			{
				if((yyvsp[-1].dfield)->as_molecular())
				{
					dcerror("Cannot use a molecular field as a constructor.");
				}
				else
				{
					dcerror("The constructor must be the first field in the class.");
				}
			}
			else
			{
				dcerror("Unknown error adding field to class.");
			}
		}
	}
#line 1896 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 526 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[-1].dfield) == nullptr)
		{
			// Ignore this field, it should have already generated a parser error
			(yyval.dfield) = nullptr;
			break;
		}

		if((yyvsp[-1].dfield)->name().empty())
		{
			dcerror("An unnamed field can't be defined in a class.");
			(yyval.dfield) = nullptr;
			break;
		}

		// Add the keywords to the class
		for(auto it = (yyvsp[0].strings).begin(); it != (yyvsp[0].strings).end(); ++it)
		{
			(yyvsp[-1].dfield)->add_keyword(*it);
		}

		(yyval.dfield) = (yyvsp[-1].dfield);
	}
#line 1924 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 550 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dfield) = (Field*)(yyvsp[0].dmolecule);
	}
#line 1932 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 557 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_struct = new Struct(parsed_module, (yyvsp[0].str));
	}
#line 1940 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 561 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		bool struct_added = parsed_module->add_struct(unique_ptr<Struct>(current_struct));
		if(!struct_added)
		{
			// Lets be really descriptive about why this failed
			Type* dtype = parsed_module->type_by_name(current_struct->name());
			if(dtype == nullptr)
			{
				dcerror("Unknown error adding struct to module.");
				break;
			}

			Struct* dstruct = dtype->as_struct();
			if(dstruct == nullptr)
			{
				dcerror("Cannot add 'struct " + current_struct->name()
				             + "' to module because a typedef was already declared with that name.");
				break;
			}

			if(dstruct->as_class())
			{
				dcerror("Cannot add 'struct " + current_struct->name()
				             + "' to module because a class was already declared with that name.");
			}
			else
			{
				dcerror("Cannot add 'struct " + current_struct->name()
				             + "' to module because a struct was already declared with that name.");
			}
		}
	}
#line 1977 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 599 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[-1].dfield) == nullptr || (yyvsp[-1].dfield)->type() == nullptr)
		{
			// Ignore this field, it should have already generated a parser error
			break;
		}

		if(!current_struct->add_field(unique_ptr<Field>((yyvsp[-1].dfield))))
		{
			// Lets be really descriptive about why this failed
			if(current_struct->field_by_name((yyvsp[-1].dfield)->name()))
			{
				dcerror("Cannot add field '" + (yyvsp[-1].dfield)->name()
				             + "', a field with that name already exists in 'struct "
				             + current_struct->name() + "'.");
			}
			else if(current_struct->name() == (yyvsp[-1].dfield)->name())
			{
				dcerror("A constructor can't be defined in a struct.");
			}
			else if((yyvsp[-1].dfield)->type()->as_method())
			{
				dcerror("A method can't be defined in a struct.");
			}
			else
			{
				dcerror("Unknown error adding field to struct.");
			}
		}
	}
#line 2012 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 645 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dfield) = new Field((yyvsp[0].dtype));
	}
#line 2020 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 649 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[-1].dtype), 0));
	}
#line 2029 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 654 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		Field* field = new Field((yyvsp[-3].dtype));
		if(!type_stack.empty()) depth_error(0, "unnamed field");
		field->set_default_value((yyvsp[0].value));
		(yyval.dfield) = field;
	}
#line 2040 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 664 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dfield) = new Field((yyvsp[0].nametype).type, (yyvsp[0].nametype).name);
	}
#line 2048 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 671 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyvsp[-3].dfield)->set_type(new Array((yyvsp[-3].dfield)->type(), (yyvsp[-1].range)));
		(yyval.dfield) = (yyvsp[-3].dfield);
	}
#line 2057 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 676 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyvsp[-3].dfield)->set_type(new Array((yyvsp[-3].dfield)->type(), (yyvsp[-1].range)));
		(yyval.dfield) = (yyvsp[-3].dfield);
	}
#line 2066 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 684 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[-1].dfield)->type(), 0));
	}
#line 2075 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 689 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!type_stack.empty()) depth_error(0, "field '" + (yyvsp[-3].dfield)->name() + "'");
		(yyvsp[-3].dfield)->set_default_value((yyvsp[0].value));
		(yyval.dfield) = (yyvsp[-3].dfield);
	}
#line 2085 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 695 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[-1].dfield)->type(), 0));
	}
#line 2094 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 700 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!type_stack.empty()) depth_error(0, "field '" + (yyvsp[-3].dfield)->name() + "'");
		(yyvsp[-3].dfield)->set_default_value((yyvsp[0].value));
		(yyval.dfield) = (yyvsp[-3].dfield);
	}
#line 2104 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 706 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[-2].dfield)->type(), 0));
	}
#line 2113 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 711 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!type_stack.empty()) depth_error(0, "method");
		(yyvsp[-4].dfield)->set_default_value((yyvsp[-2].value));
		(yyval.dfield) = (yyvsp[-4].dfield);
	}
#line 2123 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 720 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dfield) = new Field((yyvsp[0].dmethod), (yyvsp[-1].str));
	}
#line 2131 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 732 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[0].dtype) == nullptr)
		{
			// defined_type should have output an error, pass nullptr upstream
			(yyval.dtype) = nullptr;
			break;
		}

		if((yyvsp[0].dtype)->subtype() == kTypeMethod)
		{
			dcerror("Cannot use a method type here.");
			(yyval.dtype) = nullptr;
			break;
		}

		(yyval.dtype) = (yyvsp[0].dtype);
	}
#line 2153 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 750 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dtype) = (Type*)(yyvsp[0].dnumeric);
	}
#line 2161 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 758 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dtype) = new Array((yyvsp[-3].dnumeric), (yyvsp[-1].range));
	}
#line 2169 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 762 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dtype) = new Array((yyvsp[-3].dtype), (yyvsp[-1].range));
	}
#line 2177 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 766 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dtype) = new Array((yyvsp[-3].dtype), (yyvsp[-1].range));
	}
#line 2185 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 770 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dtype) = new Array((yyvsp[-3].dtype), (yyvsp[-1].range));
	}
#line 2193 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 777 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		MolecularField* mol = new MolecularField(current_class, (yyvsp[-2].str));
		if((yyvsp[0].dfield) == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.dmolecule) = mol;
			break;
		}

		bool field_added = mol->add_field((yyvsp[0].dfield));
		if(!field_added)
		{
			if((yyvsp[0].dfield)->as_molecular())
			{
				dcerror("Cannot add molecular '" + (yyvsp[0].dfield)->name() + "' to a molecular field.");
			}
			else
			{
				dcerror("Unkown error adding field " + (yyvsp[0].dfield)->name() + " to molecular '"
				             + (yyvsp[-2].str) + "'.");
			}
		}

		(yyval.dmolecule) = mol;
	}
#line 2223 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 803 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[0].dfield) == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.dmolecule) = (yyvsp[-2].dmolecule);
			break;
		}

		bool field_added = (yyvsp[-2].dmolecule)->add_field((yyvsp[0].dfield));
		if(!field_added)
		{
			if((yyvsp[0].dfield)->as_molecular())
			{
				dcerror("Cannot add molecular '" + (yyvsp[0].dfield)->name() + "' to a molecular field.");
			}
			else if(!(yyvsp[-2].dmolecule)->has_matching_keywords(*(yyvsp[0].dfield)))
			{
				dcerror("Mismatched keywords in molecular between " +
					(yyvsp[-2].dmolecule)->nth_field(0)->name() + " and " + (yyvsp[0].dfield)->name() + ".");
			}
			else
			{
				dcerror("Unkown error adding field " + (yyvsp[0].dfield)->name() + " to molecular '"
				             + (yyvsp[-2].dmolecule)->name() + "'.");
			}
		}

		(yyval.dmolecule) = (yyvsp[-2].dmolecule);
	}
#line 2257 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 835 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!current_class)
		{
			dcerror("Field '" + (yyvsp[0].str) + "' not defined in current class.");
			(yyval.dfield) = nullptr;
			break;
		}

		Field *field = current_class->field_by_name((yyvsp[0].str));
		if(field == nullptr)
		{
			dcerror("Field '" + (yyvsp[0].str) + "' not defined in current class.");
			(yyval.dfield) = nullptr;
			break;
		}

		(yyval.dfield) = field;
	}
#line 2280 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 857 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[0].subtype) == kTypeString)
		{
			if(default_string == nullptr)
			{
				default_string = new Array(new Numeric(kTypeChar));
				default_string->set_alias("string");
			}

			(yyval.dtype) = default_string;
		}
		else if((yyvsp[0].subtype) == kTypeBlob)
		{
			if(default_blob == nullptr)
			{
				default_blob = new Array(new Numeric(kTypeUint8));
				default_blob->set_alias("blob");
			}

			(yyval.dtype) = default_blob;
		}
		else
		{
			dcerror("Found builtin Array not handled by parser.");
			(yyval.dtype) = nullptr;
		}
	}
#line 2312 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 885 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[-3].subtype) == kTypeString)
		{
			Array* arr = new Array(new Numeric(kTypeChar), (yyvsp[-1].range));
			arr->set_alias("string");
			(yyval.dtype) = arr;
		}
		else if((yyvsp[-3].subtype) == kTypeBlob)
		{
			Array* arr = new Array(new Numeric(kTypeUint8), (yyvsp[-1].range));
			arr->set_alias("blob");
			(yyval.dtype) = arr;
		}
		else
		{
			dcerror("Found builtin Array not handled by parser.");
			(yyval.dtype) = nullptr;
		}
	}
#line 2336 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 914 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.dnumeric) = new Numeric((yyvsp[0].subtype)); }
#line 2342 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 919 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!(yyvsp[-3].dnumeric)->set_range((yyvsp[-1].range)))
		{
			dcerror("Invalid range for type.");
		}

		(yyval.dnumeric) = (yyvsp[-3].dnumeric);
	}
#line 2355 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 928 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!(yyvsp[-3].dnumeric)->set_range((yyvsp[-1].range)))
		{
			dcerror("Invalid range for type.");
		}

		(yyval.dnumeric) = (yyvsp[-3].dnumeric);
	}
#line 2368 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 937 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!(yyvsp[-3].dnumeric)->set_range((yyvsp[-1].range)))
		{
			dcerror("Invalid range for type.");
		}

		(yyval.dnumeric) = (yyvsp[-3].dnumeric);
	}
#line 2381 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 949 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!(yyvsp[-2].dnumeric)->set_modulus((yyvsp[0].real)))
		{
			dcerror("Invalid modulus for type.");
		}

		(yyval.dnumeric) = (yyvsp[-2].dnumeric);
	}
#line 2394 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 961 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!(yyvsp[-2].dnumeric)->set_divisor((yyvsp[0].uint32)))
		{
			dcerror("Invalid divisor for type.");
		}
	}
#line 2405 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 968 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!(yyvsp[-2].dnumeric)->set_divisor((yyvsp[0].uint32)))
		{
			dcerror("Invalid divisor for type.");
		}
	}
#line 2416 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 978 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dmethod) = new Method();
	}
#line 2424 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 982 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dmethod) = (yyvsp[-1].dmethod);
	}
#line 2432 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 989 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		Method* fn = new Method();
		bool param_added = fn->add_param(unique_ptr<Parameter>((yyvsp[0].dparam)));
		if(!param_added)
		{
			dcerror("Unknown error adding parameter to method.");
		}
		(yyval.dmethod) = fn;
	}
#line 2446 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 999 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		bool param_added = (yyvsp[-2].dmethod)->add_param(unique_ptr<Parameter>((yyvsp[0].dparam)));
		if(!param_added)
		{
			dcerror("Cannot add parameter '" + (yyvsp[0].dparam)->name()
			             + "', a parameter with that name is already used in this method.");
		}
		(yyval.dmethod) = (yyvsp[-2].dmethod);
	}
#line 2460 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1015 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dparam) = new Parameter((yyvsp[0].dtype));
	}
#line 2468 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1019 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[-1].dtype),0));
	}
#line 2477 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1024 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		Parameter* param = new Parameter((yyvsp[-3].dtype));
		if(!type_stack.empty()) depth_error(0, "type");
		param->set_default_value((yyvsp[0].value));
		(yyval.dparam) = param;
	}
#line 2488 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1033 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.dparam) = new Parameter((yyvsp[-1].dtype), (yyvsp[0].str));
	}
#line 2496 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1040 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyvsp[-3].dparam)->set_type(new Array((yyvsp[-3].dparam)->type(), (yyvsp[-1].range)));
		(yyval.dparam) = (yyvsp[-3].dparam);
	}
#line 2505 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1045 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyvsp[-3].dparam)->set_type(new Array((yyvsp[-3].dparam)->type(), (yyvsp[-1].range)));
		(yyval.dparam) = (yyvsp[-3].dparam);
	}
#line 2514 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1053 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[-1].dparam)->type(), 0));
	}
#line 2523 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1058 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!type_stack.empty()) depth_error(0, "parameter '" + (yyvsp[-3].dparam)->name() + "'");
		(yyvsp[-3].dparam)->set_default_value((yyvsp[0].value));
		(yyval.dparam) = (yyvsp[-3].dparam);
	}
#line 2533 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1064 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[-1].dparam)->type(), 0));
	}
#line 2542 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1069 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!type_stack.empty()) depth_error(0, "parameter '" + (yyvsp[-3].dparam)->name() + "'");
		(yyvsp[-3].dparam)->set_default_value((yyvsp[0].value));
		(yyval.dparam) = (yyvsp[-3].dparam);
	}
#line 2552 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1077 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.range) = NumericRange(); }
#line 2558 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1078 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.range) = NumericRange((yyvsp[0].real), (yyvsp[0].real)); }
#line 2564 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1079 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.range) = NumericRange((yyvsp[-2].real), (yyvsp[0].real)); }
#line 2570 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1083 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.range) = NumericRange(); }
#line 2576 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1084 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.range) = NumericRange((yyvsp[0].uint32), (yyvsp[0].uint32)); }
#line 2582 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1085 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.range) = NumericRange((yyvsp[-2].uint32), (yyvsp[0].uint32)); }
#line 2588 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1090 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[0].str).length() != 1)
		{
			dcerror("Single character required.");
			(yyval.uint32) = 0;
		}
		else
		{
			(yyval.uint32) = (unsigned char)(yyvsp[0].str)[0];
		}
	}
#line 2604 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1106 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		unsigned int num = (unsigned int)(yyvsp[0].uint64);
		if(num != (yyvsp[0].uint64))
		{
			dcerror("Number out of range.");
			(yyval.uint32) = 1;
		}
		(yyval.uint32) = num;
	}
#line 2618 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1118 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.real) = (double)(yyvsp[0].uint64); }
#line 2624 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1119 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.real) = (double)(yyvsp[0].int64); }
#line 2630 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1125 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[0].str).length() != 1) {
			dcerror("Single character required.");
			(yyval.real) = 0;
		} else {
			(yyval.real) = (double)(unsigned char)(yyvsp[0].str)[0];
		}
	}
#line 2643 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1138 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("signed integer");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = Value();
			break;
		}
		type_stack.pop(); // Remove numeric type from stack

		(yyval.value) = number_value(dtype, (yyvsp[0].int64));
	}
#line 2661 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1152 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("unsigned integer");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = Value();
			break;
		}
		type_stack.pop(); // Remove numeric type from stack

		(yyval.value) = number_value(dtype, (yyvsp[0].uint64));
	}
#line 2679 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1166 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("floating point");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = Value();
			break;
		}
		type_stack.pop(); // Remove numeric type from stack

		(yyval.value) = number_value(dtype, (yyvsp[0].real));
	}
#line 2697 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1180 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("char");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = Value();
			break;
		}
		type_stack.pop(); // Remove char type from stack

		if(dtype->subtype() == kTypeChar) {
			dcerror("Cannot use char value for non-string type '"
			             + format_subtype(dtype->subtype()) + "'.");
		}

		Value value(dtype);
		value.m_char = (yyvsp[0].str);
		(yyval.value) = value;
	}
#line 2722 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1201 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("string");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = Value();
			break;
		}
		type_stack.pop(); // Remove string type from stack

		if(dtype->subtype() == kTypeString || dtype->subtype() == kTypeBlob) {
			if((yyvsp[0].str).length() != dtype->fixed_size()) {
				dcerror("Value for fixed-length string has incorrect length.");
			}

			Value value(dtype);
			value.m_string = (yyvsp[0].str);
			(yyval.value) = value;
		}
		else {
			dcerror("Cannot use string value for non-string type '"
			             + format_subtype(dtype->subtype()) + "'.");
			(yyval.value) = Value();
		}
	}
#line 2753 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1228 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("hex-string");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = Value();
			break;
		}
		type_stack.pop(); // Remove type from stack

		if(dtype->subtype() == kTypeBlob) {
			if((yyvsp[0].str).length() != dtype->fixed_size()) {
				dcerror("Value for fixed-length blob has incorrect length.");
			}

			Value value(dtype)
			value.m_blob = as_buffer((yyvsp[0].str));
			(yyval.value) = value;
		} else {
			dcerror("Cannot use hex value for non-blob type '"
			             + format_subtype(dtype->subtype()) + "'.");
			(yyval.value) = as_buffer((yyvsp[0].str));
		}
	}
#line 2783 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1259 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("method");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			break;
		}

		if(dtype->as_method()) {
			current_depth++;
			const Method* method = dtype->as_method();

			size_t num_params = method->num_params();
			for(unsigned int i = 1; i <= num_params; ++i) {
				// Reverse iteration
				const Parameter* param = method->nth_param(num_params-i);
				// Add parameter types to stack such that the first is on top
				type_stack.push(TypeAndDepth(param->type(), current_depth));
			}
		} else {
			dcerror("Cannot use method-value for non-method type '"
			             + format_subtype(dtype->subtype()) + "'.");
		}
	}
#line 2813 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1285 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(type_stack.top().type->as_method()) {
			current_depth--;
		}
		type_stack.pop(); // Remove method type from the stack
		(yyval.value) = (yyvsp[-1].value);
	}
#line 2825 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1297 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if((yyvsp[-2].value)->type()->subtype() == kTypeNone) {
			// Ignore this argument list, it should have already generated an error
			(yyval.value) = Value();
		} else {
			vector<uint8_t> buf = (yyvsp[-2].value);
			pack_value((yyvsp[0].value), buf);
			(yyval.value) = buf;
		}
	}
#line 2840 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1311 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("struct");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			break;
		}

		if(dtype->as_struct()) {
			current_depth++;
			const Struct* dstruct = dtype->as_struct();

			size_t num_fields = dstruct->num_fields();
			for(unsigned int i = 1; i <= num_fields; ++i) {
				// Reverse iteration
				const Field* field = dstruct->nth_field(num_fields-i);
				// Add field types to stack such that the first is on top
				type_stack.push(TypeAndDepth(field->type(), current_depth));
			}
		} else {
			dcerror("Cannot use struct-composition for non-struct type '"
			             + format_subtype(dtype->subtype()) + "'.");
		}
	}
#line 2870 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1337 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(type_stack.top().type->as_struct()) {
			current_depth--;
		}
		type_stack.pop(); // Remove method type from the stack
		(yyval.value) = (yyvsp[-1].value);
	}
#line 2882 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1350 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		vector<uint8_t> buf = (yyvsp[-2].value);
		pack_value((yyvsp[0].value), buf);
		(yyval.value) = buf;
	}
#line 2892 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1356 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		vector<uint8_t> buf = (yyvsp[-2].value);
		pack_value((yyvsp[0].value), buf);
		(yyval.value) = buf;
	}
#line 2902 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1365 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("array");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = vector<uint8_t>();;
			break;
		}
		type_stack.pop();
		if(!dtype->as_array()) {
			dcerror("Cannot use array-composition for non-array type '"
			             + format_subtype(dtype->subtype()) + "'.");
			(yyval.value) = vector<uint8_t>();;
			break;
		}
		const Array* array = dtype->as_array();

		if(array->array_size() > 0) {
			// For fixed size arrays, an empty array is an error
			dcerror("Fixed-sized array of size "
			             + to_string(array->array_size())
			             + " can't have 0 elements.");
		} else if(array->has_range()) {
			// If we have a range, 0 elements must be valid in the range
			if(0 < array->range().min.uinteger) {
				dcerror("Too few elements in array value, minimum "
				             + to_string(array->range().min.uinteger) + ".");
			}
		}

		// Since a fixed-size array can't have zero elements, this always
		// the default value for a varsize array, which is the length-tag 0.
		(yyval.value) = vector<uint8_t>(sizeof(sizetag_t), 0);
	}
#line 2942 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1401 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!check_depth()) depth_error("array");

		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			break;
		}

		if(dtype->as_array()) {
			const Array* array = dtype->as_array();

			// For arrays we're going to do something pretty hacky:
			//    For every element we had we are going to increment the depth,
			//    and after we finish the element_values production we will compare
			//    the current_depth to the depth of our original symbol to check
			//    if the array size is proper.
			type_stack.push(TypeAndDepth(array->element_type(), current_depth));
		} else {
			dcerror("Cannot use array-composition for non-array type '"
			             + format_subtype(dtype->subtype()) + "'.");
		}
	}
#line 2970 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1425 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(type_stack.top().type->as_array()) {
			uint64_t actual_size = current_depth - type_stack.top().depth;

			const Type* dtype = type_stack.top().type;
			if(dtype == nullptr) {
				// Ignore this field, it should have already generated an error
				(yyval.value) = vector<uint8_t>();
				break;
			}

			const Array* array = dtype->as_array();
			if(array->has_range()) {
				if(actual_size > array->range().max.uinteger) {
					dcerror("Too many elements in array value, maximum "
					             + to_string(array->range().max.uinteger) + ".");
				} else if(actual_size < array->range().min.uinteger) {
					dcerror("Too few elements in array value, minimum "
					             + to_string(array->range().min.uinteger) + ".");
				}
			}

			if(array->array_size() == 0) {
				vector<uint8_t> buf = as_buffer(sizetag_t((yyvsp[-1].value).size()));
				pack_value((yyvsp[-1].value), buf);
				(yyval.value) = buf;
			} else {
				(yyval.value) = (yyvsp[-1].value);
			}
			current_depth = type_stack.top().depth;
		}

		type_stack.pop(); // Remove array type from the stack
	}
#line 3009 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1464 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		// We popped off the only element we added, so we're back to the array
		// Don't increment the depth; the array_expansion will add to
		// the current_depth depending on the number of elements it adds.
		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			break;
		}
		const Array* array = dtype->as_array();
		type_stack.push(TypeAndDepth(array->element_type(), current_depth));
	}
#line 3026 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1477 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		// $4 packed onto end of $1
		std::vector<uint8_t> buf = (yyvsp[-3].value);
		pack_value((yyvsp[0].value), buf);
		(yyval.value) = buf;
	}
#line 3037 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1487 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		current_depth++;
		(yyval.value) = (yyvsp[0].value);
	}
#line 3046 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1492 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = vector<uint8_t>();
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[0].uint32); // For arrays, we add 1 to the depth per element

		vector<uint8_t> buf;
		vector<uint8_t> base = number_value(dtype->subtype(), (yyvsp[-2].int64));
		buf.reserve(base.size() * (yyvsp[0].uint32));
		for(unsigned int i = 0; i < (yyvsp[0].uint32); ++i) {
			pack_value(base, buf);
		}
		(yyval.value) = buf;
	}
#line 3069 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1511 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = vector<uint8_t>();
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[0].uint32); // For arrays, we add 1 to the depth per element

		vector<uint8_t> buf;
		vector<uint8_t> base = number_value(dtype->subtype(), (yyvsp[-2].uint64));
		buf.reserve(base.size() * (yyvsp[0].uint32));
		for(unsigned int i = 0; i < (yyvsp[0].uint32); ++i) {
			pack_value(base, buf);
		}
		(yyval.value) = buf;
	}
#line 3092 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1530 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = vector<uint8_t>();
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[0].uint32); // For arrays, we add 1 to the depth per element

		vector<uint8_t> buf;
		vector<uint8_t> base = number_value(dtype->subtype(), (yyvsp[-2].real));
		buf.reserve(base.size() * (yyvsp[0].uint32));
		for(unsigned int i = 0; i < (yyvsp[0].uint32); ++i) {
			pack_value(base, buf);
		}
		(yyval.value) = buf;
	}
#line 3115 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1549 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = vector<uint8_t>();
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[0].uint32); // For arrays, we add 1 to the depth per element

		if(dtype->subtype() == kTypeString) {
			if((yyvsp[-2].str).length() != dtype->fixed_size()) {
				dcerror("Value for fixed-length string has incorrect length.");
			}

			vector<uint8_t> buf;
			buf.reserve((yyvsp[-2].str).length() * (yyvsp[0].uint32));
			for(unsigned int i = 0; i < (yyvsp[0].uint32); ++i) {
				pack_value((yyvsp[-2].str), buf);
			}
			(yyval.value) = buf;
		} else if(dtype->subtype() == kTypeVarstring) {
			// Prepend length tag
			vector<uint8_t> buf = as_buffer((yyvsp[-2].str).length());
			vector<uint8_t> base = as_buffer((yyvsp[-2].str));
			buf.reserve(base.size() * (yyvsp[0].uint32));
			for(unsigned int i = 0; i < (yyvsp[0].uint32); ++i) {
				pack_value(base, buf);
			}
			(yyval.value) = buf;

			// @TODO(Kevin): Check for range limits

		} else {
			dcerror("Cannot use string value for non-string type '"
			             + format_subtype(dtype->subtype()) + "'.");

			vector<uint8_t> buf;
			buf.reserve((yyvsp[-2].str).length() * (yyvsp[0].uint32));
			for(unsigned int i = 0; i < (yyvsp[0].uint32); ++i) {
				pack_value((yyvsp[-2].str), buf);
			}
			(yyval.value) = buf;
		}
	}
#line 3165 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1595 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		const Type* dtype = type_stack.top().type;
		if(dtype == nullptr) {
			// Ignore this field, it should have already generated an error
			(yyval.value) = vector<uint8_t>();
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[0].uint32); // For arrays, we add 1 to the depth per element

		if(dtype->subtype() == kTypeBlob) {
			if((yyvsp[-2].str).length() != dtype->fixed_size()) {
				dcerror("Value for fixed-length blob has incorrect length.");
			}

			vector<uint8_t> buf;
			buf.reserve((yyvsp[-2].str).length() * (yyvsp[0].uint32));
			for(unsigned int i = 0; i < (yyvsp[0].uint32); ++i) {
				pack_value((yyvsp[-2].str), buf);
			}
			(yyval.value) = buf;
		} else if(dtype->subtype() == kTypeVarblob) {
			// Prepend length tag
			vector<uint8_t> buf = as_buffer((yyvsp[-2].str).length());
			vector<uint8_t> base = as_buffer((yyvsp[-2].str));
			buf.reserve(base.size() * (yyvsp[0].uint32));
			for(unsigned int i = 0; i < (yyvsp[0].uint32); ++i) {
				pack_value(base, buf);
			}
			(yyval.value) = buf;

			// @TODO(Kevin): Check for range limits

		} else {
			dcerror("Cannot use hex value for non-blob type '"
			             + format_subtype(dtype->subtype()) + "'.");

			vector<uint8_t> buf;
			buf.reserve((yyvsp[-2].str).length() * (yyvsp[0].uint32));
			for(unsigned int i = 0; i < (yyvsp[0].uint32); ++i) {
				pack_value((yyvsp[-2].str), buf);
			}
			(yyval.value) = buf;
		}
	}
#line 3215 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1643 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.int64) = int64_t((yyvsp[0].uint64)); }
#line 3221 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1644 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.int64) = -int64_t((yyvsp[0].uint64)); }
#line 3227 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1648 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeString; }
#line 3233 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1649 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeBlob; }
#line 3239 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1653 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeChar; }
#line 3245 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1654 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeInt8; }
#line 3251 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1655 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeInt16; }
#line 3257 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1656 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeInt32; }
#line 3263 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1657 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeInt64; }
#line 3269 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1658 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeUint8; }
#line 3275 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1659 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeUint16; }
#line 3281 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1660 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeUint32; }
#line 3287 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1661 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeUint64; }
#line 3293 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1662 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeFloat32; }
#line 3299 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1663 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    { (yyval.subtype) = kTypeFloat64; }
#line 3305 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1668 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		(yyval.strings) = vector<string>();
	}
#line 3313 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1672 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1646  */
    {
		if(!parsed_module->has_keyword((yyvsp[0].str)))
		{
			dcerror("Keyword '" + (yyvsp[0].str) + "' has not been declared.");
			break;
		}

		(yyvsp[-1].strings).push_back((yyvsp[0].str));
		(yyval.strings) = (yyvsp[-1].strings);
	}
#line 3328 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
    break;


#line 3332 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.cpp" /* yacc.c:1646  */
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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 1688 "/media/storage/Devel/astron/bamboo/src/dcfile/parser.ypp" /* yacc.c:1906  */
 /* Start helper function section */
namespace bamboo
{ // open namespace bamboo


Module *read_dcfile(istream& in, const string& filename) {
    Module *m = new Module();
    bool ok = parse_dcfile(m, in, filename);
    if(ok) {
        return m;
    }

    return nullptr;
}
Module *read_dcfile(const string& filename) {
    Module *m = new Module();
    bool ok = parse_dcfile(m, filename);
    if(ok) {
        return m;
    }

    return nullptr;
}
bool parse_dcfile(Module *m, istream& in, const string& filename) {
    dcparser_init_file(in, filename, *m);
    dcparse();
    dcparser_cleanup();
    return (dc_errors() == 0);
}
bool parse_dcfile(Module *m, const string& filename) {
    ifstream in;
    in.open(filename.c_str());
    if(!in) {
        cerr << "Cannot open " << filename << " for reading.\n";
        return false;
    }
    return parse_dcfile(m, in, filename);
}
Value parse_dcvalue(const Type *dtype, const string& formatted, bool& err) {
    istringstream strm(formatted);
    return parse_dcvalue(dtype, strm, err);
}
Value parse_dcvalue(const Type *dtype, istream& in, bool& err) {
    vector<uint8_t> packed = parse_packed(dtype, in, err);
    if(err) { return Value(dtype); }
    else { return Value(dtype, packed); }
}
vector<uint8_t> parse_packed(const Type *dtype, const string& formatted, bool& err) {
    istringstream strm(formatted);
    return parse_packed(dtype, strm, err);
}
vector<uint8_t> parse_packed(const Type *dtype, istream& in, bool& err) {
    vector<uint8_t> value;
    try {
        dcparser_init_value(in, "parse_value()", dtype, value);
        dcparse();
        dcparser_cleanup();
    } catch(const exception& e) {
        err = true;
        return vector<uint8_t>();
    }

    if(dc_errors() > 0) {
        err = true;
        return vector<uint8_t>();
    }

    err = false;
    return value;
}


bool check_depth()
{
	return (!type_stack.empty() && current_depth == type_stack.top().depth);
}

void depth_error(string what)
{
	if(type_stack.empty() || current_depth < type_stack.top().depth) {
		dcerror("Too many nested values while parsing value for " + what + ".");
	} else {
		dcerror("Too few nested values while parsing value for " + what + ".");
	}
}

void depth_error(int depth, string what)
{
	if(current_depth > depth) {
		dcerror("Too few nested values before this " + what + " value.");
	} else {
		dcerror("Too many nested values before this " + what + " value.");
	}
}

Value number_value(const Type *type, double &number)
{
	switch(type->subtype()) {
	case kTypeFloat32:
		if(isinf(float(number))) { dcerror("Value is out of range for type 'float32'."); }
		{
			Value value(type);
			value.m_float32 = number;
			return value;
		}
	case kTypeFloat64:
		{
			Value value(type);
			value.m_float64 = number;
			return value;
		}
	case kTypeInt8:
	case kTypeInt16:
	case kTypeInt32:
	case kTypeInt64:
	case kTypeChar:
	case kTypeUint8:
	case kTypeUint16:
	case kTypeUint32:
	case kTypeUint64:
		dcerror("Cannot use floating-point value for integer datatype.");
		return Value();
	default:
		dcerror("Cannot use floating-point value for non-numeric datatype.");
		return Value();
	}
}

Value number_value(const Type *type, int64_t &number)
{
	switch(type->subtype()) {
	case kTypeInt8:
		if(INT8_MIN > number || number > INT8_MAX) {
			dcerror("Signed integer out of range for type 'int8'.");
		} else {
			Value value(type);
			value.m_int8 = number;
			return value;
		}
	case kTypeInt16:
		if(INT16_MIN > number || number > INT16_MAX) {
			dcerror("Signed integer out of range for type 'int16'.");
		} else {
			Value value(type);
			value.m_int16 = number;
			return value;
		}
	case kTypeInt32:
		if(INT32_MIN > number || number > INT32_MAX) {
			dcerror("Signed integer out of range for type 'int32'.");
		} else {
			Value value(type);
			value.m_int32 = number;
			return value;
		}
	case kTypeInt64:
		{
			Value value(type);
			value.m_int64 = number;
			return value;
		}
	case kTypeChar:
	case kTypeUint8:
	case kTypeUint16:
	case kTypeUint32:
	case kTypeUint64:
		if(number < 0) {
			dcerror("Can't use negative value for unsigned integer datatype.");
		} else {
			uint64_t value = number;
			return number_value(type, value);
		}
	case kTypeFloat32:
	case kTypeFloat64:
		{
			double value = number;
			return number_value(type, value);
		}
	default:
		dcerror("Cannot use signed integer value for non-numeric datatype.");
		return Value();
	}
}

Value number_value(const Type *type, uint64_t &number)
{
	switch(type->subtype()) {
	case kTypeChar:
		if(number > UINT8_MAX) {
			dcerror("Unsigned integer out of range for type 'char'.");
		} else {
			Value value(type);
			value.m_char = number;
			return value;
		}
	case kTypeUint8:
		if(number > UINT8_MAX) {
			dcerror("Unsigned integer out of range for type 'uint8'.");
		} else {
			Value value(type);
			value.m_uint8 = number;
			return value;
		}
	case kTypeUint16:
		if(number > UINT16_MAX) {
			dcerror("Unsigned integer out of range for type 'uint16'.");
		} else {
			Value value(type);
			value.m_uint16 = number;
			return value;
		}
	case kTypeUint32:
		if(number > UINT32_MAX) {
			dcerror("Unsigned integer out of range for type 'uint32'.");
		} else {
			Value value(type);
			value.m_uint32 = number;
			return value;
		}
	case kTypeUint64:
		{
			Value value(type);
			value.m_uint64 = number;
			return value;
		}
	case kTypeInt8:
	case kTypeInt16:
	case kTypeInt32:
	case kTypeInt64:
		if(number > INT64_MAX) {
			dcerror("Unsigned integer out of range for signed integer datatype.");
		} else {
			int64_t value = number;
			return number_value(type, value);
		}
	case kTypeFloat32:
	case kTypeFloat64:
		{
			double value = number;
			return number_value(type, value);
		}
	default:
		dcerror("Cannot use unsigned integer value for non-numeric datatype.");
		return Value();
	}
}


} // close namespace bamboo
