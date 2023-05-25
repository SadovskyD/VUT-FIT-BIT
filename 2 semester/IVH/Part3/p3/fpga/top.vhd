-- Blikani ledkami
-- uvodni priklad pro IVH - Vojtech Mrazek
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

architecture main of tlv_gp_ifc is
   signal A : std_logic_vector(3 downto 0) := (others => '0');
   signal R : std_logic_vector(7 downto 0); 
	signal DATA : std_logic_vector(127 downto 0) :=  -- todo: zde zmente data na to, co chcete zobrazit
		"0000000000000000" &
		"0000000000000000" &
		"0000110000110000" &
		"0000101001000000" &
		"0000101000100000" &
		"0000101000010000" &
		"0000110001100000" &
		"0000000000000000";

begin


	-- do tohoto souboru v 3. projektu nezasahujte
	-- pro finalni projekt jej muzete vyuzit a libovolne upravit!

	dis : entity work.display port map (
		CLK => CLK,
		SMCLK => SMCLK,
		RESET => RESET,
		DATA => DATA,
		A => A,
		R => R

	);

    -- mapovani vystupu
    -- nemenit
    X(6) <= A(3);
    X(8) <= A(1);
    X(10) <= A(0);
    X(7) <= '0'; -- en_n
    X(9) <= A(2);

    X(16) <= R(1);
    X(18) <= R(0);
    X(20) <= R(7);
    X(22) <= R(2);
  
    X(17) <= R(4);
    X(19) <= R(3);
    X(21) <= R(6);
    X(23) <= R(5);
end main;

