#ifndef ABSYN_H_
#define ABSYN_H_

typedef int A_pos;

typedef struct A_var_ *A_var;
typedef struct A_exp_ *A_exp;
typedef struct A_dec_ *A_dec;
typedef struct A_ty_ *A_ty;

typedef struct A_decList_ *A_decList;
typedef struct A_expList_ *A_expList;
typedef struct A_field_ *A_field;
typedef struct A_fieldList_ *A_fieldList;
typedef struct A_efield_ *A_efield;
typedef struct A_efieldList_ *A_efieldList;

typedef enum {A_notOp, A_bnotOp} A_unoper; //A_ptrOp, A_adrOp, 

typedef enum {A_plusOp, A_minusOp, A_timesOp, A_divideOp, A_modOp, 
				A_leftOp, A_rightOp, A_andOp, A_eorOp, A_orOp, A_landOp, 
				A_lorOp, 
				A_eqOp, A_neqOp, A_ltOp, A_leOp, A_gtOp, A_geOp} A_oper;

struct A_var_ {
	enum {A_simpleVar, A_fieldVar, A_subscriptVar, A_addressVar} kind;
	A_pos pos;
	union {
		S_symbol simple;
		struct {A_var var; S_symbol sym;} field;
		struct {A_var var; A_exp exp;} subscript;
		A_var address;
	} u;
};

struct A_exp_ {
	enum {
		A_varExp, A_nilExp, A_intExp, A_charExp, A_floatExp, A_stringExp, A_callExp, A_conExp,
		A_opExp, A_unaryExp, A_seqExp, A_assignExp, A_ifExp,
		A_whileExp, A_forExp, A_breakExp, A_continueExp, A_letExp, //A_arrayExp,
		A_caseExp, A_switchExp, A_returnExp 
	} kind;
    A_pos pos;
    union {
		A_var var;
		int intt;
		double floatt;
		char charr;
		string stringg;
		struct {S_symbol func; A_expList args;} call;
		struct {A_exp left; A_exp mid; A_exp right;} con;
		struct {A_oper oper; A_exp left; A_exp right;} op;
		struct {A_unoper unoper; A_exp exp;} unary;
		A_expList seq;
		struct {A_var var; A_exp exp;} assign;
		struct {A_exp test, then, elsee;} iff; /* elsee is optional */
		struct {A_exp test, body;} whilee;
		struct {A_exp e1, e2, e3, body;} forr;
		struct {A_decList decs; A_exp body;} let;
		struct {A_exp constant, body;} casee;
		struct {A_exp test; A_expList bodyList;} switchh;
		struct {A_exp res;} returnn;
	} u;
};
struct A_dec_ {
	enum {A_functionDec, A_varDec, A_structDec} kind;
    A_pos pos;
	union {
		struct {S_symbol name; A_fieldList params; A_ty result; A_exp body;} function;
		struct {A_efieldList varList; A_ty typ; bool escape;} var; /* escape may change after the initial declaration */
		struct {A_ty typ; A_fieldList structure;} structt;
	} u;
};

struct A_ty_ {
	enum {A_nameTy, A_arrayTy, A_structTy} kind;
	union {
		S_symbol name;
		struct {
			A_ty tar;
			int length;
		} array;
	} u;
};

/* Linked lists and nodes of lists */

struct A_field_ {S_symbol name; A_ty typ; A_pos pos; bool escape;};
struct A_fieldList_ {A_field head; A_fieldList tail;};
struct A_expList_ {A_exp head; A_expList tail;};
struct A_decList_ {A_dec head; A_decList tail;};
struct A_efield_ {S_symbol name; A_exp exp;};
struct A_efieldList_ {A_efield head; A_efieldList tail;};


/* Function Prototypes */
A_var A_SimpleVar(A_pos pos, S_symbol sym);
A_var A_FieldVar(A_pos pos, A_var var, S_symbol sym);
A_var A_SubscriptVar(A_pos pos, A_var var, A_exp exp);
A_var A_AddressVar(A_pos pos, A_var var);

A_exp A_VarExp(A_pos pos, A_var var);
A_exp A_NilExp(A_pos pos);
A_exp A_IntExp(A_pos pos, int i);
A_exp A_FloatExp(A_pos pos, double d);
A_exp A_CharExp(A_pos pos, char c);
A_exp A_StringExp(A_pos pos, string s);
A_exp A_CallExp(A_pos pos, S_symbol func, A_expList args);
A_exp A_ConExp(A_pos pos, A_exp left, A_exp mid, A_exp right);
A_exp A_OpExp(A_pos pos, A_oper oper, A_exp left, A_exp right);
A_exp A_UnaryExp(A_pos pos, A_unoper oper, A_exp exp);
A_exp A_SeqExp(A_pos pos, A_expList seq);
A_exp A_AssignExp(A_pos pos, A_var var, A_exp exp);
A_exp A_IfExp(A_pos pos, A_exp test, A_exp then, A_exp elsee);
A_exp A_WhileExp(A_pos pos, A_exp test, A_exp body);
A_exp A_ForExp(A_pos pos, A_exp e1, A_exp e2, A_exp e3, A_exp body);
A_exp A_BreakExp(A_pos pos);
A_exp A_ContinueExp(A_pos pos);
A_exp A_LetExp(A_pos pos, A_decList decs, A_exp body);
A_exp A_CaseExp(A_pos pos, A_exp constant, A_exp body);
A_exp A_SwitchExp(A_pos pos, A_exp test, A_expList body);//
A_exp A_ReturnExp(A_pos pos, A_exp res);

A_dec A_FunctionDec(A_pos pos, S_symbol name, A_fieldList params, A_ty result, A_exp body);

//variable declaration
A_dec A_VarDec(A_pos pos, A_efieldList varList, A_ty typ);
A_efieldList A_EfieldList(A_efield head, A_efieldList tail);
A_efield A_Efield(S_symbol name, A_exp init);

//struct declaration
A_dec A_StructDec(A_pos pos, A_ty typ, A_fieldList structure);
A_fieldList A_FieldList(A_field head, A_fieldList tail);
A_fieldList A_MergeFieldList(A_fieldList head, A_fieldList tail);
A_field A_Field(A_pos pos, S_symbol name, A_ty typ);

A_ty A_NameTy(S_symbol name);//simple type
A_ty A_StructTy(S_symbol name);//struct type
A_ty A_ArrayTy(A_ty tar, int length);//array type

A_decList A_DecList(A_dec head, A_decList tail);

A_expList A_ExpList(A_exp head, A_expList tail);

#endif