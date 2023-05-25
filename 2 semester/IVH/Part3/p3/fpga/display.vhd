-- Blikani ledkami
-- uvodni priklad pro IVH - Vojtech Mrazek
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- DO entity nezasahujte, bude pouzita k simulaci!
entity display is
	port (
		data : in std_logic_vector(127 downto 0); -- data k zobrazeni, format je na popsany dole
		reset : in std_logic;
		clk : in std_logic; -- hodiny 25 MHz
		smclk : in std_logic; -- hodiny 7.3728MHz
		A : out std_logic_vector(3 downto 0);
		R : out std_logic_vector(7 downto 0)
	);
end entity display;

architecture behv of display is
   	function log2(A: integer) return integer is
		variable bits : integer := 0;
		variable b : integer := 1;
	begin
		while (b <= a) loop
			b := b * 2;
			bits := bits + 1;
		end loop;
		return bits;
	end function;
	constant TMR : natural := 20; -- 18 je normalni
   signal cnt : std_logic_vector(log2(TMR) downto 0) := (others => '0');
   signal A_temp : std_logic_vector(3 downto 0) := (others => '0');
   signal R_temp : std_logic_vector(7 downto 0) := (others => '0');
	-- definujte si libovolne signaly
   signal en : std_logic;
begin
	
	counter: process(clk)
	begin
		if reset = '1' then
			cnt <= (others => '0');
		elsif ( rising_edge(clk) ) then
			cnt <= cnt + 1;
			en <= '0';
			if cnt = TMR then
				cnt <= (others => '0');
				en <= '1';
			end if;
		end if;
	end process;
	
	proc2: process(en)
		function GETID( X, Y, ROWS, COLS: integer) return integer is
			variable id : integer := 0;
		begin 
			id := (rows*(X mod COLS))+(Y mod ROWS); 
			return id;
		end function GETID;
		variable x : integer range 0 to 15 := 15;
	begin
		if reset = '1' then
		A_temp <= (others => '0');
		R_temp <= (others => '0');
		x := 15;
		elsif ( rising_edge(en) ) then
			R_temp(7) <= data(GETID(0, x, 16, 8));
			R_temp(6) <= data(GETID(1, x, 16, 8));
			R_temp(5) <= data(GETID(2, x, 16, 8));
			R_temp(4) <= data(GETID(3, x, 16, 8));
			R_temp(3) <= data(GETID(4, x, 16, 8));
			R_temp(2) <= data(GETID(5, x, 16, 8));
			R_temp(1) <= data(GETID(6, x, 16, 8));
			R_temp(0) <= data(GETID(7, x, 16, 8));
			x := x-1;
			A_temp <= A_temp +1;
		end if;
	end process;
	
	A <= A_temp-1;
	R <= R_temp;
end behv;

