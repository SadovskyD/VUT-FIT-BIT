<?php
/*							 
 Script parse.php for IPP   
 Dmytro Sadovskyi (xsadov06)*/

include 'scanner.php';
include 'syntax.php';
include 'def.php';

$stdout = fopen('php://stdout', 'w');
$stdin = fopen('php://stdin', 'r');

if ($argc > 1)
{
	for ($i=1; $i < $argc; $i++)
	{ 
		if (($argv[$i] == "--help") or ($argv[$i] == "-help"))
		{
			if ($argc > 2)
			{
				echo("Too much args. with --help\n");
				exit(CODE_BAD_S_ARGS);
			}
			echo ("The script reads the source code in IPPcode23 checks the lexical and syntactic correctness of the code and prints an XML representation of the program to standard output\n");
			echo("Usage: parser.php [options] < inputFile\n");
			exit(CODE_OK);
		}
	}
}

check_syntax();

exit(CODE_OK);
?>