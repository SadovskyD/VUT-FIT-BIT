library IEEE;
use IEEE.STD_LOGIC_1164.all;
USE ieee.numeric_std.ALL; 

package matrix_pack is
  
function GETID( X, Y, ROWS, COLS: integer ) return integer;
type DIRECTION_T is ( DIR_LEFT, DIR_RIGHT );
end matrix_pack;

package body matrix_pack is

function GETID( X, Y, ROWS, COLS: integer) return integer is
  variable id : integer := 0;
  begin 
  
  id := (rows*(X mod COLS))+(Y mod ROWS);
  
  return id;
  end;

end matrix_pack;