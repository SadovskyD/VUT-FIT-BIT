<?php
/*							 
 Script parse.php for IPP   
 Dmytro Sadovskyi (xsadov06)*/
 
define('CODE_OK', 0);
define('CODE_BAD_S_ARGS', 10);
define('CODE_BAD_HEAD', 21);
define('CODE_BAD_OPCODE', 22);
define('CODE_OTHER', 23);

define('TOK_EOF', 1);
define('TOK_CONST', 2);
define('TOK_VAR', 3);
define('TOK_HEADER', 4);
define('TOK_LABEL', 5);
define('TOK_INSTRUCTION', 6);
define('TOK_TYPE', 7);

$instruction_list = array(
"MOVE", //0
"CREATEFRAME",
"PUSHFRAME",
"POPFRAME",
"DEFVAR",
"CALL",//5
"RETURN",
"PUSHS",
"POPS",
"ADD",
"SUB",//10
"MUL",
"IDIV",
"LT",
"GT",
"EQ",//15
"AND",
"OR",
"NOT",
"INT2CHAR",
"STRI2INT",//20
"READ",
"WRITE",
"CONCAT",
"STRLEN",
"GETCHAR",//25
"SETCHAR",
"TYPE",
"LABEL",
"JUMP",
"JUMPIFEQ",//30
"JUMPIFNEQ",
"EXIT",
"DPRINT",
"BREAK"//34
);