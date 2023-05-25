<?php
/*							 
 Script parse.php for IPP   
 Dmytro Sadovskyi (xsadov06)*/

//Na základě tokenů získaných z funkce scan_next () generuje a výpisuje na STDOUT XML reprezentaci kódu IPPcode23.
function check_syntax()
{
    global $instruction_list;
    global $tmp_doc;

    $inst_counter = 0;

    $analyzed_sentence = array();
	
	$tmp_doc = new DOMDocument('1.0', 'UTF-8');
	$tmp_doc->formatOutput = true;

	$xml_final = $tmp_doc->createElement('program');
	$xml_final->setAttribute('language', 'IPPcode23');
	$xml_final = $tmp_doc->appendChild($xml_final);

    $analyzed_sentence = scan_next();
	
    if (($analyzed_sentence[0] != NULL) and ($analyzed_sentence[0][0] != NULL))
	{
        if ($analyzed_sentence[0][0] != TOK_HEADER) exit(CODE_BAD_HEAD);
    }
    else exit(CODE_OTHER);

    while ($analyzed_sentence[0][0] != TOK_EOF)
	{
        $analyzed_sentence = scan_next();

        if ($analyzed_sentence[0][0] == TOK_EOF) break;
        elseif ($analyzed_sentence[0][0] == TOK_INSTRUCTION)
		{
            $inst_counter++;

            $instruction = $tmp_doc->createElement("instruction");
            $instruction->setAttribute("order", $inst_counter);
            $instruction->setAttribute("opcode", $instruction_list[$analyzed_sentence[0][1]]);
            
            switch ($analyzed_sentence[0][1])
			{
				//MOVE INT2CHAR STRLEN TYPE NOT
                case 0: case 18: case 19: case 24: case 27:
                    if (count($analyzed_sentence) != (3)) exit(CODE_OTHER);
                    if ($analyzed_sentence[1][0] != TOK_VAR) exit(CODE_OTHER);
					if ($analyzed_sentence[2][0] != TOK_VAR && $analyzed_sentence[2][0] != TOK_CONST) exit(CODE_OTHER);
                    
					$var_arg = $tmp_doc->createElement("arg1", htmlspecialchars($analyzed_sentence[1][1]));
                    $var_arg->setAttribute("type", "var");
                    $instruction->appendChild($var_arg);
					
                    if ($analyzed_sentence[2][0] == TOK_VAR)
					{
                        $var_arg = $tmp_doc->createElement("arg2", htmlspecialchars($analyzed_sentence[2][1]));
                        $var_arg->setAttribute("type", "var");
                    }
                    else
					{
                        $var_arg = $tmp_doc->createElement("arg2", htmlspecialchars($analyzed_sentence[2][2]));
                        $var_arg->setAttribute("type", $analyzed_sentence[2][1]);
                    }
                    $instruction->appendChild($var_arg);
                break;
				
				//CALL LABEL JUMP
                case 5: case 28: case 29:
                    if (count($analyzed_sentence) != (2)) exit(CODE_OTHER);
					if ($analyzed_sentence[1][0] != TOK_LABEL) exit(CODE_OTHER);

                    $var_arg = $tmp_doc->createElement("arg1", htmlspecialchars($analyzed_sentence[1][1]));
                    $var_arg->setAttribute("type", "label");
                    $instruction->appendChild($var_arg);
                break;
				
				//CREATEFRAME PUSHFRAME POPFRAME RETURN BREAK
                case 1: case 2: case 3: case 6: case 34:
					if (count($analyzed_sentence) != (1)) exit(CODE_OTHER);
                break;
				
                //DEFVAR POPS
                case 4: case 8:
					if (count($analyzed_sentence) != (2)) exit(CODE_OTHER);
                    if ($analyzed_sentence[1][0] != TOK_VAR) exit(CODE_OTHER);

                    $var_arg = $tmp_doc->createElement("arg1", htmlspecialchars($analyzed_sentence[1][1]));
                    $var_arg->setAttribute("type", "var");
                    $instruction->appendChild($var_arg);
                break;
				
				//PUSHS WRITE EXIT DPRINT
                case 7: case 22: case 32: case 33:
                    if (count($analyzed_sentence) != (2)) exit(CODE_OTHER);
					if ($analyzed_sentence[1][0] != TOK_VAR && $analyzed_sentence[1][0] != TOK_CONST) exit(CODE_OTHER);
                    if ($analyzed_sentence[1][0] == TOK_VAR)
					{
                        $var_arg = $tmp_doc->createElement("arg1", htmlspecialchars($analyzed_sentence[1][1]));
                        $var_arg->setAttribute("type", "var");
                    }
                    else
					{
                        $var_arg = $tmp_doc->createElement("arg1", htmlspecialchars($analyzed_sentence[1][2]));
                        $var_arg->setAttribute("type", $analyzed_sentence[1][1]);
                    }

                    $instruction->appendChild($var_arg);
                break;
				
				//READ
                case 21:
                    if (count($analyzed_sentence) != (3)) exit(CODE_OTHER);
                    if ($analyzed_sentence[1][0] != TOK_VAR) exit(CODE_OTHER);
                    if ($analyzed_sentence[2][0] == TOK_TYPE)
					{
                        $var_arg = $tmp_doc->createElement("arg1", htmlspecialchars($analyzed_sentence[1][1]));
                        $var_arg->setAttribute("type", "var");
                        $instruction->appendChild($var_arg);
                        $var_arg = $tmp_doc->createElement("arg2", htmlspecialchars($analyzed_sentence[2][1]));
                        $var_arg->setAttribute("type", "type");
                        $instruction->appendChild($var_arg);
                    }
                    else exit(CODE_OTHER);
                break;
				
				// ADD SUB MUL IDIV LT GT EG AND OR
                case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 20: case 23: case 25: case 26:
                    if (count($analyzed_sentence) != (4)) exit(CODE_OTHER);
                    if ($analyzed_sentence[1][0] != TOK_VAR) exit(CODE_OTHER);
					if ($analyzed_sentence[2][0] != TOK_VAR && $analyzed_sentence[2][0] != TOK_CONST) exit(CODE_OTHER);
					if ($analyzed_sentence[3][0] != TOK_VAR && $analyzed_sentence[3][0] != TOK_CONST) exit(CODE_OTHER);

                    $var_arg = $tmp_doc->createElement("arg1", htmlspecialchars($analyzed_sentence[1][1]));
                    $var_arg->setAttribute("type", "var");
                    $instruction->appendChild($var_arg);

                    if ($analyzed_sentence[2][0] == TOK_VAR)
					{
                        $var_arg = $tmp_doc->createElement("arg2", htmlspecialchars($analyzed_sentence[2][1]));
                        $var_arg->setAttribute("type", "var");
                    }
                    else
					{
                        $var_arg = $tmp_doc->createElement("arg2", htmlspecialchars($analyzed_sentence[2][2]));
                        $var_arg->setAttribute("type", $analyzed_sentence[2][1]);
                    }
                    $instruction->appendChild($var_arg);

                    if ($analyzed_sentence[3][0] == TOK_VAR)
					{
                        $var_arg = $tmp_doc->createElement("arg3", htmlspecialchars($analyzed_sentence[3][1]));
                        $var_arg->setAttribute("type", "var");
                    }
                    else
					{
                        $var_arg = $tmp_doc->createElement("arg3", htmlspecialchars($analyzed_sentence[3][2]));
                        $var_arg->setAttribute("type", $analyzed_sentence[3][1]);
                    }
                    $instruction->appendChild($var_arg);
                break;

                case 30: case 31:
                    if (count($analyzed_sentence) != (4)) exit(CODE_OTHER);
                    if ($analyzed_sentence[1][0] != TOK_LABEL) exit(CODE_OTHER);
					if ($analyzed_sentence[2][0] != TOK_VAR && $analyzed_sentence[2][0] != TOK_CONST) exit(CODE_OTHER);
					if ($analyzed_sentence[3][0] != TOK_VAR && $analyzed_sentence[3][0] != TOK_CONST) exit(CODE_OTHER);
                    $var_arg = $tmp_doc->createElement("arg1", htmlspecialchars($analyzed_sentence[1][1]));
                    $var_arg->setAttribute("type", "label");
                    $instruction->appendChild($var_arg);

                    if ($analyzed_sentence[2][0] == TOK_VAR)
					{
                        $var_arg = $tmp_doc->createElement("arg2", htmlspecialchars($analyzed_sentence[2][1]));
                        $var_arg->setAttribute("type", "var");
                    }
                    else
					{
                        $var_arg = $tmp_doc->createElement("arg2", htmlspecialchars($analyzed_sentence[2][2]));
                        $var_arg->setAttribute("type", $analyzed_sentence[2][1]);
                    }
                    $instruction->appendChild($var_arg);

                    if ($analyzed_sentence[3][0] == TOK_VAR)
					{
                        $var_arg = $tmp_doc->createElement("arg3", htmlspecialchars($analyzed_sentence[3][1]));
                        $var_arg->setAttribute("type", "var");
                    }
                    else
					{
                        $var_arg = $tmp_doc->createElement("arg3", htmlspecialchars($analyzed_sentence[3][2]));
                        $var_arg->setAttribute("type", $analyzed_sentence[3][1]);
                    }
                    $instruction->appendChild($var_arg);
                break;

                default:
					exit(CODE_BAD_OPCODE);
                    break;
            }
        }
        else exit(CODE_BAD_OPCODE);

        $xml_final->appendChild($instruction);
    }
    echo $tmp_doc->saveXML();
}