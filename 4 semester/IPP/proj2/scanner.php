<?php
/*							 
 Script parse.php for IPP   
 Dmytro Sadovskyi (xsadov06)*/
 
 //Čte kód ze standardního vstupu, na základě kódu generuje token a vrací jej jako pole.
function scan_next()
{
    global $stdin;
	global $instruction_list;

    $prepared_sentence = array();
    $start_sentense = true;

    while (true)
	{  
        if (($sentence = fgets($stdin)) == false)
		{
            array_push($prepared_sentence, array(TOK_EOF));
            return $prepared_sentence;
        }

        if (preg_match('~^\s*#~', $sentence) || preg_match('~^\s*$~', $sentence))
		{
            continue;
        }

        $split_comment = explode('#', $sentence);
        $split_word = preg_split('~\s+~', $split_comment[0]);
        if (end($split_word) == "")
		{
            array_pop($split_word);
        }
        if ($split_word[0] == "")
		{
            array_shift($split_word);
        }

        break;
    }
    foreach($split_word as $word)
	{

        if (preg_match('~@~', $word))
		{
            if (preg_match('~^(int|bool|string|nil)~', $word))
			{
                if (
                    preg_match('~^int@[+-]?[0-9]+$~', $word) || preg_match('~^nil@nil$~', $word) || preg_match('~^bool@(true|false)$~', $word) || preg_match('~^string@$~', $word) ||
                    (preg_match('~^string@~', $word) && !preg_match('~(\\\\($|\p{S}|\p{P}\p{Z}|\p{M}|\p{L}|\p{C})|(\\\\[0-9]{0,2}($|\p{S}|\p{P}\p{Z}|\p{M}|\p{L}|\p{C}))| |#)~', $word))
                )
				{
                    $type_and_data = explode('@', $word, 2);
                    $final = array();
                    array_push($final, TOK_CONST);
                    array_push($final, $type_and_data[0]);
                    array_push($final, $type_and_data[1]);
                    array_push($prepared_sentence, $final);
                }
                else exit(CODE_OTHER);
            }
            else
			{
                if (preg_match('~^(GF|TF|LF)@[a-zA-Z_\-$&%*!?]*$~', $word))
				{
                    array_push($prepared_sentence, array(TOK_VAR, $word));
                }
                else exit(CODE_OTHER);
            }
        }
        else
		{
            if (preg_match('~^(int|bool|string|nil)$~', $word))
			{
                array_push($prepared_sentence, array(TOK_TYPE, $word));
            }
            else
			{
                if (preg_match('~^\.ippcode23$~i', $word))
				{
                    array_push($prepared_sentence, array(TOK_HEADER));
                }
                else
				{
                    $instr_number = -1;
                    
					for ($i=0; $i <= 34; $i++)
					{ 
						if (strcasecmp($word, $instruction_list[$i]) == 0)
						{
							$instr_number = $i;
							break;
						}
					}
                    
					if ($instr_number != -1 && $start_sentense == true)
					{
                        array_push($prepared_sentence, array(TOK_INSTRUCTION, $instr_number));
                    }
                    else
					{
                        if (preg_match('~^[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$~', $word))
						{
                            array_push($prepared_sentence, array(TOK_LABEL, $word));
                        }
                        else exit(CODE_BAD_OPCODE);
                    }
                }
            }
        }
        $start_sentense = false;
    }
    return $prepared_sentence;
}