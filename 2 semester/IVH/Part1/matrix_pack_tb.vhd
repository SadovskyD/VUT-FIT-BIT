LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
use work.matrix_pack.all; 

ENTITY matrix_pack_tb IS
END matrix_pack_tb;
 
ARCHITECTURE behavior OF matrix_pack_tb IS 
signal X,Y,ROWS,COLS,ID: integer := 1;
   
BEGIN
 
 process is
 begin
 
 X <= 1;
 Y <= 2;
 COLS <= 6;
 ROWS <= 3;
 wait for 10 ns;
 assert (id = 5)  report "Result does not match what was expected in the test 1";

 X <= 5;
 Y <= 1;
 wait for 10 ns;
 assert (id = 16)  report "Result does not match what was expected in the test 2";
 
 X <= -2;
 Y <= 3;
 COLS <= 3;
 ROWS <= 5;
 wait for 10 ns;
 assert (id = 8)  report "Result does not match what was expected in the test 3";

 X <= -2;
 Y <= -2;
 wait for 10 ns;
 assert (id = 8)  report "Result does not match what was expected in the test 4";

 X <= 6;
 Y <= 0;
 COLS <= 2;
 ROWS <= 3;
 wait for 10 ns;
 assert (id = 0)  report "Result does not match what was expected in the test 5";
 
 X <= 3;
 Y <= 4;
 wait for 10 ns;
 assert (id = 4)  report "Result does not match what was expected in the test 6";
 wait;
 
 end process;
 ID <= getid(X,Y,ROWS,COLS);

END;